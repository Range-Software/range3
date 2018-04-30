/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   scalar_field_dialog.cpp                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   14-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Scalar field dialog class definition         *
 *********************************************************************/

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

#include "session.h"
#include "scalar_field_dialog.h"

ScalarFieldDialog::ScalarFieldDialog(uint modelID, QWidget *parent) :
    QDialog(parent),
    modelID(modelID),
    entityID(RConstants::eod)
{
    this->createDialog();
}

ScalarFieldDialog::ScalarFieldDialog(uint modelID, uint entityID, QWidget *parent) :
    QDialog(parent),
    modelID(modelID),
    entityID(entityID)
{
    this->createDialog();
}

int ScalarFieldDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        RVariableType varType = this->getVariableType();
        QList<SessionEntityID> entities = this->modelTree->getSelected();

        if (varType != R_VARIABLE_NONE && entities.size() > 0)
        {
            Session::getInstance().storeCurentModelVersion(this->modelID,tr("Create scalar field"));

            if (this->entityID != RConstants::eod)
            {
                RScalarField &scalarField = Session::getInstance().getModel(this->modelID).getScalarField(this->entityID);

                scalarField.clearElementGroupIDs();

                scalarField.setVariableType(varType);

                for (int i=0;i<entities.size();i++)
                {
                    uint elementGroupId = Session::getInstance().getModel(this->modelID).getEntityGroupID(entities[i].getType(),
                                                                                                          entities[i].getEid());
                    if (elementGroupId == RConstants::eod)
                    {
                        continue;
                    }
                    scalarField.addElementGroupID(elementGroupId);
                }
                scalarField.setMaxPointSize(double(this->pointSizeSpin->value()));

                RLogger::info("Modified Scalar field \'%s\'\n",scalarField.getName().toUtf8().constData());
            }
            else
            {
                RScalarField scalarField;

                scalarField.setName(RVariable::getName(varType));
                scalarField.setVariableType(varType);

                for (int i=0;i<entities.size();i++)
                {
                    uint elementGroupId = Session::getInstance().getModel(this->modelID).getElementGroupID(entities[i].getType(),
                                                                                                           entities[i].getEid());
                    scalarField.addElementGroupID(elementGroupId);
                }
                scalarField.setMaxPointSize(double(this->pointSizeSpin->value()));

                Session::getInstance().getModel(this->modelID).addScalarField(scalarField);
                RLogger::info("Created new Scalar field \'%s\'\n",scalarField.getName().toUtf8().constData());
            }

            Session::getInstance().setModelChanged(this->modelID);
        }
    }

    return retVal;
}

