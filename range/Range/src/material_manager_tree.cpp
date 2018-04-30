/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_manager_tree.cpp                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   8-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Material manager tree class definition              *
 *********************************************************************/

#include <QMessageBox>

#include "action_list.h"
#include "material_manager_tree.h"
#include "material_list.h"
#include "session.h"

typedef enum _MaterialTreeColumn
{
    MATERIAL_TREE_COLUMN_NAME = 0,
    MATERIAL_TREE_COLUMN_FILE,
    MATERIAL_TREE_N_COLUMNS
} MaterialTreeColumn;

MaterialManagerTree::MaterialManagerTree(ActionList *actionList,
                                         QWidget *parent) :
    QTreeWidget(parent),
    actionList(actionList),
    problemTypeMask(R_PROBLEM_NONE),
    doNotPopulate(false)
{
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSortingEnabled(true);
    this->sortByColumn(MATERIAL_TREE_COLUMN_NAME,Qt::AscendingOrder);

    this->setColumnCount(MATERIAL_TREE_N_COLUMNS);

    QTreeWidgetItem* headerItem = new QTreeWidgetItem();
    headerItem->setText(MATERIAL_TREE_COLUMN_NAME,QString("Name"));
    headerItem->setText(MATERIAL_TREE_COLUMN_FILE,QString("File"));
    this->setHeaderItem(headerItem);
    this->setRootIsDecorated(false);
    this->resizeColumnToContents(MATERIAL_TREE_COLUMN_NAME);
    this->resizeColumnToContents(MATERIAL_TREE_COLUMN_FILE);

    this->populate();

    this->setContextMenuPolicy(Qt::ActionsContextMenu);

    this->addAction(actionList->getAction(ACTION_MATERIAL_NEW));
    this->addAction(actionList->getAction(ACTION_MATERIAL_IMPORT));
    this->addAction(actionList->getAction(ACTION_MATERIAL_DELETE));

    QObject::connect(&MaterialList::getInstance(),
                     &MaterialList::materialAdded,
                     this,
                     &MaterialManagerTree::onMaterialAdded);
    QObject::connect(&MaterialList::getInstance(),
                     &MaterialList::materialRemoved,
                     this,
                     &MaterialManagerTree::onMaterialRemoved);
    QObject::connect(&MaterialList::getInstance(),
                     &MaterialList::materialRenamed,
                     this,
                     &MaterialManagerTree::onMaterialRenamed);
    QObject::connect(&MaterialList::getInstance(),
                     &MaterialList::materialChanged,
                     this,
                     &MaterialManagerTree::onMaterialChanged);

    QObject::connect(this,
                     &MaterialManagerTree::itemChanged,
                     this,
                     &MaterialManagerTree::onItemChanged);
    QObject::connect(this,
                     &MaterialManagerTree::itemDoubleClicked,
                     this,
                     &MaterialManagerTree::onItemDoubleClicked);
    QObject::connect(this,
                     &MaterialManagerTree::itemSelectionChanged,
                     this,
                     &MaterialManagerTree::onItemSelectionChanged);

    QObject::connect(&Session::getInstance(),
                     &Session::modelSelectionChanged,
                     this,
                     &MaterialManagerTree::onModelSelectionChanged);
    QObject::connect(&Session::getInstance(),
                     &Session::modelAdded,
                     this,
                     &MaterialManagerTree::onModelAdded,
                     Qt::BlockingQueuedConnection);
    QObject::connect(&Session::getInstance(),
                     &Session::problemSelectionChanged,
                     this,
                     &MaterialManagerTree::onProblemChanged);
    QObject::connect(this,&MaterialManagerTree::materialSelected,
                     this,&MaterialManagerTree::onMaterialSelected);
}

