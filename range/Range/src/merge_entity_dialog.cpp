/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   merge_entity_dialog.cpp                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   30-th November 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Merge entity dialog class definition                *
 *********************************************************************/

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>

#include "merge_entity_dialog.h"
#include "session.h"
#include "model_action.h"
#include "job_manager.h"

MergeEntityDialog::MergeEntityDialog(REntityGroupTypeMask entityTypeMask, QWidget *parent) :
    QDialog(parent),
    entityTypeMask(entityTypeMask)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Merge entities"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout (mainLayout);

    QLabel *listLabel = new QLabel(tr("Following entities will be merged:"));
    mainLayout->addWidget(listLabel);

    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        QList<SessionEntityID> selectedEntities = this->findEntities(modelIDs[i],this->entityTypeMask);

        QList<SessionEntityID> selectedPoints = SessionEntityID::filterList(selectedEntities,R_ENTITY_GROUP_POINT);
        QList<SessionEntityID> selectedLines = SessionEntityID::filterList(selectedEntities,R_ENTITY_GROUP_LINE);
        QList<SessionEntityID> selectedSurfaces = SessionEntityID::filterList(selectedEntities,R_ENTITY_GROUP_SURFACE);
        QList<SessionEntityID> selectedVolumes = SessionEntityID::filterList(selectedEntities,R_ENTITY_GROUP_VOLUME);

        Model &rModel = Session::getInstance().getModel(modelIDs[i]);

        QGroupBox *modelGroupBox= new QGroupBox;
        modelGroupBox->setTitle(rModel.getName());
        mainLayout->addWidget(modelGroupBox);

        QGridLayout *modelGroupLayout = new QGridLayout;
        modelGroupBox->setLayout(modelGroupLayout);

        QString text = "<dl>";
        if (selectedPoints.size() >= 2)
        {
            text += "<dt>" + tr("Points:");
            text += "<dd>";
            for (int j=0;j<selectedPoints.size();j++)
            {
                text += rModel.getPoint(selectedPoints[j].getEid()).getName() + QString(",&nbsp;");
            }
        }
        if (selectedLines.size() >= 2)
        {
            text += "<dt>" + tr("Lines:");
            text += "<dd>";
            for (int j=0;j<selectedLines.size();j++)
            {
                text += rModel.getLine(selectedLines[j].getEid()).getName() + QString(",&nbsp;");
            }
        }
        if (selectedSurfaces.size() >= 2)
        {
            text += "<dt>" + tr("Surfaces:");
            text += "<dd>";
            for (int j=0;j<selectedSurfaces.size();j++)
            {
                text += rModel.getSurface(selectedSurfaces[j].getEid()).getName() + QString(",&nbsp;");
            }
        }
        if (selectedVolumes.size() >= 2)
        {
            text += "<dt>" + tr("Volumes:");
            text += "<dd>";
            for (int j=0;j<selectedVolumes.size();j++)
            {
                text += rModel.getVolume(selectedVolumes[j].getEid()).getName() + QString(",&nbsp;");
            }
        }
        text += "</dl>";

        QLabel *modelLabel = new QLabel(text);
        modelGroupLayout->addWidget(modelLabel,1,0,1,1);
    }

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&MergeEntityDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&MergeEntityDialog::accept);

    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

int MergeEntityDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();
        for (int i=0;i<modelIDs.size();i++)
        {
            QList<SessionEntityID> selectedEntities = SessionEntityID::filterList(this->findEntities(modelIDs[i],this->entityTypeMask));

            if (selectedEntities.size() < 2)
            {
                continue;
            }
            ModelActionInput modelActionInput(modelIDs[i]);
            modelActionInput.setMergeEntities(selectedEntities);

            ModelAction *modelAction = new ModelAction;
            modelAction->setAutoDelete(true);
            modelAction->addAction(modelActionInput);
            JobManager::getInstance().submit(modelAction);
        }
    }

    return retVal;
}

bool MergeEntityDialog::entitiesSelected(REntityGroupTypeMask entityTypeMask)
{
    QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<modelIDs.size();i++)
    {
        QList<SessionEntityID> selectedEntities = Session::filterSelectedEntityIDs(Session::getInstance().getSelectedEntityIDs(),modelIDs[i],entityTypeMask);

        QList<SessionEntityID> selectedPoints = SessionEntityID::filterList(selectedEntities,R_ENTITY_GROUP_POINT);
        QList<SessionEntityID> selectedLines = SessionEntityID::filterList(selectedEntities,R_ENTITY_GROUP_LINE);
        QList<SessionEntityID> selectedSurfaces = SessionEntityID::filterList(selectedEntities,R_ENTITY_GROUP_SURFACE);
        QList<SessionEntityID> selectedVolumes = SessionEntityID::filterList(selectedEntities,R_ENTITY_GROUP_VOLUME);
        if (selectedPoints.size() >= 2 || selectedLines.size() >= 2 || selectedSurfaces.size() >= 2 || selectedVolumes.size() >= 2)
        {
            return true;
        }
    }
    return false;
}

QList<SessionEntityID> MergeEntityDialog::findEntities(uint mid, REntityGroupTypeMask entityTypeMask)
{
    return Session::filterSelectedEntityIDs(Session::getInstance().getSelectedEntityIDs(),mid,entityTypeMask);
}
