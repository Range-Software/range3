/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   geometry_transform_widget.cpp                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   6-th November 2014                                       *
 *                                                                   *
 *  DESCRIPTION: Geometry transform widget class definition          *
 *********************************************************************/

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QCheckBox>
#include <QScrollArea>

#include "geometry_transform_widget.h"
#include "model_action.h"
#include "job_manager.h"
#include "session.h"

GeometryTransformWidget::GeometryTransformWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    this->setLayout(vBoxLayout);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    vBoxLayout->addWidget(scrollArea);

    QWidget *containerWidget = new QWidget;
    scrollArea->setWidget(containerWidget);

    QVBoxLayout *containerLayout = new QVBoxLayout(containerWidget);

    QTabWidget *tabWidget = new QTabWidget;
    containerLayout->addWidget(tabWidget);

    this->scaleWidget = new GeometryScaleWidget(this->input.getScaleCenter(),this->input.getScale());
    tabWidget->addTab(this->scaleWidget,tr("Scale"));

    this->rotateWidget = new GeometryRotateWidget(this->input.getRotationCenter(),this->input.getRotation());
    tabWidget->addTab(this->rotateWidget,tr("Rotate"));

    this->translateWidget = new GeometryTranslateWidget(this->input.getTranslation());
    tabWidget->addTab(this->translateWidget,tr("Translate"));

    QGroupBox *applyToGroupBox = new QGroupBox(tr("Apply to"));
    containerLayout->addWidget(applyToGroupBox);

    QGridLayout *applyToLayout = new QGridLayout;
    applyToGroupBox->setLayout(applyToLayout);

    this->applyToAllRadio = new QRadioButton(tr("All"));
    this->applyToAllRadio->setChecked(this->input.getApplyTo() == GeometryTransformInput::ApplyToAll);
    applyToLayout->addWidget(this->applyToAllRadio,0,0,1,1);

    this->applyToVisibleRadio = new QRadioButton(tr("Visible"));
    this->applyToVisibleRadio->setChecked(this->input.getApplyTo() == GeometryTransformInput::ApplyToVisible);
    applyToLayout->addWidget(this->applyToVisibleRadio,0,1,1,1);

    this->applyToSelectedRadio = new QRadioButton(tr("Selected"));
    this->applyToSelectedRadio->setChecked(this->input.getApplyTo() == GeometryTransformInput::ApplyToSelected);
    applyToLayout->addWidget(this->applyToSelectedRadio,0,2,1,1);

    this->includeSharedNodesCheck = new QGroupBox(tr("Include shared nodes"));
    this->includeSharedNodesCheck->setCheckable(true);
    this->includeSharedNodesCheck->setChecked(this->input.getIncludeSharedNodes());
    applyToLayout->addWidget(this->includeSharedNodesCheck,1,0,1,3);
    this->includeSharedNodesCheck->setDisabled(this->input.getApplyTo() == GeometryTransformInput::ApplyToAll);

    QVBoxLayout *includeSharedNodesLayout = new QVBoxLayout;
    this->includeSharedNodesCheck->setLayout(includeSharedNodesLayout);

    this->splitSharedNodesCheck = new QGroupBox(tr("Detach/split shared nodes"));
    this->splitSharedNodesCheck->setCheckable(true);
    this->splitSharedNodesCheck->setChecked(this->input.getSplitSharedNodes());
    includeSharedNodesLayout->addWidget(this->splitSharedNodesCheck);

    QVBoxLayout *splitSharedNodesLayout = new QVBoxLayout;
    this->splitSharedNodesCheck->setLayout(splitSharedNodesLayout);

    this->sweepNodesCheck = new QGroupBox(tr("Sweep nodes"));
    this->sweepNodesCheck->setCheckable(true);
    this->sweepNodesCheck->setChecked(this->input.getSweepSharedNodes());
    splitSharedNodesLayout->addWidget(this->sweepNodesCheck);

    QVBoxLayout *sweepNodesLayout = new QVBoxLayout;
    this->sweepNodesCheck->setLayout(sweepNodesLayout);

    this->sweepNStepsSpin = new QSpinBox;
    this->sweepNStepsSpin->setPrefix(tr("Sweep steps") + ": ");
    this->sweepNStepsSpin->setMinimum(1);
    this->sweepNStepsSpin->setValue(this->input.getNSweepSteps());
    sweepNodesLayout->addWidget(this->sweepNStepsSpin);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    vBoxLayout->addLayout(buttonsLayout);

    this->cancelButton = new QPushButton(QIcon(":/icons/file/pixmaps/range-cancel.svg"),tr("Cancel"));
    buttonsLayout->addWidget(this->cancelButton);

    this->okButton = new QPushButton(QIcon(":/icons/file/pixmaps/range-ok.svg"),tr("Ok"));
    buttonsLayout->addWidget(this->okButton);

    QObject::connect(scaleWidget,
                     &GeometryScaleWidget::scaleChanged,
                     this,
                     &GeometryTransformWidget::onScaleChanged);
    QObject::connect(rotateWidget,
                     &GeometryRotateWidget::rotationChanged,
                     this,
                     &GeometryTransformWidget::onRotationChanged);
    QObject::connect(translateWidget,
                     &GeometryTranslateWidget::translationChanged,
                     this,
                     &GeometryTransformWidget::onTranslationChanged);

    QObject::connect(this->applyToAllRadio,
                     &QRadioButton::clicked,
                     this,
                     &GeometryTransformWidget::onApplyToAllClicked);
    QObject::connect(this->applyToSelectedRadio,
                     &QRadioButton::clicked,
                     this,
                     &GeometryTransformWidget::onApplyToSelectedClicked);
    QObject::connect(this->applyToVisibleRadio,
                     &QRadioButton::clicked,
                     this,
                     &GeometryTransformWidget::onApplyToVisibleClicked);

    QObject::connect(this->includeSharedNodesCheck,
                     &QGroupBox::clicked,
                     this,
                     &GeometryTransformWidget::onIncludeSharedNodesClicked);
    QObject::connect(this->splitSharedNodesCheck,
                     &QGroupBox::clicked,
                     this,
                     &GeometryTransformWidget::onSplitSharedNodesClicked);
    QObject::connect(this->sweepNodesCheck,
                     &QGroupBox::clicked,
                     this,
                     &GeometryTransformWidget::onSweepSharedNodesClicked);
    this->connect(this->sweepNStepsSpin,SIGNAL(valueChanged(int)),SLOT(onSweepNStepsSpinChanged(int)));