void MaterialManagerTree::populate(void)
{
    if (this->doNotPopulate)
    {
        return;
    }

    RProblemTypeMask modelPrbTypeMask = R_PROBLEM_NONE;
    REntityGroupTypeMask elementGroupTypeMask = R_ENTITY_GROUP_NONE;
    QList<SessionEntityID> selectedEntityIDs = Session::getInstance().getSelectedEntityIDs();

    for (int i=0;i<selectedEntityIDs.size();i++)
    {
        elementGroupTypeMask |= selectedEntityIDs[i].getType();
        modelPrbTypeMask |= Session::getInstance().getModel(selectedEntityIDs[i].getMid()).getProblemTaskTree().getProblemTypeMask();
    }

    std::vector<RMaterialPropertyType> propertyTypes = RMaterialProperty::getTypes(modelPrbTypeMask);
    std::vector<QString> materialNames = MaterialList::getInstance().getMaterialNames(propertyTypes);

    for (uint i=0;i<materialNames.size();i++)
    {
        bool applies = false;
        if (elementGroupTypeMask & R_ENTITY_GROUP_POINT ||
            elementGroupTypeMask & R_ENTITY_GROUP_LINE ||
            elementGroupTypeMask & R_ENTITY_GROUP_SURFACE ||
            elementGroupTypeMask & R_ENTITY_GROUP_VOLUME)
        {
            applies = true;
        }
        if (!applies)
        {
            continue;
        }
        this->addMaterial(materialNames[i],false);
    }

    this->updateCheckStates();
    this->actionList->processAvailability();
}

void MaterialManagerTree::updateCheckStates(void)
{
    QList<SessionEntityID> selectedEntityIDs = Session::getInstance().getSelectedEntityIDs();
    QMap<QString,bool> selectedMaterialNames;

    for (int i=0;i<selectedEntityIDs.size();i++)
    {
        QString materialName;
        switch (selectedEntityIDs[i].getType())
        {
            case R_ENTITY_GROUP_POINT:
                materialName = Session::getInstance().getModel(selectedEntityIDs[i].getMid()).getPoint(selectedEntityIDs[i].getEid()).getMaterial().getName();
                break;
            case R_ENTITY_GROUP_LINE:
                materialName = Session::getInstance().getModel(selectedEntityIDs[i].getMid()).getLine(selectedEntityIDs[i].getEid()).getMaterial().getName();
                break;
            case R_ENTITY_GROUP_SURFACE:
                materialName = Session::getInstance().getModel(selectedEntityIDs[i].getMid()).getSurface(selectedEntityIDs[i].getEid()).getMaterial().getName();
                break;
            case R_ENTITY_GROUP_VOLUME:
                materialName = Session::getInstance().getModel(selectedEntityIDs[i].getMid()).getVolume(selectedEntityIDs[i].getEid()).getMaterial().getName();
                break;
            default:
                break;
        }
        selectedMaterialNames[materialName] = true;
    }

    Qt::CheckState state;
    switch (selectedMaterialNames.size())
    {
        case 0:
            state = Qt::Unchecked;
            break;
        case 1:
            state = Qt::Checked;
            break;
        default:
            state = Qt::PartiallyChecked;
            break;
    }

    this->blockSignals(true);

    for (int i=0;i<this->topLevelItemCount();i++)
    {
        if (selectedMaterialNames.find(this->topLevelItem(i)->text(MATERIAL_TREE_COLUMN_NAME)) == selectedMaterialNames.end())
        {
            this->topLevelItem(i)->setData(MATERIAL_TREE_COLUMN_NAME,Qt::CheckStateRole,QVariant(Qt::Unchecked));
        }
        else
        {
            this->topLevelItem(i)->setData(MATERIAL_TREE_COLUMN_NAME,Qt::CheckStateRole,QVariant(state));
        }
    }

    this->blockSignals(false);
}

void MaterialManagerTree::updateProblemTypeMask(void)
{
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

    this->problemTypeMask = R_PROBLEM_NONE;
    for (int i=0;i<selectedModelIDs.size();i++)
    {
        this->problemTypeMask |= Session::getInstance().getModel(selectedModelIDs[i]).getProblemTaskTree().getProblemTypeMask();
    }

    this->blockSignals(true);
    for (int i=0;i<this->topLevelItemCount();i++)
    {
        QTreeWidgetItem *item = this->topLevelItem(i);
        RMaterial material = MaterialList::getInstance().get(item->text(MATERIAL_TREE_COLUMN_NAME));
        this->setItemValid(item,material.validForProblemType(this->problemTypeMask));
    }
    this->blockSignals(false);
}

