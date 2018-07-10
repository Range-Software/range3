/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   time_solver_setup_widget.cpp                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   15-th June 2012                                          *
 *                                                                   *
 *  DESCRIPTION: Time-solver setup widget class definition           *
 *********************************************************************/

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>

#include "session.h"
#include "time_solver_setup_widget.h"
#include "value_line_edit.h"

TimeSolverSetupWidget::TimeSolverSetupWidget(const RTimeSolver &timeSolver, QWidget *parent)
    : QWidget(parent)
    , timeSolver(timeSolver)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    this->setLayout(mainLayout);

    QGroupBox *groupBox = new QGroupBox(tr("Time-solver"));
    groupBox->setToolTip(tr("Enable/disable time-dependent solution."));
    groupBox->setCheckable(true);
    groupBox->setChecked(this->timeSolver.getEnabled());
    mainLayout->addWidget(groupBox);

    QObject::connect(groupBox,&QGroupBox::toggled,this,&TimeSolverSetupWidget::onTimeSolverEnabledChanged);

    QGridLayout *groupLayout = new QGridLayout;
    groupBox->setLayout(groupLayout);

    int groupLayoutRow = 0;

    // Time-march approximation
    QLabel *labelTimeApprox = new QLabel(tr("Time-march approximation"));
    groupLayout->addWidget(labelTimeApprox,groupLayoutRow,0);

    QComboBox *comboTimeApprox = new QComboBox();
    comboTimeApprox->addItem(RTimeSolver::getTimeMarchApproximationName(R_TIME_MARCH_CENTRAL));
    comboTimeApprox->addItem(RTimeSolver::getTimeMarchApproximationName(R_TIME_MARCH_BACKWARD));
    comboTimeApprox->addItem(RTimeSolver::getTimeMarchApproximationName(R_TIME_MARCH_FORWARD));
    comboTimeApprox->setCurrentIndex(this->timeSolver.getTimeMarchApproximation());
    comboTimeApprox->setToolTip(tr("Time-march approximation."));
    groupLayout->addWidget(comboTimeApprox,groupLayoutRow++,1);

    this->connect(comboTimeApprox,SIGNAL(currentIndexChanged(int)),SLOT(onTimeApproximationChanged(int)));

    // Start time
    QLabel *labelStartTime = new QLabel(tr("Start time"));
    groupLayout->addWidget(labelStartTime,groupLayoutRow,0);

    ValueLineEdit *lineStartTime = new ValueLineEdit;
    lineStartTime->setDoubleValidator();
    lineStartTime->setText(QString::number(this->timeSolver.getInputStartTime()));
    lineStartTime->setToolTip(tr("Start time in seconds."));
    groupLayout->addWidget(lineStartTime,groupLayoutRow++,1);

    QObject::connect(lineStartTime,&ValueLineEdit::valueChanged,this,&TimeSolverSetupWidget::onStartTimeChanged);

    // End time
    QLabel *labelEndTime = new QLabel(tr("End time") + " (" + tr("read only") + ")");
    groupLayout->addWidget(labelEndTime,groupLayoutRow,0);

    this->lineEndTime = new ValueLineEdit;
    this->lineEndTime->setDoubleValidator();
    this->lineEndTime->setValue(this->findEndTime());
    this->lineEndTime->setToolTip(tr("End time in seconds."));
    this->lineEndTime->setReadOnly(true);
    groupLayout->addWidget(this->lineEndTime,groupLayoutRow++,1);

    // Default time-step size
    QLabel *labelTimeStepSize = new QLabel(tr("Time-step size"));
    groupLayout->addWidget(labelTimeStepSize,groupLayoutRow,0);

    ValueLineEdit *lineTimeStepSize = new ValueLineEdit(1.0e-99,1.0e99);
    lineTimeStepSize->setText(QString::number(this->timeSolver.getInputTimeStepSize()));
    lineTimeStepSize->setToolTip(tr("Time-step size in seconds. This also determines end time."));
    groupLayout->addWidget(lineTimeStepSize,groupLayoutRow++,1);

    QObject::connect(lineTimeStepSize,&ValueLineEdit::valueChanged,this,&TimeSolverSetupWidget::onTimeStepSizeChanged);

    // Number of time-steps
    QLabel *labelNTimeSteps = new QLabel(tr("Number of time-steps"));
    groupLayout->addWidget(labelNTimeSteps,groupLayoutRow,0);

    QSpinBox *spinNTimeSteps = new QSpinBox;
    spinNTimeSteps->setMinimum(R_TIME_STEP_MIN_NUMBER);
    spinNTimeSteps->setMaximum(R_TIME_STEP_MAX_NUMBER);
    spinNTimeSteps->setValue(this->timeSolver.getInputNTimeSteps());
    spinNTimeSteps->setToolTip(tr("Number of time-steps to solve."));
    groupLayout->addWidget(spinNTimeSteps,groupLayoutRow++,1);

    this->connect(spinNTimeSteps,SIGNAL(valueChanged(int)),SLOT(onNTimeStepsChanged(int)));

    // Write output frequency
    QLabel *labelOutputFrequency = new QLabel(tr("Output frequency"));
    groupLayout->addWidget(labelOutputFrequency,groupLayoutRow,0);

    QSpinBox *spinOutputFrequency = new QSpinBox;
    spinOutputFrequency->setMinimum(R_TIME_MIN_OUTPUT_FREQUENCY);
    spinOutputFrequency->setMaximum(R_TIME_MAX_OUTPUT_FREQUENCY);
    spinOutputFrequency->setValue(this->timeSolver.getOutputFrequency());
    spinOutputFrequency->setToolTip(tr("Number of time-steps after which results will be written to a file."));
    groupLayout->addWidget(spinOutputFrequency,groupLayoutRow++,1);

    this->connect(spinOutputFrequency,SIGNAL(valueChanged(int)),SLOT(onOutputFrequencyChanged(int)));
}

