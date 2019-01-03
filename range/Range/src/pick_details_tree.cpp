/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   pick_details_tree.cpp                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   17-th June 2013                                          *
 *                                                                   *
 *  DESCRIPTION: Pick details tree class definition                  *
 *********************************************************************/

#include <QHBoxLayout>

#include "pick_details_tree.h"
#include "session.h"

typedef enum _PickDetailsTreeColumn
{
    PICK_DETAILS_TREE_COLUMN_1 = 0,
    PICK_DETAILS_TREE_COLUMN_2,
    PICK_DETAILS_TREE_COLUMN_3,
    PICK_DETAILS_TREE_COLUMN_4,
    PICK_DETAILS_TREE_COLUMN_5,
    PICK_DETAILS_TREE_N_COLUMNS
} PickDetailsTreeColumn;

PickDetailsTree::PickDetailsTree(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setColumnCount(PICK_DETAILS_TREE_N_COLUMNS);
    this->setHeaderHidden(true);

    this->populate();

    QObject::connect(&Session::getInstance(),
                     &Session::pickListChanged,
                     this,
                     &PickDetailsTree::onPickListChanged);
    QObject::connect(&Session::getInstance(),
                     &Session::modelAdded,
                     this,
                     &PickDetailsTree::onPickListChanged);
    QObject::connect(&Session::getInstance(),
                     &Session::modelRemoved,
                     this,
                     &PickDetailsTree::onPickListChanged);
    QObject::connect(&Session::getInstance(),
                     &Session::modelChanged,
                     this,
                     &PickDetailsTree::onPickListChanged);
    QObject::connect(this,
                     &PickDetailsTree::itemExpanded,
                     this,
                     &PickDetailsTree::onItemExpanded);
}