void MaterialManagerTree::setItemValid(QTreeWidgetItem *item, bool valid)
{
    QFont font = item->font(MATERIAL_TREE_COLUMN_NAME);
    font.setBold(valid);
    font.setItalic(!valid);
    item->setFont(MATERIAL_TREE_COLUMN_NAME,font);
}

void MaterialManagerTree::addMaterial(const QString &materialName, bool setSelected)
{
    this->blockSignals(true);

    RMaterial material = MaterialList::getInstance().get(materialName);

    QTreeWidgetItem *item = new QTreeWidgetItem(this);
    item->setText(MATERIAL_TREE_COLUMN_NAME,materialName);
    item->setData(MATERIAL_TREE_COLUMN_NAME,Qt::CheckStateRole,QVariant(Qt::Unchecked));
    item->setData(MATERIAL_TREE_COLUMN_NAME,Qt::UserRole,QVariant(material.getID()));
    this->setItemValid(item,material.validForProblemType(this->problemTypeMask));
    item->setText(MATERIAL_TREE_COLUMN_FILE,MaterialList::getInstance().getFileName(materialName));
    QFont font = item->font(MATERIAL_TREE_COLUMN_FILE);
    font.setItalic(true);
    item->setFont(MATERIAL_TREE_COLUMN_FILE,font);
    this->resizeColumnToContents(MATERIAL_TREE_COLUMN_NAME);
    this->resizeColumnToContents(MATERIAL_TREE_COLUMN_FILE);
    this->blockSignals(false);

    if (setSelected)
    {
        this->clearSelection();
        item->setSelected(true);
    }
}

void MaterialManagerTree::onModelSelectionChanged(uint)
{
    this->updateProblemTypeMask();
    this->updateCheckStates();
}