void ScalarFieldDialog::createDialog(void)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->resize(500,400);

    QString windowTitleStr = tr("Scalar field editor");
    this->setWindowTitle(tr(windowTitleStr.toUtf8().constData()));

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);


    QLabel *titleLabel = new QLabel;
    if (this->entityID == RConstants::eod)
    {
        titleLabel->setText(tr("Create new Scalar field."));
    }
    else
    {
        titleLabel->setText(tr("Modify Scalar field:") + " <b>" + Session::getInstance().getModel(modelID).getScalarField(entityID).getName() + "</b>");
    }

    uint rowCount = 0;

    mainLayout->addWidget(titleLabel, rowCount, 0, 1, 1);

    rowCount++;

    REntityGroupTypeMask typeMask = R_ENTITY_GROUP_POINT
                                  | R_ENTITY_GROUP_LINE
                                  | R_ENTITY_GROUP_SURFACE
                                  | R_ENTITY_GROUP_VOLUME
                                  | R_ENTITY_GROUP_CUT
                                  | R_ENTITY_GROUP_ISO
                                  | R_ENTITY_GROUP_STREAM_LINE;

    this->modelTree = new ModelTreeSimple(this->modelID,typeMask,this);
    if (this->entityID != RConstants::eod)
    {
        Model &rModel = Session::getInstance().getModel(modelID);
        RScalarField &rScalarField = rModel.getScalarField(entityID);
        const std::vector<unsigned int> groupIDs = rScalarField.getElementGroupIDs();
        this->modelTree->clearSelection();
        for (uint i=0;i<groupIDs.size();i++)
        {
            REntityGroupType entityGroupType;
            uint entityIDNum;
            if (rModel.getEntityID(groupIDs[i],entityGroupType,entityIDNum))
            {
                this->modelTree->selectEntity(this->modelID,entityGroupType,entityIDNum);
            }
        }
    }

    mainLayout->addWidget(this->modelTree, rowCount, 0, 1, 1);

    QObject::connect(this->modelTree,
                     &QTreeWidget::itemSelectionChanged,
                     this,
                     &ScalarFieldDialog::onModelTreeSelectionChanged);

    this->variableTree = new QTreeWidget(this);
    this->variableTree->setSelectionMode(QAbstractItemView::SingleSelection);
    this->variableTree->setColumnCount(ScalarFieldDialog::NColumns);

    QTreeWidgetItem* headerItem = new QTreeWidgetItem();
    headerItem->setText(ScalarFieldDialog::Name,QString("Variables"));
    headerItem->setText(ScalarFieldDialog::Type,QString("type"));
    this->variableTree->setHeaderItem(headerItem);
    this->variableTree->setColumnHidden(ScalarFieldDialog::Type,true);

    Model &rModel = Session::getInstance().getModel(this->modelID);

    for (uint i=0;i<rModel.getNVariables();i++)
    {
        RVariable &rVariable = rModel.getVariable(i);

        QTreeWidgetItem *itemVariable = new QTreeWidgetItem(this->variableTree);
        itemVariable->setText(ScalarFieldDialog::Name, rVariable.getName());
        itemVariable->setData(ScalarFieldDialog::Type,Qt::DisplayRole,QVariant(rVariable.getType()));
        if (this->entityID != RConstants::eod)
        {
            RScalarField &rScalarField = Session::getInstance().getModel(modelID).getScalarField(entityID);
            itemVariable->setSelected(rVariable.getType() == rScalarField.getVariableType());
        }
    }

    mainLayout->addWidget(this->variableTree, rowCount, 1, 1, 1);

    rowCount++;

    QObject::connect(this->variableTree,
                     &QTreeWidget::itemSelectionChanged,
                     this,
                     &ScalarFieldDialog::onVariableTreeSelectionChanged);

    QHBoxLayout *pointSizeLayout = new QHBoxLayout;
    mainLayout->addLayout(pointSizeLayout, rowCount, 0, 1, 2);

    QLabel *pointSizeLabel = new QLabel(tr("Point size in pixels:"));
    pointSizeLayout->addWidget(pointSizeLabel);

    this->pointSizeSpin = new QSpinBox;
//    this->pointSizeSpin->set
    this->pointSizeSpin->setMinimum(1);
    this->pointSizeSpin->setMaximum(100);
    if (this->entityID != RConstants::eod)
    {
        RScalarField &rScalarField = Session::getInstance().getModel(modelID).getScalarField(entityID);
        this->pointSizeSpin->setValue(rScalarField.getMaxPointSize());
    }
    else
    {
        this->pointSizeSpin->setValue(5);
    }
    pointSizeLayout->addWidget(this->pointSizeSpin);

    rowCount++;

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, rowCount, 0, 1, 2);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    this->okButton = new QPushButton(okIcon, tr("Ok"));
    this->okButton->setEnabled(this->variableTree->selectedItems().size() && this->modelTree->selectedItems().size());
    this->okButton->setDefault(true);
    buttonsLayout->addWidget(this->okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&ScalarFieldDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&ScalarFieldDialog::accept);
}

RVariableType ScalarFieldDialog::getVariableType(void) const
{
    QList<QTreeWidgetItem*> items = this->variableTree->selectedItems();

    if (items.size() == 0)
    {
        return R_VARIABLE_NONE;
    }

    return RVariableType(items[0]->data(ScalarFieldDialog::Type,Qt::DisplayRole).toInt());
}

void ScalarFieldDialog::onVariableTreeSelectionChanged(void)
{
    this->okButton->setEnabled(this->variableTree->selectedItems().size() && this->modelTree->selectedItems().size());
}

void ScalarFieldDialog::onModelTreeSelectionChanged()
{
    this->okButton->setEnabled(this->variableTree->selectedItems().size() && this->modelTree->selectedItems().size());
}