double TimeSolverSetupWidget::findEndTime(void) const
{
    return this->timeSolver.getInputStartTime() + this->timeSolver.getInputTimeStepSize() * this->timeSolver.getInputNTimeSteps();
}

void TimeSolverSetupWidget::onTimeSolverEnabledChanged(bool checked)
{
    this->timeSolver.setEnabled(checked);
    emit this->changed(this->timeSolver);
}

void TimeSolverSetupWidget::onTimeApproximationChanged(int index)
{
    RTimeMarchApproximation tma = RTimeMarchApproximation(index);
    if (R_TIME_MARCH_APPROXIMATION_TYPE_IS_VALID(tma))
    {
        this->timeSolver.setTimeMarchApproximation(tma);
        emit this->changed(this->timeSolver);
    }
}

void TimeSolverSetupWidget::onStartTimeChanged(double startTime)
{
    this->timeSolver.setInputStartTime(startTime);
    this->lineEndTime->setValue(this->findEndTime());
    emit this->changed(this->timeSolver);
}

void TimeSolverSetupWidget::onTimeStepSizeChanged(double timeStepSize)
{
    this->timeSolver.setInputTimeStepSize(timeStepSize);
    this->lineEndTime->setValue(this->findEndTime());
    emit this->changed(this->timeSolver);
}

void TimeSolverSetupWidget::onNTimeStepsChanged(int nTimeSteps)
{
    this->timeSolver.setInputNTimeSteps(nTimeSteps);
    this->lineEndTime->setValue(this->findEndTime());
    emit this->changed(this->timeSolver);
}

void TimeSolverSetupWidget::onOutputFrequencyChanged(int outputFrequency)
{
    this->timeSolver.setOutputFrequency(outputFrequency);
    this->lineEndTime->setValue(this->findEndTime());
    emit this->changed(this->timeSolver);
}