void MaterialManagerTree::onModelAdded(uint modelID)
{
    QMap<QString,RMaterial> unknownMaterials;
    QMap<QString,RMaterial> differentMaterials;

    // Loop over all element groups and check if it contains unknown material.

    for (uint j=0;j<Session::getInstance().getModel(modelID).getNPoints();j++)
    {
        RMaterial &material = Session::getInstance().getModel(modelID).getPoint(j).getMaterial();
        if (material.getName().isEmpty())
        {
            continue;
        }
        if (MaterialList::getInstance().containName(material.getName()))
        {
            RMaterial cmpMaterial = MaterialList::getInstance().get(material.getName());
            if (!cmpMaterial.propertiesEqual(material))
            {
                differentMaterials[material.getName()] = material;
            }
        }
        else
        {
            if (unknownMaterials.find(material.getName()) == unknownMaterials.end())
            {
                unknownMaterials[material.getName()] = material;
            }
        }
    }
    for (uint j=0;j<Session::getInstance().getModel(modelID).getNLines();j++)
    {
        RMaterial &material = Session::getInstance().getModel(modelID).getLine(j).getMaterial();
        if (material.getName().isEmpty())
        {
            continue;
        }
        if (MaterialList::getInstance().containName(material.getName()))
        {
            RMaterial cmpMaterial = MaterialList::getInstance().get(material.getName());
            if (!cmpMaterial.propertiesEqual(material))
            {
                differentMaterials[material.getName()] = material;
            }
        }
        else
        {
            if (unknownMaterials.find(material.getName()) == unknownMaterials.end())
            {
                unknownMaterials[material.getName()] = material;
            }
        }
    }
    for (uint j=0;j<Session::getInstance().getModel(modelID).getNSurfaces();j++)
    {
        RMaterial &material = Session::getInstance().getModel(modelID).getSurface(j).getMaterial();
        if (material.getName().isEmpty())
        {
            continue;
        }
        if (MaterialList::getInstance().containName(material.getName()))
        {
            RMaterial cmpMaterial = MaterialList::getInstance().get(material.getName());
            if (!cmpMaterial.propertiesEqual(material))
            {
                differentMaterials[material.getName()] = material;
            }
        }
        else
        {
            if (unknownMaterials.find(material.getName()) == unknownMaterials.end())
            {
                unknownMaterials[material.getName()] = material;
            }
        }
    }
    for (uint j=0;j<Session::getInstance().getModel(modelID).getNVolumes();j++)
    {
        RMaterial &material = Session::getInstance().getModel(modelID).getVolume(j).getMaterial();
        if (material.getName().isEmpty())
        {
            continue;
        }
        if (MaterialList::getInstance().containName(material.getName()))
        {
            RMaterial cmpMaterial = MaterialList::getInstance().get(material.getName());
            if (!cmpMaterial.propertiesEqual(material))
            {
                differentMaterials[material.getName()] = material;
            }
        }
        else
        {
            if (unknownMaterials.find(material.getName()) == unknownMaterials.end())
            {
                unknownMaterials[material.getName()] = material;
            }
        }
    }

    bool addUnknown = false;
    if (unknownMaterials.size() > 0)
    {
        QString materialNames;
        QMap<QString,RMaterial>::const_iterator iter;
        for (iter = unknownMaterials.constBegin(); iter != unknownMaterials.constEnd(); ++iter)
        {
            materialNames += "<li><b>" + iter.key() + "</b></li>";
        }

        QString question = "Following materials are not present in your database:<ol>"
                         + materialNames
                         + "</ol>Do you want to add them to the material list?";
        int response = QMessageBox::question(this,
                                             "Unknown materials",
                                             question,
                                             QMessageBox::Yes | QMessageBox::No,
                                             QMessageBox::No);
        if (response == QMessageBox::Yes)
        {
            addUnknown = true;
        }
    }

    if (addUnknown)
    {
        QMap<QString,RMaterial>::iterator iter;
        for (iter = unknownMaterials.begin(); iter != unknownMaterials.end(); ++iter)
        {
            MaterialList::getInstance().add(iter.value());
        }
    }
    else
    {
        QMap<QString,RMaterial>::const_iterator iter;
        for (iter = unknownMaterials.constBegin(); iter != unknownMaterials.constEnd(); ++iter)
        {
            // Remove unknown material from the model's element groups.
            for (uint j=0;j<Session::getInstance().getModel(modelID).getNPoints();j++)
            {
                if (Session::getInstance().getModel(modelID).getPoint(j).getMaterial().getName() == iter.key())
                {
                    Session::getInstance().getModel(modelID).getPoint(j).setMaterial(RMaterial());
                }
            }
            for (uint j=0;j<Session::getInstance().getModel(modelID).getNLines();j++)
            {
                if (Session::getInstance().getModel(modelID).getLine(j).getMaterial().getName() == iter.key())
                {
                    Session::getInstance().getModel(modelID).getLine(j).setMaterial(RMaterial());
                }
            }
            for (uint j=0;j<Session::getInstance().getModel(modelID).getNSurfaces();j++)
            {
                if (Session::getInstance().getModel(modelID).getSurface(j).getMaterial().getName() == iter.key())
                {
                    Session::getInstance().getModel(modelID).getSurface(j).setMaterial(RMaterial());
                }
            }
            for (uint j=0;j<Session::getInstance().getModel(modelID).getNVolumes();j++)
            {
                if (Session::getInstance().getModel(modelID).getVolume(j).getMaterial().getName() == iter.key())
                {
                    Session::getInstance().getModel(modelID).getVolume(j).setMaterial(RMaterial());
                }
            }
        }
    }

    bool addDifferent = false;
    if (differentMaterials.size() > 0)
    {
        QString materialNames;
        QMap<QString,RMaterial>::const_iterator iter;
        for (iter = differentMaterials.constBegin(); iter != differentMaterials.constEnd(); ++iter)
        {
            materialNames += "<li><b>" + iter.key() + "</b></li>";
        }

        QString question = "Model contains following materials which already exist in your database and have different properties:<ol>"
                         + materialNames
                         + "</ol>Do you want to add them to the material list with different names?<br/>If not material from database will be applied.";
        int response = QMessageBox::question(this,
                                             "Different materials",
                                             question,
                                             QMessageBox::Yes | QMessageBox::No,
                                             QMessageBox::No);
        if (response == QMessageBox::Yes)
        {
            addDifferent = true;
        }
    }

    if (addDifferent)
    {
        QMap<QString,RMaterial>::iterator iter;
        for (iter = differentMaterials.begin(); iter != differentMaterials.end(); ++iter)
        {
            RMaterial &rMaterial = iter.value();

            QString materialNameBase = rMaterial.getName() + QString(" - Imported");
            QString materialName = materialNameBase;
            uint nTries = 0;
            while (MaterialList::getInstance().containName(materialName))
            {
                materialName = materialNameBase + " - " + QString::number(++nTries);
            }

            rMaterial.setName(materialName);
            MaterialList::getInstance().add(rMaterial);
        }

        QMap<QString,RMaterial>::const_iterator citer;
        for (citer = differentMaterials.constBegin(); citer != differentMaterials.constEnd(); ++citer)
        {
            // Remove unknown material from the model's element groups.
            for (uint j=0;j<Session::getInstance().getModel(modelID).getNPoints();j++)
            {
                if (Session::getInstance().getModel(modelID).getPoint(j).getMaterial().getName() == citer.key())
                {
                    Session::getInstance().getModel(modelID).getPoint(j).setMaterial(citer.value());
                }
            }
            for (uint j=0;j<Session::getInstance().getModel(modelID).getNLines();j++)
            {
                if (Session::getInstance().getModel(modelID).getLine(j).getMaterial().getName() == citer.key())
                {
                    Session::getInstance().getModel(modelID).getLine(j).setMaterial(citer.value());
                }
            }
            for (uint j=0;j<Session::getInstance().getModel(modelID).getNSurfaces();j++)
            {
                if (Session::getInstance().getModel(modelID).getSurface(j).getMaterial().getName() == citer.key())
                {
                    Session::getInstance().getModel(modelID).getSurface(j).setMaterial(citer.value());
                }
            }
            for (uint j=0;j<Session::getInstance().getModel(modelID).getNVolumes();j++)
            {
                if (Session::getInstance().getModel(modelID).getVolume(j).getMaterial().getName() == citer.key())
                {
                    Session::getInstance().getModel(modelID).getVolume(j).setMaterial(citer.value());
                }
            }
        }
    }
    else
    {
        QMap<QString,RMaterial>::const_iterator iter;
        for (iter = differentMaterials.constBegin(); iter != differentMaterials.constEnd(); ++iter)
        {
            // Remove unknown material from the model's element groups.
            for (uint j=0;j<Session::getInstance().getModel(modelID).getNPoints();j++)
            {
                if (Session::getInstance().getModel(modelID).getPoint(j).getMaterial().getName() == iter.key())
                {
                    Session::getInstance().getModel(modelID).getPoint(j).setMaterial(RMaterial());
                }
            }
            for (uint j=0;j<Session::getInstance().getModel(modelID).getNLines();j++)
            {
                if (Session::getInstance().getModel(modelID).getLine(j).getMaterial().getName() == iter.key())
                {
                    Session::getInstance().getModel(modelID).getLine(j).setMaterial(RMaterial());
                }
            }
            for (uint j=0;j<Session::getInstance().getModel(modelID).getNSurfaces();j++)
            {
                if (Session::getInstance().getModel(modelID).getSurface(j).getMaterial().getName() == iter.key())
                {
                    Session::getInstance().getModel(modelID).getSurface(j).setMaterial(RMaterial());
                }
            }
            for (uint j=0;j<Session::getInstance().getModel(modelID).getNVolumes();j++)
            {
                if (Session::getInstance().getModel(modelID).getVolume(j).getMaterial().getName() == iter.key())
                {
                    Session::getInstance().getModel(modelID).getVolume(j).setMaterial(RMaterial());
                }
            }
        }
    }
}

