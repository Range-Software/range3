/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   matrix_solver_config_dialog.cpp                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   1-st February 2013                                       *
 *                                                                   *
 *  DESCRIPTION: Matrix solver config. dialog class definition       *
 *********************************************************************/

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "session.h"
#include "matrix_solver_config_dialog.h"

MatrixSolverConfigDialog::MatrixSolverConfigDialog(uint modelID, QWidget *parent) :
    QDialog(parent),
    modelID(modelID)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Matrix solver configuration"));

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    RMatrixSolverConf &solverConf = Session::getInstance().getModel(this->modelID).getMatrixSolverConf();

    uint rowCount = 0;

    QLabel *labelNIterations = new QLabel(tr("Number of iterations:"));
    mainLayout->addWidget(labelNIterations, rowCount, 0, 1, 1);

    this->spinNIterations = new QSpinBox;
    this->spinNIterations->setRange(1,INT_MAX);
    this->spinNIterations->setValue(solverConf.getNOuterIterations());
    mainLayout->addWidget(this->spinNIterations, rowCount, 1, 1, 1);

    rowCount ++;

    QLabel *labelCvgValue = new QLabel(tr("Convergence value:"));
    mainLayout->addWidget(labelCvgValue, rowCount, 0, 1, 1);

    this->editCvgValue = new ValueLineEdit(0.0,1.0);
    this->editCvgValue->setValue(solverConf.getSolverCvgValue());
    mainLayout->addWidget(this->editCvgValue, rowCount, 1, 1, 1);

    rowCount ++;

    QLabel *labelOutputFrequency = new QLabel(tr("Solver output frequency:"));
    mainLayout->addWidget(labelOutputFrequency, rowCount, 0, 1, 1);

    this->spinOutputFrequency = new QSpinBox;
    this->spinOutputFrequency->setRange(0,INT_MAX);
    this->spinOutputFrequency->setValue(solverConf.getOutputFrequency());
    mainLayout->addWidget(this->spinOutputFrequency, rowCount, 1, 1, 1);

    rowCount ++;

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, rowCount++, 0, 1, 2);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&MatrixSolverConfigDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&MatrixSolverConfigDialog::accept);
}

int MatrixSolverConfigDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        RMatrixSolverConf &solverConf = Session::getInstance().getModel(this->modelID).getMatrixSolverConf();

        solverConf.setNOuterIterations(this->spinNIterations->value());
        solverConf.setSolverCvgValue(this->editCvgValue->getValue());
        solverConf.setOutputFrequency(this->spinOutputFrequency->value());
    }

    return retVal;
}
