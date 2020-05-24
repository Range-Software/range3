/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   monitoring_points_dialog.h                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   12-th March 20134                                        *
 *                                                                   *
 *  DESCRIPTION: Monitoring points class declaration                 *
 *********************************************************************/

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

#include <rmlib.h>

#include "monitoring_points_dialog.h"
#include "session.h"
#include "variable_selector.h"

MonitoringPointsDialog::MonitoringPointsDialog(uint modelID, QWidget *parent) :
    QDialog(parent),
    modelID(modelID)
{
    const RMonitoringPointManager &monitoringPointsManager = Session::getInstance().getModel(this->modelID).getMonitoringPointManager();

    this->setWindowTitle(tr("Define monitoring points"));
    this->resize(600,300);

    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    int mainLayoutRow = 0;

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout(mainLayout);

    this->tableWidget = new QTableWidget(0,4);
    this->tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Variable") << "X" << "Y" << "Z");
    for (uint i=0;i<monitoringPointsManager.size();i++)
    {
        this->addMonitoringPoint(monitoringPointsManager[i]);
    }
    mainLayout->addWidget(this->tableWidget, mainLayoutRow++, 0, 1, 1);

    QHBoxLayout *actionButtonsLayout = new QHBoxLayout;
    mainLayout->addLayout(actionButtonsLayout, mainLayoutRow++, 0, 1, 1);

    QPushButton *addButton = new QPushButton(tr("Add"));
    actionButtonsLayout->addWidget(addButton);

    this->removeButton = new QPushButton(tr("Remove"));
    this->removeButton->setEnabled(this->tableWidget->selectedItems().size() > 0);
    actionButtonsLayout->addWidget(this->removeButton);

    this->usePickedButton = new QPushButton(tr("Set coordinates from picked element/node"));
    this->usePickedButton->setEnabled(!Session::getInstance().getPickList().isEmpty() && this->tableWidget->selectedItems().size() > 0);
    mainLayout->addWidget(this->usePickedButton, mainLayoutRow++, 0, 1, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, mainLayoutRow++, 0, 1, 1);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(this->tableWidget,&QTableWidget::itemSelectionChanged,this,&MonitoringPointsDialog::onItemSelectionChanged);
    QObject::connect(addButton,&QPushButton::clicked,this,&MonitoringPointsDialog::onAddClicked);
    QObject::connect(this->removeButton,&QPushButton::clicked,this,&MonitoringPointsDialog::onRemoveClicked);
    QObject::connect(this->usePickedButton,&QPushButton::clicked,this,&MonitoringPointsDialog::onUsePickedClicked);
    QObject::connect(cancelButton,&QPushButton::clicked,this,&MonitoringPointsDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&MonitoringPointsDialog::accept);
}

int MonitoringPointsDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        Session::getInstance().storeCurentModelVersion(this->modelID,tr("Set monitoring points"));

        RMonitoringPointManager &monitoringPointsManager = Session::getInstance().getModel(this->modelID).getMonitoringPointManager();
        monitoringPointsManager.clear();

        for (int i=0;i<this->tableWidget->rowCount();i++)
        {
            VariableSelector *variableSelector = dynamic_cast<VariableSelector*>(this->tableWidget->cellWidget(i,0));
            QTableWidgetItem *xItem = this->tableWidget->item(i,1);
            QTableWidgetItem *yItem = this->tableWidget->item(i,2);
            QTableWidgetItem *zItem = this->tableWidget->item(i,3);

            RVariableType variableType = variableSelector->getCurrentVariableType();
            double x = xItem->text().toDouble();
            double y = yItem->text().toDouble();
            double z = zItem->text().toDouble();

            monitoringPointsManager.push_back(RMonitoringPoint(variableType,RR3Vector(x,y,z)));
        }
        Session::getInstance().setModelChanged(this->modelID);
    }

    return retVal;
}

void MonitoringPointsDialog::addMonitoringPoint(const RMonitoringPoint &monitoringPoint)
{
    int row = this->tableWidget->rowCount();

    this->tableWidget->insertRow(row);

    VariableSelector *variableSelector = new VariableSelector(RProblem::getVariableTypes(Session::getInstance().getModel(this->modelID).getProblemTaskTree().getProblemTypeMask()));
    variableSelector->setCurrentVariableType(monitoringPoint.getVariableType());
    this->tableWidget->setCellWidget(row,0,variableSelector);

    QTableWidgetItem *xItem = new QTableWidgetItem(QString::number(monitoringPoint.getPosition()[0]));
    this->tableWidget->setItem(row,1,xItem);

    QTableWidgetItem *yItem = new QTableWidgetItem(QString::number(monitoringPoint.getPosition()[1]));
    this->tableWidget->setItem(row,2,yItem);

    QTableWidgetItem *zItem = new QTableWidgetItem(QString::number(monitoringPoint.getPosition()[2]));
    this->tableWidget->setItem(row,3,zItem);
}

QList<int> MonitoringPointsDialog::findSelectedRows(void) const
{
    QList<QTableWidgetItem*> selectedItems = this->tableWidget->selectedItems();
    QMap<int,bool> rowBook;

    for (int i=0;i<selectedItems.size();i++)
    {
        rowBook[selectedItems.at(i)->row()] = true;
    }

    QList<int> rowList = rowBook.uniqueKeys();
    std::sort(rowList.begin(),rowList.end());

    return rowList;
}

RR3Vector MonitoringPointsDialog::findPickedPosition(void) const
{
    const PickList &pickList = Session::getInstance().getPickList();

    const QVector<PickItem> &pickItems = pickList.getItems();

    for (int i=0;i<pickItems.size();i++)
    {
        const SessionEntityID &pickEntityID = pickItems[i].getEntityID();

        if (pickEntityID.getMid() != this->modelID)
        {
            continue;
        }

        if (pickItems[i].getItemType() == PICK_ITEM_NODE)
        {
            return Session::getInstance().getModel(pickEntityID.getMid()).getNode(pickItems[i].getNodeID()).toVector();
        }
        else if (pickItems[i].getItemType() == PICK_ITEM_ELEMENT)
        {
            double cx, cy, cz;
            const Model &rModel = Session::getInstance().getModel(pickEntityID.getMid());
            rModel.getElement(pickItems[i].getElementID()).findCenter(rModel.getNodes(),cx,cy,cz);
            return RR3Vector(cx, cy, cz);
        }
    }

    return RR3Vector();
}

void MonitoringPointsDialog::onItemSelectionChanged(void)
{
    this->removeButton->setEnabled(this->tableWidget->selectedItems().size() > 0);
    this->usePickedButton->setEnabled(this->tableWidget->selectedItems().size() > 0);
}

void MonitoringPointsDialog::onAddClicked(void)
{
    this->addMonitoringPoint(RMonitoringPoint());
}

void MonitoringPointsDialog::onRemoveClicked(void)
{
    QList<int> selectedRows = this->findSelectedRows();
    for(int i=selectedRows.count()-1;i>=0;i--)
    {
        this->tableWidget->removeRow(selectedRows.at(i));
    }
}

void MonitoringPointsDialog::onUsePickedClicked(void)
{
    RR3Vector position = this->findPickedPosition();

    QList<int> selectedRows = this->findSelectedRows();
    for (int i=0;i<selectedRows.size();i++)
    {
        this->tableWidget->item(selectedRows.at(i),1)->setText(QString::number(position[0]));
        this->tableWidget->item(selectedRows.at(i),2)->setText(QString::number(position[1]));
        this->tableWidget->item(selectedRows.at(i),3)->setText(QString::number(position[2]));
    }
}
