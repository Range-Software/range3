/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   bool_union_dialog.cpp                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   26-th March 2015                                         *
 *                                                                   *
 *  DESCRIPTION: Bool operation - union dialog class definition      *
 *********************************************************************/

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QPushButton>
#include <QLabel>

#include "bool_union_dialog.h"
#include "model_action.h"
#include "job_manager.h"
#include "session.h"

BoolUnionDialog::BoolUnionDialog(uint modelID, const QList<SessionEntityID> &entityIDs, QWidget *parent) :
    QDialog(parent),
    modelID(modelID),
    entityIDs(entityIDs)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Union"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    QLabel *messageLabel = new QLabel(tr("Create union from selected surfaces?"));
    mainLayout->addWidget(messageLabel);

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

int BoolUnionDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        ModelActionInput modelActionInput(this->modelID);
        modelActionInput.setBoolUnion(this->entityIDs);

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }

    return retVal;
}
