/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   point_inside_surface_dialog.cpp                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   27-th March 2015                                         *
 *                                                                   *
 *  DESCRIPTION: Check if point is inside surface dialog class       *
 *               definition                                          *
 *********************************************************************/

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QPushButton>

#include "point_inside_surface_dialog.h"
#include "model_action.h"
#include "job_manager.h"
#include "session.h"

PointInsideSurfaceDialog::PointInsideSurfaceDialog(uint modelID, const QList<SessionEntityID> &entityIDs, QWidget *parent) :
    QDialog(parent),
    modelID(modelID),
    entityIDs(entityIDs)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Union"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    this->position = new PositionWidget(tr("Point position"),RR3Vector(0.0,0.0,0.0));
    this->position->hideButtons();
    this->position->hideSliders();
    mainLayout->addWidget(this->position);

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

int PointInsideSurfaceDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        ModelActionInput modelActionInput(this->modelID);
        modelActionInput.setCheckPointInsideSurface(this->position->getPosition(),this->entityIDs);

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }

    return retVal;
}
