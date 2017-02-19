/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   bool_difference_dialog.cpp                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   26-th March 2015                                         *
 *                                                                   *
 *  DESCRIPTION: Bool operation - difference dialog class definition *
 *********************************************************************/

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QPushButton>
#include <QLabel>

#include "bool_difference_dialog.h"
#include "model_action.h"
#include "job_manager.h"
#include "session.h"

BoolDifferenceDialog::BoolDifferenceDialog(uint modelID, const QList<SessionEntityID> &entityIDs, QWidget *parent) :
    QDialog(parent),
    modelID(modelID)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Difference"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    QLabel *label = new QLabel(tr("Select entity which will be substracted."));
    mainLayout->addWidget(label);

    this->listWidget = new QListWidget;
    this->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    mainLayout->addWidget(this->listWidget);

    Model &rModel = Session::getInstance().getModel(this->modelID);
    for (int i=0;i<entityIDs.size();i++)
    {
        if (entityIDs[i].getMid() != this->modelID)
        {
            continue;
        }
        uint groupID = rModel.getEntityGroupID(entityIDs[i].getType(),entityIDs[i].getEid());
        if (groupID == RConstants::eod)
        {
            continue;
        }
        const REntityGroup *pEntityGroup = rModel.getEntityGroupPtr(groupID,true);
        if (!pEntityGroup)
        {
            continue;
        }
        this->entityIDs.push_back(entityIDs[i]);

        QListWidgetItem *item = new QListWidgetItem(this->listWidget);
        item->setText(pEntityGroup->getName());
        item->setSelected(true);
        item->setData(Qt::UserRole,QVariant(this->entityIDs.size()-1));
    }

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&QDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&QDialog::accept);
}

int BoolDifferenceDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        QList<QListWidgetItem*> selectedItems = this->listWidget->selectedItems();
        if (selectedItems.size() > 0)
        {
            int position = selectedItems[0]->data(Qt::UserRole).toInt();
            if (position >= 0 && position < this->entityIDs.size())
            {
                qSwap(this->entityIDs[position],this->entityIDs[this->entityIDs.size()-1]);
            }
        }

        ModelActionInput modelActionInput(this->modelID);
        modelActionInput.setBoolDifference(this->entityIDs);

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }

    return retVal;
}
