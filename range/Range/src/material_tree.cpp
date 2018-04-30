/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_tree.cpp                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   13-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Material tree class definition                      *
 *********************************************************************/


#include "material_list.h"
#include "material_tree.h"
#include "material_property_add_dialog.h"
#include "material_property_line_edit.h"
#include "material_property_edit_dialog.h"
#include "push_button.h"

typedef enum _MaterialTreeColumn
{
    MATERIAL_TREE_PROPERTY_NAME = 0,
    MATERIAL_TREE_PROPERTY_VALUE,
    MATERIAL_TREE_PROPERTY_UNITS,
    MATERIAL_TREE_PROPERTY_TYPE,
    MATERIAL_TREE_N_COLUMNS
} MaterialTreeColumn;

MaterialTree::MaterialTree(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setRootIsDecorated(true);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSortingEnabled(true);
    this->sortByColumn(MATERIAL_TREE_PROPERTY_NAME,Qt::AscendingOrder);

    this->setColumnCount(MATERIAL_TREE_N_COLUMNS);

    QTreeWidgetItem* headerItem = new QTreeWidgetItem();
    headerItem->setText(MATERIAL_TREE_PROPERTY_TYPE,QString("Property type"));
    headerItem->setText(MATERIAL_TREE_PROPERTY_NAME,QString("Property name"));
    headerItem->setText(MATERIAL_TREE_PROPERTY_VALUE,QString("Value"));
    headerItem->setText(MATERIAL_TREE_PROPERTY_UNITS,QString("Units"));
    this->setHeaderItem(headerItem);

    this->setColumnHidden(MATERIAL_TREE_PROPERTY_TYPE,true);

    this->setContextMenuPolicy(Qt::ActionsContextMenu);

    this->actionAddProperty = new QAction(this);
    this->actionAddProperty->setText(tr("Add new property"));
    this->actionAddProperty->setIcon(QIcon(":/icons/file/pixmaps/range-new.svg"));
    this->addAction(this->actionAddProperty);

    this->actionRemoveProperty = new QAction(this);
    this->actionRemoveProperty->setText(tr("Remove property"));
    this->actionRemoveProperty->setIcon(QIcon(":/icons/file/pixmaps/range-close.svg"));
    this->addAction(this->actionRemoveProperty);

    QObject::connect(this->actionAddProperty,&QAction::triggered,this,&MaterialTree::onPropertyAdd);
    QObject::connect(this->actionRemoveProperty,&QAction::triggered,this,&MaterialTree::onPropertyDelete);

    this->populate();

    QObject::connect(this,&MaterialTree::currentItemChanged,this,&MaterialTree::onCurrentItemChanged);
}

void MaterialTree::populate(void)
{
    this->blockSignals(true);

    this->clear();
    this->actionAddProperty->setDisabled(true);
    this->actionRemoveProperty->setDisabled(true);

    if (this->materialName.length() == 0)
    {
        return;
    }
    this->actionAddProperty->setEnabled(true);

    RMaterial material = MaterialList::getInstance().get(this->materialName);

    for (uint i=0;i<material.size();i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(this);
        item->setData(MATERIAL_TREE_PROPERTY_TYPE,Qt::DisplayRole,QVariant(material.get(i).getType()));
        item->setText(MATERIAL_TREE_PROPERTY_NAME,material.get(i).getName());
        item->setText(MATERIAL_TREE_PROPERTY_UNITS,material.get(i).getUnits());

        MaterialPropertyLineEdit *lineEdit = new MaterialPropertyLineEdit(material.get(i).getType());
        lineEdit->setValue(material.get(i).get(RVariable::getInitValue(material.get(i).getKeyType())));
        this->setItemWidget(item,MATERIAL_TREE_PROPERTY_VALUE,lineEdit);
        if (material.get(i).size() > 1)
        {
            lineEdit->setDisabled(true);
        }

        QTreeWidgetItem *child = new QTreeWidgetItem(item);
        child->setFirstColumnSpanned(true);
        child->setData(MATERIAL_TREE_PROPERTY_TYPE,Qt::DisplayRole,QVariant(material.get(i).getType()));
        PushButton *buttonValues = new PushButton(int(material.get(i).getType()),QString("Edit temperature dependent values"));
        this->setItemWidget(child,MATERIAL_TREE_PROPERTY_NAME,buttonValues);

        QObject::connect(lineEdit,&MaterialPropertyLineEdit::valueChanged,
                         this,&MaterialTree::onValueChanged);
        QObject::connect(buttonValues,&PushButton::clicked,this,&MaterialTree::onButtonValueClicked);
    }

    this->blockSignals(false);
}