void MaterialManagerTree::onProblemChanged(uint)
{
    this->updateProblemTypeMask();
}

void MaterialManagerTree::onMaterialAdded(const QString &materialName, bool isNew)
{
    this->addMaterial(materialName, isNew);
    this->updateCheckStates();
}

void MaterialManagerTree::onMaterialRemoved(const QString &, uint removedID)
{
    for (int i=this->topLevelItemCount()-1;i>=0;i--)
    {
        if (this->topLevelItem(i)->data(MATERIAL_TREE_COLUMN_NAME,Qt::UserRole).toUInt() == removedID)
        {
            this->takeTopLevelItem(i);
        }
    }

    // Loop over all element groups and delete the material.
    for (uint i=0;i<Session::getInstance().getNModels();i++)
    {
        for (uint j=0;j<Session::getInstance().getModel(i).getNPoints();j++)
        {
            if (Session::getInstance().getModel(i).getPoint(j).getMaterial().getID() == removedID)
            {
                Session::getInstance().getModel(i).getPoint(j).setMaterial(RMaterial());
            }
        }
        for (uint j=0;j<Session::getInstance().getModel(i).getNLines();j++)
        {
            if (Session::getInstance().getModel(i).getLine(j).getMaterial().getID() == removedID)
            {
                Session::getInstance().getModel(i).getLine(j).setMaterial(RMaterial());
            }
        }
        for (uint j=0;j<Session::getInstance().getModel(i).getNSurfaces();j++)
        {
            if (Session::getInstance().getModel(i).getSurface(j).getMaterial().getID() == removedID)
            {
                Session::getInstance().getModel(i).getSurface(j).setMaterial(RMaterial());
            }
        }
        for (uint j=0;j<Session::getInstance().getModel(i).getNVolumes();j++)
        {
            if (Session::getInstance().getModel(i).getVolume(j).getMaterial().getID() == removedID)
            {
                Session::getInstance().getModel(i).getVolume(j).setMaterial(RMaterial());
            }
        }
    }
}

