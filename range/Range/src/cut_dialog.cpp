/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   cut_dialog.cpp                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   22-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Cut dialog class definition                         *
 *********************************************************************/

#include <cmath>

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QSlider>

#include "session.h"
#include "cut_dialog.h"
#include "direction_widget.h"
#include "position_widget.h"

CutDialog::CutDialog(uint modelID, uint entityID, QWidget *parent)
    : QDialog(parent)
    , modelID(modelID)
    , entityID(entityID)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Cut editor"));
    this->resize(500,650);

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    QLabel *titleLabel = new QLabel;
    if (this->entityID == RConstants::eod)
    {
        titleLabel->setText(tr("Create new cut."));
    }
    else
    {
        titleLabel->setText(tr("Modify cut:") + " <b>" + Session::getInstance().getModel(modelID).getCut(entityID).getName() + "</b>");
    }
    mainLayout->addWidget(titleLabel, 0, 0, 1, 1);

    REntityGroupTypeMask typeMask = R_ENTITY_GROUP_POINT
                                  | R_ENTITY_GROUP_LINE
                                  | R_ENTITY_GROUP_SURFACE
                                  | R_ENTITY_GROUP_VOLUME;

    Model &rModel = Session::getInstance().getModel(modelID);

    this->modelTree = new ModelTreeSimple(this->modelID,typeMask,this);
    if (this->entityID != RConstants::eod)
    {
        const RCut &rCut = rModel.getCut(entityID);
        this->cutPlane = rCut.getPlane();

        const std::vector<unsigned int> groupIDs = rCut.getElementGroupIDs();
        this->modelTree->clearSelection();
        for (uint i=0;i<groupIDs.size();i++)
        {
            REntityGroupType entityGroupType;
            uint entityIDNum;
            if (rModel.getEntityID(groupIDs[i],entityGroupType,entityIDNum))
            {
                this->modelTree->selectEntity(modelID,entityGroupType,entityIDNum);
            }
        }
    }
    else
    {
        RR3Vector position;
        rModel.findNodeCenter(position[0],position[1],position[2]);
        this->cutPlane.setPosition(position);
    }
    mainLayout->addWidget(this->modelTree, 1, 0, 1, 1);

    QObject::connect(this->modelTree,
                     &ModelTreeSimple::itemSelectionChanged,
                     this,
                     &CutDialog::onModelTreeSelectionChanged);

    double xMin = 0.0;
    double xMax = 0.0;
    double yMin = 0.0;
    double yMax = 0.0;
    double zMin = 0.0;
    double zMax = 0.0;

    rModel.findNodeLimits(xMin,xMax,yMin,yMax,zMin,zMax);

    double dx = xMax - xMin;
    double dy = yMax - yMin;
    double dz = zMax - zMin;

    xMin -= dx;
    xMax += dx;

    yMin -= dy;
    yMax += dy;

    zMin -= dz;
    zMax += dz;

    PositionWidget *positionWidget = new PositionWidget("Plane position",this->cutPlane.getPosition());
    positionWidget->hideButtons();
    positionWidget->hidePickButton();
    positionWidget->setXRange(xMin,xMax);
    positionWidget->setYRange(yMin,yMax);
    positionWidget->setZRange(zMin,zMax);
    mainLayout->addWidget(positionWidget, 2, 0, 1, 1);

    DirectionWidget *directionWidget = new DirectionWidget("Plane normal",this->cutPlane.getNormal());
    directionWidget->hideButtons();
    mainLayout->addWidget(directionWidget, 3, 0, 1, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, 4, 0, 1, 1);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    this->okButton = new QPushButton(okIcon, tr("Ok"));
    this->okButton->setEnabled(this->modelTree->selectedItems().size());
    this->okButton->setDefault(true);
    buttonsLayout->addWidget(this->okButton);

    QObject::connect(positionWidget,&PositionWidget::changed,this,&CutDialog::onPositionChanged);
    QObject::connect(directionWidget,&DirectionWidget::changed,this,&CutDialog::onDirectionChanged);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&CutDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&CutDialog::accept);

    Session::getInstance().setBeginDrawCutPlane(this->cutPlane);

    QObject::connect(this,&QDialog::rejected,this,&CutDialog::onReject);
    QObject::connect(this,&QDialog::accepted,this,&CutDialog::onAccept);
}

void CutDialog::onAccept(void)
{
    Session::getInstance().storeCurentModelVersion(this->modelID,tr("Create a cut"));

    QList<SessionEntityID> entities = this->modelTree->getSelected();

    if (this->entityID != RConstants::eod)
    {
        RCut &rCut = Session::getInstance().getModel(this->modelID).getCut(this->entityID);

        rCut.setPlane(this->cutPlane);

        rCut.clearElementGroupIDs();

        for (int i=0;i<entities.size();i++)
        {
            uint elementGroupId = Session::getInstance().getModel(this->modelID).getElementGroupID(entities[i].getType(),
                                                                                                   entities[i].getEid());
            if (elementGroupId == RConstants::eod)
            {
                continue;
            }
            rCut.addElementGroupID(elementGroupId);
        }

        RLogger::info("Modified vector field \'%s\'\n",rCut.getName().toUtf8().constData());
    }
    else
    {
        RCut cut;

        cut.setName("Cut");
        cut.setPlane(this->cutPlane);

        for (int i=0;i<entities.size();i++)
        {
            uint elementGroupId = Session::getInstance().getModel(this->modelID).getElementGroupID(entities[i].getType(),
                                                                                                   entities[i].getEid());
            cut.addElementGroupID(elementGroupId);
        }

        Session::getInstance().getModel(this->modelID).addCut(cut);
        RLogger::info("Created new vector field \'%s\'\n",cut.getName().toUtf8().constData());
    }

    Session::getInstance().setModelChanged(this->modelID);

    this->close();
    Session::getInstance().setEndDrawCutPlane();
}

void CutDialog::onReject(void)
{
    this->close();
    Session::getInstance().setEndDrawCutPlane();
}

void CutDialog::onPositionChanged(const RR3Vector &position)
{
    this->cutPlane.setPosition(position);
    Session::getInstance().setBeginDrawCutPlane(this->cutPlane);
}

void CutDialog::onDirectionChanged(const RR3Vector &direction)
{
    this->cutPlane.setNormal(direction);
    Session::getInstance().setBeginDrawCutPlane(this->cutPlane);
}

void CutDialog::onModelTreeSelectionChanged(void)
{
    this->okButton->setEnabled(this->modelTree->selectedItems().size());
}
