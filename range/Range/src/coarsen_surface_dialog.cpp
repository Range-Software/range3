/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   coarsen_surface_dialog.cpp                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   17-th March 2015                                         *
 *                                                                   *
 *  DESCRIPTION: Coarsen surface dialog class definition             *
 *********************************************************************/

#include <QGridLayout>
#include <QIcon>
#include <QPushButton>
#include <QLabel>

#include <float.h>

#include <rblib.h>

#include "coarsen_surface_dialog.h"
#include "model_action.h"
#include "job_manager.h"
#include "session.h"

static uint mid = RConstants::eod;
static double edgeLength = RConstants::eps;
static double elementArea = RConstants::eps;

CoarsenSurfaceDialog::CoarsenSurfaceDialog(uint modelID, const QList<SessionEntityID> &entityIDs, QWidget *parent) :
    QDialog(parent),
    modelID(modelID),
    entityIDs(entityIDs)
{
    if (mid != modelID)
    {
        this->findGeometryLimits();
    }

    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Coarsen surface elements"));

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout(mainLayout);

    uint mainLayoutRow = 0;

    QLabel *messageLabel = new QLabel(tr("Coarsen selected surfaces."));
    mainLayout->addWidget(messageLabel, mainLayoutRow++,0,1,2);

    QLabel *edgeLengthLabel = new QLabel(tr("Edge length:"));
    mainLayout->addWidget(edgeLengthLabel, mainLayoutRow,0,1,1);

    this->edgeLengthEdit = new ValueLineEdit(0.0,DBL_MAX);
    this->edgeLengthEdit->setValue(edgeLength);
    mainLayout->addWidget(this->edgeLengthEdit, mainLayoutRow,1,1,1);
    mainLayoutRow++;

    QLabel *elementAreaLabel = new QLabel(tr("Element area:"));
    mainLayout->addWidget(elementAreaLabel, mainLayoutRow,0,1,1);

    this->elementAreaEdit = new ValueLineEdit(0.0,DBL_MAX);
    this->elementAreaEdit->setValue(elementArea);
    mainLayout->addWidget(this->elementAreaEdit, mainLayoutRow,1,1,1);
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

int CoarsenSurfaceDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        mid = this->modelID;
        edgeLength = this->edgeLengthEdit->getValue();
        elementArea = this->elementAreaEdit->getValue();

        ModelActionInput modelActionInput(this->modelID);
        modelActionInput.setCoarsenSurface(this->entityIDs,this->edgeLengthEdit->getValue(),this->elementAreaEdit->getValue());

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }

    return retVal;
}

void CoarsenSurfaceDialog::findGeometryLimits(void)
{
    edgeLength = elementArea = RConstants::eps;

    const Model &rModel = Session::getInstance().getModel(this->modelID);

    bool firstTime = true;
    for (uint i=0;i<rModel.getNElements();i++)
    {
        const RElement &rElement = rModel.getElement(i);

        if (!R_ELEMENT_TYPE_IS_SURFACE(rElement.getType()))
        {
            continue;
        }

        uint n1, n2;
        double lenght, area;

        if (!rElement.findShortestEdge(rModel.getNodes(),lenght,n1,n2))
        {
            continue;
        }

        if (!rElement.findArea(rModel.getNodes(),area))
        {
            continue;
        }

        if (firstTime)
        {
            edgeLength = lenght;
            elementArea = area;
            firstTime = false;
        }
        else
        {
            edgeLength = std::min(edgeLength,lenght);
            elementArea = std::min(elementArea,area);
        }
    }
}