void MaterialManagerTree::onMaterialRenamed(const QString &materialOldName, const QString &materialNewName)
{
    // Loop over all element groups and rename applied material.
    for (uint i=0;i<Session::getInstance().getNModels();i++)
    {
        for (uint j=0;j<Session::getInstance().getModel(i).getNPoints();j++)
        {
            if (Session::getInstance().getModel(i).getPoint(j).getMaterial().getName() == materialOldName)
            {
                Session::getInstance().getModel(i).getPoint(j).getMaterial().setName(materialNewName);
            }
        }
        for (uint j=0;j<Session::getInstance().getModel(i).getNLines();j++)
        {
            if (Session::getInstance().getModel(i).getLine(j).getMaterial().getName() == materialOldName)
            {
                Session::getInstance().getModel(i).getLine(j).getMaterial().setName(materialNewName);
            }
        }
        for (uint j=0;j<Session::getInstance().getModel(i).getNSurfaces();j++)
        {
            if (Session::getInstance().getModel(i).getSurface(j).getMaterial().getName() == materialOldName)
            {
                Session::getInstance().getModel(i).getSurface(j).getMaterial().setName(materialNewName);
            }
        }
        for (uint j=0;j<Session::getInstance().getModel(i).getNVolumes();j++)
        {
            if (Session::getInstance().getModel(i).getVolume(j).getMaterial().getName() == materialOldName)
            {
                Session::getInstance().getModel(i).getVolume(j).getMaterial().setName(materialNewName);
            }
        }
    }
}

void MaterialManagerTree::onMaterialChanged(const QString &materialName)
{
    // Loop over all element groups and set changed material.

    RMaterial material = MaterialList::getInstance().get(materialName);

    for (uint i=0;i<Session::getInstance().getNModels();i++)
    {
        for (uint j=0;j<Session::getInstance().getModel(i).getNPoints();j++)
        {
            if (Session::getInstance().getModel(i).getPoint(j).getMaterial().getName() == materialName)
            {
                Session::getInstance().getModel(i).getPoint(j).setMaterial(material);
            }
        }
        for (uint j=0;j<Session::getInstance().getModel(i).getNLines();j++)
        {
            if (Session::getInstance().getModel(i).getLine(j).getMaterial().getName() == materialName)
            {
                Session::getInstance().getModel(i).getLine(j).setMaterial(material);
            }
        }
        for (uint j=0;j<Session::getInstance().getModel(i).getNSurfaces();j++)
        {
            if (Session::getInstance().getModel(i).getSurface(j).getMaterial().getName() == materialName)
            {
                Session::getInstance().getModel(i).getSurface(j).setMaterial(material);
            }
        }
        for (uint j=0;j<Session::getInstance().getModel(i).getNVolumes();j++)
        {
            if (Session::getInstance().getModel(i).getVolume(j).getMaterial().getName() == materialName)
            {
                Session::getInstance().getModel(i).getVolume(j).setMaterial(material);
            }
        }
    }

    this->updateProblemTypeMask();
}

