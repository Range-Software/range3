/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   mark_entity_dialog.cpp                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   8-th November 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Mark entity dialog class definition                 *
 *********************************************************************/

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

#include "session.h"
#include "mark_entity_dialog.h"
#include "model_action.h"
#include "job_manager.h"

double MarkEntityDialog::separationAngle = 45.0;

MarkEntityDialog::MarkEntityDialog(REntityGroupType entityType, QWidget *parent)
    : QDialog(parent)
    , entityType(entityType)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    QString windowTitleStr = tr("Mark entity") + " (" + RElementGroup::getTypeName(entityType) + ")";
    this->setWindowTitle(tr(windowTitleStr.toUtf8().constData()));

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    this->autoMarkRadio = new QRadioButton("Auto-mark all entities");
    mainLayout->addWidget(this->autoMarkRadio,0,0,1,2);

    QString markPickMessage = tr("To mark elements at least one element must be picked.")
                              + " " + tr("Use") + " <strong>"
                              + GLActionEvent::findKeyMouseCombination(GL_ACTION_EVENT_PICK_ELEMENT)
                              + "</strong> " + tr("to pick element") + ".";
    this->pickedMarkRadio = new QRadioButton("Mark only selected and related elements");
    this->pickedMarkRadio->setToolTip(markPickMessage);
    mainLayout->addWidget(this->pickedMarkRadio,1,0,1,2);

    if (Session::getInstance().getPickList().isEmpty())
    {
        this->autoMarkRadio->setChecked(true);
        this->pickedMarkRadio->setDisabled(true);
    }
    else
    {
        this->pickedMarkRadio->setChecked(true);
    }

    QLabel *separationAngleLabel = new QLabel(tr("Separation angle:"));
    mainLayout->addWidget(separationAngleLabel, 2, 0, 1, 1);

    this->separationAngleSpin = new QSpinBox;
    this->separationAngleSpin->setRange(0,360);
    this->separationAngleSpin->setValue(MarkEntityDialog::separationAngle);
    mainLayout->addWidget(this->separationAngleSpin, 2, 1, 1, 2);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, 3, 0, 1, 2);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&MarkEntityDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&MarkEntityDialog::accept);
}

int MarkEntityDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        this->separationAngle = double(this->separationAngleSpin->value());
        bool markSuccess = false;
        if (this->autoMarkRadio->isChecked() || Session::getInstance().getPickList().isEmpty())
        {
            // Automark all selected models.
            QList<uint> modelIDs = Session::getInstance().getSelectedModelIDs();
            for (int i=0;i<modelIDs.size();i++)
            {
                RLogger::info("Auto-marking model \'%s\'\n",Session::getInstance().getModel(modelIDs[i]).getName().toUtf8().constData());
                if (this->autoMarkEntities(modelIDs[i]))
                {
                    markSuccess = true;
                }
            }
        }
        else
        {
            // Mark each model at a time.
            QList<uint> modelIDs = Session::getInstance().getPickList().getModelIDs();
            for (int i=0;i<modelIDs.size();i++)
            {
                RLogger::info("Marking model \'%s\'\n",Session::getInstance().getModel(modelIDs[i]).getName().toUtf8().constData());
                if (this->markEntities(modelIDs[i],Session::getInstance().getPickList().getItems(modelIDs[i])))
                {
                    markSuccess = true;
                }
            }
        }
        if (markSuccess)
        {
            Session::getInstance().getPickList().clear();
        }
    }

    return retVal;
}

bool MarkEntityDialog::autoMarkEntities(uint modelID)
{
    ModelActionInput modelActionInput(modelID);

    switch (this->entityType)
    {
        case R_ENTITY_GROUP_POINT:
            break;
        case R_ENTITY_GROUP_LINE:
            break;
        case R_ENTITY_GROUP_SURFACE:
            modelActionInput.setAutoMarkSurfaces(this->separationAngle);
            break;
        case R_ENTITY_GROUP_VOLUME:
            break;
        default:
            RLogger::error("Unknown element group \'%d\'",int(this->entityType));
            return false;
    }

    ModelAction *modelAction = new ModelAction;
    modelAction->setAutoDelete(true);
    modelAction->addAction(modelActionInput);
    JobManager::getInstance().submit(modelAction);

    return true;
}

bool MarkEntityDialog::markEntities(uint modelID, const QVector<PickItem> &pickList)
{
    QList<uint> elementIDs;
    for (int j=0;j<pickList.size();j++)
    {
        const SessionEntityID &pickEntityID = pickList[j].getEntityID();
        if (pickEntityID.getMid() == modelID && pickEntityID.getType() == this->entityType)
        {
            uint elementID = pickList[j].getElementID();
            if (elementID < Session::getInstance().getModel(modelID).getNElements() &&
                R_ELEMENT_TYPE_IS_SURFACE(Session::getInstance().getModel(modelID).getElement(elementID).getType()))
            {
                elementIDs.append(elementID);
            }
        }
    }

    if (elementIDs.size() == 0)
    {
        QMessageBox::warning(this,tr("No elements were picked."),tr("To mark elements at least one element must be picked.")
                                                                 + " " + tr("Use") + " "
                                                                 + GLActionEvent::findKeyMouseCombination(GL_ACTION_EVENT_PICK_ELEMENT)
                                                                 + " " + tr("to pick element") + ".");
        return false;
    }

    ModelActionInput modelActionInput(modelID);

    switch (this->entityType)
    {
        case R_ENTITY_GROUP_POINT:
            break;
        case R_ENTITY_GROUP_LINE:
            break;
        case R_ENTITY_GROUP_SURFACE:
            modelActionInput.setMarkSurfaces(this->separationAngle,elementIDs);
            break;
        case R_ENTITY_GROUP_VOLUME:
            break;
        default:
            RLogger::error("Unknown element group \'%d\'",int(this->entityType));
            return false;
    }

    ModelAction *modelAction = new ModelAction;
    modelAction->setAutoDelete(true);
    modelAction->addAction(modelActionInput);
    JobManager::getInstance().submit(modelAction);

    return true;
}