void MaterialTree::onMaterialSelected(const QString &materialName)
{
    this->materialName = materialName;
    this->populate();
}

void MaterialTree::onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    if (current)
    {
        this->actionRemoveProperty->setEnabled(true);
    }
    else
    {
        this->actionRemoveProperty->setDisabled(true);
    }
    RMaterial material = MaterialList::getInstance().get(this->materialName);
    if (material.size()+2 < R_MATERIAL_PROPERTY_N_TYPES)
    {
        this->actionAddProperty->setEnabled(true);
    }
    else
    {
        this->actionAddProperty->setDisabled(true);
    }
}

void MaterialTree::onValueChanged(RMaterialPropertyType type, double value)
{
    RMaterial material = MaterialList::getInstance().get(this->materialName);
    uint propertyPosition = material.findPosition(type);
    material.get(propertyPosition).add(RVariable::getInitValue(material.get(propertyPosition).getKeyType()),value);
    MaterialList::getInstance().add(material);
}

void MaterialTree::onButtonValueClicked(int id)
{
    RMaterial material = MaterialList::getInstance().get(this->materialName);
    uint propertyPosition = material.findPosition(RMaterialPropertyType(id));

    MaterialPropertyEditDialog materialPropertyEditDialog(material.get(propertyPosition),this);

    if (materialPropertyEditDialog.exec() == QDialog::Accepted)
    {
//        for (int i=0;i<this->topLevelItemCount();i++)
//        {
//            RMaterialPropertyType type = RMaterialPropertyType(this->topLevelItem(i)->data(MATERIAL_TREE_PROPERTY_TYPE,Qt::DisplayRole).toInt());
//            if (type != RMaterialPropertyType(id))
//            {
//                continue;
//            }
//            MaterialPropertyLineEdit *lineEdit = (MaterialPropertyLineEdit*)this->itemWidget(this->topLevelItem(i),MATERIAL_TREE_PROPERTY_VALUE);
//            lineEdit->setValue(material.get(propertyPosition).get(RVariable::getInitValue(material.get(propertyPosition).getKeyType())));
//            lineEdit->setEnabled(material.get(propertyPosition).size() == 1);
//        }
        MaterialList::getInstance().blockSignals(true);
        MaterialList::getInstance().add(material);
        MaterialList::getInstance().blockSignals(false);
        this->populate();
    }
}

void MaterialTree::onPropertyAdd(void)
{
    RMaterial material = MaterialList::getInstance().get(this->materialName);
    MaterialPropertyAddDialog addPropertyDialog(material,this);
    if (addPropertyDialog.exec() == QDialog::Accepted)
    {
        MaterialList::getInstance().add(material);
        this->populate();
    }
}

void MaterialTree::onPropertyDelete(void)
{
    RMaterial material = MaterialList::getInstance().get(this->materialName);

    QList<QTreeWidgetItem*> items = this->selectedItems();

    for (int i=0;i<items.size();i++)
    {
        RMaterialPropertyType type = RMaterialPropertyType(items[i]->data(MATERIAL_TREE_PROPERTY_TYPE,Qt::DisplayRole).toInt());

        uint propertyPosition = material.findPosition(type);
        material.remove(propertyPosition);
    }
    MaterialList::getInstance().add(material);
    this->populate();
}