void MaterialManagerTree::onItemChanged(QTreeWidgetItem *item, int)
{
    uint id = item->data(MATERIAL_TREE_COLUMN_NAME,Qt::UserRole).toUInt();
    QString oldName = MaterialList::getInstance().findID(id);
    QString newName = item->text(MATERIAL_TREE_COLUMN_NAME);

    if (oldName != newName)
    {
        if (MaterialList::getInstance().containName(newName))
        {
            RLogger::warning("Material \'%s\' can not be renamed to \'%s\', because there already is a material with such name.\n",
                             oldName.toUtf8().constData(),newName.toUtf8().constData());
            item->setText(MATERIAL_TREE_COLUMN_NAME,oldName);
        }
        else
        {
            if (!MaterialList::getInstance().rename(oldName,newName))
            {
                RLogger::error("Failed to rename material from \'%s\' to \'%s\'\n",oldName.toUtf8().constData(),newName.toUtf8().constData());
                this->blockSignals(true);
                item->setText(MATERIAL_TREE_COLUMN_NAME,oldName);
                this->blockSignals(false);
            }
            emit this->materialSelected(newName);
        }
    }
    else
    {
        Qt::CheckState checkState = item->checkState(MATERIAL_TREE_COLUMN_NAME);
        RMaterial material;
        if (checkState == Qt::Checked)
        {
            material = MaterialList::getInstance().get(item->text(MATERIAL_TREE_COLUMN_NAME));
        }

        QList<SessionEntityID> selectedEntityIDs = Session::getInstance().getSelectedEntityIDs();

        for (int i=0;i<selectedEntityIDs.size();i++)
        {
            switch (selectedEntityIDs[i].getType())
            {
                case R_ENTITY_GROUP_POINT:
                    Session::getInstance().getModel(selectedEntityIDs[i].getMid()).getPoint(selectedEntityIDs[i].getEid()).setMaterial(material);
                    break;
                case R_ENTITY_GROUP_LINE:
                    Session::getInstance().getModel(selectedEntityIDs[i].getMid()).getLine(selectedEntityIDs[i].getEid()).setMaterial(material);
                    break;
                case R_ENTITY_GROUP_SURFACE:
                    Session::getInstance().getModel(selectedEntityIDs[i].getMid()).getSurface(selectedEntityIDs[i].getEid()).setMaterial(material);
                    break;
                case R_ENTITY_GROUP_VOLUME:
                    Session::getInstance().getModel(selectedEntityIDs[i].getMid()).getVolume(selectedEntityIDs[i].getEid()).setMaterial(material);
                    break;
                default:
                    break;
            }
        }
    }
    this->updateCheckStates();
}

void MaterialManagerTree::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    this->blockSignals(true);
    if (column == MATERIAL_TREE_COLUMN_NAME)
    {
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
    else
    {
        if (item->flags() & Qt::ItemIsEditable)
        {
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        }
    }
    this->blockSignals(false);
}

void MaterialManagerTree::onItemSelectionChanged(void)
{
    QList<QTreeWidgetItem *> items = this->selectedItems();

    if (items.size() == 0)
    {
        MaterialList::getInstance().selectMaterial(QString());
        emit this->materialSelected(QString());
    }

    for (int i=0;i<items.size();i++)
    {
        QString materialName = items[i]->text(MATERIAL_TREE_COLUMN_NAME);

        MaterialList::getInstance().selectMaterial(materialName);

        emit this->materialSelected(materialName);
    }

    this->actionList->processAvailability();
}

void MaterialManagerTree::onMaterialSelected(void)
{
    this->doNotPopulate = true;
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();
    for (int i=0;i<modelIDs.size();i++)
    {
        Session::getInstance().setProblemChanged(modelIDs[i]);
    }
    this->doNotPopulate = false;
}
