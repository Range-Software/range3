/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   bc_tree.cpp                                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   9-th February 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Boundary condition tree class definition            *
 *********************************************************************/

#include <QStandardItemModel>
#include <QLineEdit>
#include <QPushButton>

#include "component_edit_dialog.h"
#include "session.h"
#include "bc_tree.h"
#include "variable_value_edit.h"
#include "push_button.h"
#include "main_window.h"

typedef enum _BCTreeColumn
{
    BC_TREE_PROPERTY_NAME = 0,
    BC_TREE_PROPERTY_VALUE,
    BC_TREE_PROPERTY_UNITS,
    BC_TREE_PROPERTY_TYPE,
    BC_TREE_N_COLUMNS
} BCTreeColumn;

BCTree::BCTree(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setRootIsDecorated(true);
    this->setSelectionMode(QAbstractItemView::NoSelection);

    this->setColumnCount(BC_TREE_N_COLUMNS);

    QTreeWidgetItem* headerItem = new QTreeWidgetItem();
    headerItem->setText(BC_TREE_PROPERTY_TYPE,QString("Property type"));
    headerItem->setText(BC_TREE_PROPERTY_NAME,QString("Property name"));
    headerItem->setText(BC_TREE_PROPERTY_VALUE,QString("Value"));
    headerItem->setText(BC_TREE_PROPERTY_UNITS,QString("Units"));
    this->setHeaderItem(headerItem);

    this->setColumnHidden(BC_TREE_PROPERTY_TYPE,true);

    this->populate();
}

void BCTree::populate(void)
{
    this->clear();

    for (uint i=0;i<this->bc.size();i++)
    {
        RConditionComponent component = this->bc.getComponent(i);

        QTreeWidgetItem *item = new QTreeWidgetItem(this);
        item->setData(BC_TREE_PROPERTY_TYPE,Qt::DisplayRole,QVariant(component.getType()));
        item->setText(BC_TREE_PROPERTY_NAME,component.getName());
        item->setText(BC_TREE_PROPERTY_UNITS,component.getUnits());
        VariableValueEdit *lineEdit = new VariableValueEdit(component.getType());
        lineEdit->setValue(component.getValue(0));
        this->setItemWidget(item,BC_TREE_PROPERTY_VALUE,lineEdit);
        if (component.size() > 1)
        {
            lineEdit->setDisabled(true);
        }

        QTreeWidgetItem *child = new QTreeWidgetItem(item);
        child->setFirstColumnSpanned(true);
        PushButton *buttonValues = new PushButton(int(component.getType()),QString("Edit time dependent values"));
        this->setItemWidget(child,BC_TREE_PROPERTY_NAME,buttonValues);

        QObject::connect(lineEdit,&VariableValueEdit::valueChanged,this,&BCTree::onBcValueChanged);
        QObject::connect(buttonValues,&PushButton::clicked,this,&BCTree::onButtonValueClicked);
    }

    this->resizeColumnToContents(BC_TREE_PROPERTY_NAME);
    this->resizeColumnToContents(BC_TREE_PROPERTY_VALUE);
    this->resizeColumnToContents(BC_TREE_PROPERTY_UNITS);
}

void BCTree::updateSelectedEntities(void) const
{
    QList<SessionEntityID> entityIDs = Session::getInstance().getSelectedEntityIDs();

    for (int i=0;i<entityIDs.size();i++)
    {
        switch (entityIDs[i].getType())
        {
            case R_ENTITY_GROUP_POINT:
                Session::getInstance().getModel(entityIDs[i].getMid()).getPoint(entityIDs[i].getEid()).addBoundaryCondition(this->bc);
                break;
            case R_ENTITY_GROUP_LINE:
                Session::getInstance().getModel(entityIDs[i].getMid()).getLine(entityIDs[i].getEid()).addBoundaryCondition(this->bc);
                break;
            case R_ENTITY_GROUP_SURFACE:
                Session::getInstance().getModel(entityIDs[i].getMid()).getSurface(entityIDs[i].getEid()).addBoundaryCondition(this->bc);
                break;
            case R_ENTITY_GROUP_VOLUME:
                Session::getInstance().getModel(entityIDs[i].getMid()).getVolume(entityIDs[i].getEid()).addBoundaryCondition(this->bc);
                break;
            default:
                break;
        }
        Session::getInstance().setBoundaryConditionChanged(entityIDs[i].getMid(),
                                                           entityIDs[i].getType(),
                                                           entityIDs[i].getEid());
    }

    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();
    for (int i=0;i<modelIDs.size();i++)
    {
        Session::getInstance().setProblemChanged(modelIDs[i]);
    }
}

void BCTree::onBcSelected(RBoundaryConditionType bcType, bool applied)
{
    if (applied)
    {
        QList<SessionEntityID> entityIDs = Session::getInstance().getSelectedEntityIDs();

        for (int i=0;i<entityIDs.size();i++)
        {
            switch (entityIDs[i].getType())
            {
                case R_ENTITY_GROUP_POINT:
                    this->bc = Session::getInstance().getModel(entityIDs[i].getMid()).getPoint(entityIDs[i].getEid()).getBoundaryCondition(bcType);
                    break;
                case R_ENTITY_GROUP_LINE:
                    this->bc = Session::getInstance().getModel(entityIDs[i].getMid()).getLine(entityIDs[i].getEid()).getBoundaryCondition(bcType);
                    break;
                case R_ENTITY_GROUP_SURFACE:
                    this->bc = Session::getInstance().getModel(entityIDs[i].getMid()).getSurface(entityIDs[i].getEid()).getBoundaryCondition(bcType);
                    break;
                case R_ENTITY_GROUP_VOLUME:
                    this->bc = Session::getInstance().getModel(entityIDs[i].getMid()).getVolume(entityIDs[i].getEid()).getBoundaryCondition(bcType);
                    break;
                default:
                    this->bc.setType(bcType);
                    break;
            }
        }

        this->populate();
    }
    else
    {
        this->bc.setType(R_BOUNDARY_CONDITION_NONE);
        this->clear();
    }
}

void BCTree::onBcValueChanged(RVariableType variableType, double value)
{
    for (uint i=0;i<this->bc.size();i++)
    {
        if (this->bc.getComponent(i).getType() == variableType)
        {
            this->bc.getComponent(i).add(0,value);
        }
    }

    this->updateSelectedEntities();
}

void BCTree::onButtonValueClicked(int id)
{
    unsigned int cPosition = this->bc.findComponentPosition(RVariableType(id));
    if (cPosition == RConstants::eod)
    {
        return;
    }
    ComponentEditDialog componentEditDialog(this->bc.getComponent(cPosition),MainWindow::getInstance());
    if (componentEditDialog.exec() == QDialog::Accepted)
    {
        this->updateSelectedEntities();
        for (int i=0;i<this->topLevelItemCount();i++)
        {
            RVariableType variableType = RVariableType(this->topLevelItem(i)->data(BC_TREE_PROPERTY_TYPE,Qt::DisplayRole).toInt());
            if (variableType != RVariableType(id))
            {
                continue;
            }
            VariableValueEdit *lineEdit = (VariableValueEdit*)this->itemWidget(this->topLevelItem(i),BC_TREE_PROPERTY_VALUE);
            lineEdit->setValue(this->bc.getComponent(cPosition).getValue(0));
            lineEdit->setEnabled(this->bc.getComponent(cPosition).size() == 1);
        }
    }
}
