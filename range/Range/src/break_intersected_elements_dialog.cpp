/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   break_intersected_elements_dialog.cpp                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   24-th March 2015                                         *
 *                                                                   *
 *  DESCRIPTION: Break intersected elements dialog class definition  *
 *********************************************************************/

#include <QGridLayout>
#include <QIcon>
#include <QPushButton>
#include <QLabel>

#include "break_intersected_elements_dialog.h"
#include "model_action.h"
#include "job_manager.h"
#include "session.h"


BreakIntersectedElementsDialog::BreakIntersectedElementsDialog(uint modelID, QWidget *parent)
    : QDialog(parent)
    , modelID(modelID)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Break intersected elements"));

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout(mainLayout);

    uint mainLayoutRow = 0;

    QLabel *nIterationsLabel = new QLabel(tr("Number of iterations:"));
    mainLayout->addWidget(nIterationsLabel, mainLayoutRow,0,1,1);

    this->nIterationsSpin = new QSpinBox;
    this->nIterationsSpin->setRange(1,100);
    this->nIterationsSpin->setValue(3);
    mainLayout->addWidget(this->nIterationsSpin, mainLayoutRow,1,1,1);

    mainLayoutRow++;

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, mainLayoutRow++, 0, 1, 2);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&QDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&QDialog::accept);

}

int BreakIntersectedElementsDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        ModelActionInput modelActionInput(this->modelID);
        modelActionInput.setBreakIntersectedElements(uint(this->nIterationsSpin->value()));

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }

    return retVal;
}
