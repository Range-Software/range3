/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   merge_near_nodes_dialog.cpp                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   18-th November 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Merge near nodes dialog class definition            *
 *********************************************************************/

#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "merge_near_nodes_dialog.h"
#include "session.h"
#include "model_action.h"
#include "job_manager.h"

MergeNearNodesDialog::MergeNearNodesDialog(uint modelID, QWidget *parent)
    : QDialog(parent)
    , modelID(modelID)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Merge near nodes"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout (mainLayout);

    QLabel *label = new QLabel(tr("Merge near nodes for model") + " <b>" + Session::getInstance().getModel(this->modelID).getName() + "</b>");
    mainLayout->addWidget(label);

    QHBoxLayout *toleranceLayout = new QHBoxLayout;
    mainLayout->addLayout(toleranceLayout);

    QLabel *toleranceLabel = new QLabel(tr("Tolerance:"));
    toleranceLayout->addWidget(toleranceLabel);

    this->toleranceEdit = new ValueLineEdit(0.0,1e10);
    this->toleranceEdit->setValue(Session::getInstance().getModel(this->modelID).findMinimumNodeDistance());
    toleranceLayout->addWidget(this->toleranceEdit);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&MergeNearNodesDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&MergeNearNodesDialog::accept);
}

int MergeNearNodesDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        ModelActionInput modelActionInput(this->modelID);
        modelActionInput.setMergeNearNodes(this->toleranceEdit->getValue());

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }

    return retVal;
}