void PickDetailsTree::populate(void)
{
    this->clear();

    const QVector<PickItem> &rPickItems = Session::getInstance().getPickList().getItems();

    for (int i=0;i<rPickItems.size();i++)
    {
        QTreeWidgetItem *topItem = new QTreeWidgetItem(this);
        QString itemText;

        const Model &rModel = Session::getInstance().getModel(rPickItems[i].getEntityID().getMid());

        const SessionEntityID &rEntityID = rPickItems[i].getEntityID();
        uint elementID = rPickItems[i].getElementID();
        uint nodeID = rPickItems[i].getNodeID();

        switch (rPickItems[i].getItemType())
        {
            case PICK_ITEM_ELEMENT:
            {
                QTreeWidgetItem *childItem = nullptr;

                // Type
                childItem = new QTreeWidgetItem(topItem);
                childItem->setText(PICK_DETAILS_TREE_COLUMN_1,tr("Family:"));
                childItem->setText(PICK_DETAILS_TREE_COLUMN_2,REntityGroup::getTypeName(rEntityID.getType()));

                // Nodes
                if (REntityGroup::typeIsElementGroup(rEntityID.getType()))
                {
                    itemText = tr("Element") + " # " + QString::number(elementID);
                    const RElement &rElement = rModel.getElement(elementID);
                    for (uint j=0;j<rElement.size();j++)
                    {
                        const RNode &rNode = rModel.getNode(rElement.getNodeId(j));

                        childItem = new QTreeWidgetItem(topItem);
                        childItem->setText(PICK_DETAILS_TREE_COLUMN_1,tr("Node ") + QString::number(j+1) + ":");
                        childItem->setText(PICK_DETAILS_TREE_COLUMN_2,QString::number(rNode.getX()));
                        childItem->setText(PICK_DETAILS_TREE_COLUMN_3,QString::number(rNode.getY()));
                        childItem->setText(PICK_DETAILS_TREE_COLUMN_4,QString::number(rNode.getZ()));
                        childItem->setText(PICK_DETAILS_TREE_COLUMN_5,"[m]");
                    }

                    std::vector<RElement> edgeElements = rElement.generateEdgeElements();

                    for (uint j=0;j<edgeElements.size();j++)
                    {
                        double edgeLength = 0.0;

                        if (edgeElements[j].findLength(rModel.getNodes(),edgeLength))
                        {
                            childItem = new QTreeWidgetItem(topItem);
                            childItem->setText(PICK_DETAILS_TREE_COLUMN_1,tr("Edge ") + QString::number(edgeElements[j].getNodeId(0)) + "-" + QString::number(edgeElements[j].getNodeId(1)) + ":");
                            childItem->setText(PICK_DETAILS_TREE_COLUMN_2,QString::number(edgeLength));
                            childItem->setText(PICK_DETAILS_TREE_COLUMN_5,"[m]");
                        }
                    }
                }
                else
                {
                    itemText = tr("Interpolated element") + " # " + QString::number(rPickItems[i].getElementPosition());
                    const RInterpolatedElement *pIElement = nullptr;
                    switch (rEntityID.getType())
                    {
                        case R_ENTITY_GROUP_CUT:
                            pIElement = &rModel.getCut(rEntityID.getEid()).at(rPickItems[i].getElementPosition());
                            break;
                        case R_ENTITY_GROUP_ISO:
                            pIElement = &rModel.getIso(rEntityID.getEid()).at(rPickItems[i].getElementPosition());
                            break;
                        default:
                            break;
                    }
                    if (!pIElement)
                    {
                        continue;
                    }
                    for (uint j=0;j<pIElement->size();j++)
                    {
                        const RInterpolatedNode &rINode = pIElement->at(j);

                        childItem = new QTreeWidgetItem(topItem);
                        childItem->setText(PICK_DETAILS_TREE_COLUMN_1,tr("Node ") + QString::number(j+1) + ":");
                        childItem->setText(PICK_DETAILS_TREE_COLUMN_2,QString::number(rINode.getX()));
                        childItem->setText(PICK_DETAILS_TREE_COLUMN_3,QString::number(rINode.getY()));
                        childItem->setText(PICK_DETAILS_TREE_COLUMN_4,QString::number(rINode.getZ()));
                        childItem->setText(PICK_DETAILS_TREE_COLUMN_5,"[m]");
                    }
                }

                break;
            }
            case PICK_ITEM_NODE:
            {
                QTreeWidgetItem *childItem = nullptr;

                if (REntityGroup::typeIsElementGroup(rEntityID.getType()))
                {
                    itemText = tr("Node") + " # " + QString::number(nodeID);

                    const RNode &rNode = rModel.getNode(nodeID);

                    childItem = new QTreeWidgetItem(topItem);
                    childItem->setText(PICK_DETAILS_TREE_COLUMN_1,tr("Coordinates") + ":");
                    childItem->setText(PICK_DETAILS_TREE_COLUMN_2,QString::number(rNode.getX()));
                    childItem->setText(PICK_DETAILS_TREE_COLUMN_3,QString::number(rNode.getY()));
                    childItem->setText(PICK_DETAILS_TREE_COLUMN_4,QString::number(rNode.getZ()));
                    childItem->setText(PICK_DETAILS_TREE_COLUMN_5,"[m]");
                }
                else if (REntityGroup::typeIsInterpolatedElementGroup(rEntityID.getType()))
                {
                    itemText = tr("Interpolated node") + " # " + QString::number(rPickItems[i].getElementPosition()) + " / " + QString::number(rPickItems[i].getNodePosition());
                }

                // Type
                childItem = new QTreeWidgetItem(topItem);
                childItem->setText(PICK_DETAILS_TREE_COLUMN_1,tr("Family") + ":");
                childItem->setText(PICK_DETAILS_TREE_COLUMN_5,REntityGroup::getTypeName(rEntityID.getType()));

                break;
            }
            case PICK_ITEM_HOLE_ELEMENT:
            {
                itemText = tr("Edge") + " # " + QString::number(rPickItems[i].getElementPosition());
                break;
            }
            default:
            {
                continue;
            }
        }

        QMap<RVariableType, PickValue> resultsValues = rModel.getPickedResultsValues(rPickItems[i]);
        if (resultsValues.size() > 0)
        {
            QTreeWidgetItem *childItem = new QTreeWidgetItem(topItem);
            childItem->setText(PICK_DETAILS_TREE_COLUMN_1,"Computed results");
            childItem->setFirstColumnSpanned(true);

            QMap<RVariableType, PickValue>::iterator iter;

            for (iter = resultsValues.begin(); iter != resultsValues.end(); ++iter)
            {
                RVariableType variableType = RVariableType(iter.key());
                RRVector values(iter.value().getValues());

                QTreeWidgetItem *variableItem = new QTreeWidgetItem(childItem);
                variableItem->setText(PICK_DETAILS_TREE_COLUMN_1,RVariable::getName(variableType));

                if (values.size() == 1)
                {
                    variableItem->setText(PICK_DETAILS_TREE_COLUMN_2,QString::number(values[0]));
                    variableItem->setText(PICK_DETAILS_TREE_COLUMN_5,"[" +RVariable::getUnits(variableType) + "]");
                }
                else if (values.size() == 2)
                {
                    variableItem->setText(PICK_DETAILS_TREE_COLUMN_2,QString::number(values[0]));
                    variableItem->setText(PICK_DETAILS_TREE_COLUMN_3,QString::number(values[1]));
                    variableItem->setText(PICK_DETAILS_TREE_COLUMN_5,"[" + RVariable::getUnits(variableType) + "]");
                }
                else if (values.size() == 3)
                {
                    variableItem->setText(PICK_DETAILS_TREE_COLUMN_2,QString::number(values[0]));
                    variableItem->setText(PICK_DETAILS_TREE_COLUMN_3,QString::number(values[1]));
                    variableItem->setText(PICK_DETAILS_TREE_COLUMN_4,QString::number(values[2]));
                    variableItem->setText(PICK_DETAILS_TREE_COLUMN_5,"[" + RVariable::getUnits(variableType) + "]");
                }
            }
        }

        topItem->setText(PICK_DETAILS_TREE_COLUMN_1,itemText);
        topItem->setFirstColumnSpanned(true);
        topItem->setExpanded(true);
    }

    for (int i=0;i<this->topLevelItemCount();i++)
    {
        this->topLevelItem(i)->setForeground(PICK_DETAILS_TREE_COLUMN_1,this->palette().brush(QPalette::Active,QPalette::Text));
        QFont font(this->topLevelItem(i)->font(PICK_DETAILS_TREE_COLUMN_1));
        font.setBold(true);
        this->topLevelItem(i)->setFont(PICK_DETAILS_TREE_COLUMN_1,font);

        for (int j=0;j<this->topLevelItem(i)->childCount();j++)
        {
            this->topLevelItem(i)->child(j)->setForeground(PICK_DETAILS_TREE_COLUMN_1,this->palette().brush(QPalette::Active,QPalette::Text));
            QFont font(this->topLevelItem(i)->child(j)->font(PICK_DETAILS_TREE_COLUMN_1));
            font.setBold(true);
            this->topLevelItem(i)->child(j)->setFont(PICK_DETAILS_TREE_COLUMN_1,font);
        }
    }

    this->resizeColumnsToContent();
}

void PickDetailsTree::onPickListChanged(void)
{
    this->populate();
}

void PickDetailsTree::onItemExpanded(QTreeWidgetItem *)
{
    this->resizeColumnsToContent();
}

void PickDetailsTree::resizeColumnsToContent(void)
{
    this->resizeColumnToContents(PICK_DETAILS_TREE_COLUMN_5);
    this->resizeColumnToContents(PICK_DETAILS_TREE_COLUMN_4);
    this->resizeColumnToContents(PICK_DETAILS_TREE_COLUMN_3);
    this->resizeColumnToContents(PICK_DETAILS_TREE_COLUMN_2);
    this->resizeColumnToContents(PICK_DETAILS_TREE_COLUMN_1);
}