//    QObject::connect(this->sweepNStepsSpin,
//                     &QSpinBox::valueChanged,
//                     this,
//                     &GeometryTransformWidget::onSweepNStepsSpinChanged);

    QObject::connect(this->cancelButton,
                     &QPushButton::clicked,
                     this,
                     &GeometryTransformWidget::onCancelClicked);
    QObject::connect(this->okButton,
                     &QPushButton::clicked,
                     this,
                     &GeometryTransformWidget::onOkClicked);
}

void GeometryTransformWidget::onScaleChanged(const RR3Vector &center, const RR3Vector &scales)
{
    this->input.setScale(scales);
    this->input.setScaleCenter(center);
}

void GeometryTransformWidget::onRotationChanged(const RR3Vector &center, const RR3Vector &rotations)
{
    this->input.setRotation(rotations);
    this->input.setRotationCenter(center);
}

void GeometryTransformWidget::onTranslationChanged(const RR3Vector &distances)
{
    this->input.setTranslation(distances);
}

void GeometryTransformWidget::onApplyToAllClicked(bool checked)
{
    if (checked)
    {
        this->input.setApplyTo(GeometryTransformInput::ApplyToAll);
    }
    this->includeSharedNodesCheck->setDisabled(checked);
}

void GeometryTransformWidget::onApplyToSelectedClicked(bool checked)
{
    if (checked)
    {
        this->input.setApplyTo(GeometryTransformInput::ApplyToSelected);
    }
    this->includeSharedNodesCheck->setEnabled(checked);
}

void GeometryTransformWidget::onApplyToVisibleClicked(bool checked)
{
    if (checked)
    {
        this->input.setApplyTo(GeometryTransformInput::ApplyToVisible);
    }
    this->includeSharedNodesCheck->setEnabled(checked);
}

void GeometryTransformWidget::onIncludeSharedNodesClicked(bool checked)
{
    this->input.setIncludeSharedNodes(checked);
}

void GeometryTransformWidget::onSplitSharedNodesClicked(bool checked)
{
    this->input.setSplitSharedNodes(checked);
}

void GeometryTransformWidget::onSweepSharedNodesClicked(bool checked)
{
    this->input.setSweepSharedNodes(checked);
}

void GeometryTransformWidget::onSweepNStepsSpinChanged(int nSweepSteps)
{
    this->input.setNSweepSteps(uint(nSweepSteps));
}

void GeometryTransformWidget::onCancelClicked(void)
{
    emit this->finalize();
}

void GeometryTransformWidget::onOkClicked(void)
{
    QList<uint> selectedModelIDs = Session::getInstance().getSelectedModelIDs();

    for (int i=0;i<selectedModelIDs.size();i++)
    {
        ModelActionInput modelActionInput(selectedModelIDs.at(i));
        modelActionInput.setGeometryTransform(this->input);
        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }

    emit this->finalize();
}