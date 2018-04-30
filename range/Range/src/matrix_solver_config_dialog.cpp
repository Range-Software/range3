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

#include <rmlib.h>

#include "session.h"
#include "matrix_solver_config_dialog.h"

MatrixSolverConfigDialog::MatrixSolverConfigDialog(uint modelID, QWidget *parent) :
    QDialog(parent),
    modelID(modelID)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Matrix solver configuration"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    // CG SOLVER
    RMatrixSolverConf &solverConfCG = Session::getInstance().getModel(this->modelID).getMatrixSolverConf(RMatrixSolverConf::CG);

    this->groupCG = new QGroupBox(RMatrixSolverConf::getName(RMatrixSolverConf::CG));
    mainLayout->addWidget(this->groupCG);

    QGridLayout *cgLayout = new QGridLayout;
    this->groupCG->setLayout(cgLayout);

    uint cgRowCount = 0;

    QLabel *labelCGNIterations = new QLabel(tr("Number of iterations:"));
    cgLayout->addWidget(labelCGNIterations, cgRowCount, 0, 1, 1);

    this->spinCGNIterations = new QSpinBox;
    this->spinCGNIterations->setRange(1,INT_MAX);
    this->spinCGNIterations->setValue(solverConfCG.getNOuterIterations());
    cgLayout->addWidget(this->spinCGNIterations, cgRowCount, 1, 1, 1);

    cgRowCount ++;

    QLabel *labelCGCvgValue = new QLabel(tr("Convergence value:"));
    cgLayout->addWidget(labelCGCvgValue, cgRowCount, 0, 1, 1);

    this->editCGCvgValue = new ValueLineEdit(0.0,1.0);
    this->editCGCvgValue->setValue(solverConfCG.getSolverCvgValue());
    cgLayout->addWidget(this->editCGCvgValue, cgRowCount, 1, 1, 1);

    cgRowCount ++;

    QLabel *labelCGOutputFrequency = new QLabel(tr("Solver output frequency:"));
    cgLayout->addWidget(labelCGOutputFrequency, cgRowCount, 0, 1, 1);

    this->spinCGOutputFrequency = new QSpinBox;
    this->spinCGOutputFrequency->setRange(0,INT_MAX);
    this->spinCGOutputFrequency->setValue(solverConfCG.getOutputFrequency());
    cgLayout->addWidget(this->spinCGOutputFrequency, cgRowCount, 1, 1, 1);

    cgRowCount ++;

    // GMRES SOLVER
    RMatrixSolverConf &solverConfGMRES = Session::getInstance().getModel(this->modelID).getMatrixSolverConf(RMatrixSolverConf::GMRES);

    this->groupGMRES = new QGroupBox(RMatrixSolverConf::getName(RMatrixSolverConf::GMRES));
    mainLayout->addWidget(this->groupGMRES);

    QGridLayout *gmresLayout = new QGridLayout;
    this->groupGMRES->setLayout(gmresLayout);

    uint gmresRowCount = 0;

    QLabel *labelGMRESNInnerIterations = new QLabel(tr("Number of inner iterations:"));
    gmresLayout->addWidget(labelGMRESNInnerIterations, gmresRowCount, 0, 1, 1);

    this->spinGMRESNInnerIterations = new QSpinBox;
    this->spinGMRESNInnerIterations->setRange(1,INT_MAX);
    this->spinGMRESNInnerIterations->setValue(solverConfGMRES.getNInnerIterations());
    gmresLayout->addWidget(this->spinGMRESNInnerIterations, gmresRowCount, 1, 1, 1);

    gmresRowCount ++;

    QLabel *labelGMRESNOuterIterations = new QLabel(tr("Number of outer iterations:"));
    gmresLayout->addWidget(labelGMRESNOuterIterations, gmresRowCount, 0, 1, 1);

    this->spinGMRESNOuterIterations = new QSpinBox;
    this->spinGMRESNOuterIterations->setRange(1,INT_MAX);
    this->spinGMRESNOuterIterations->setValue(solverConfGMRES.getNOuterIterations());
    gmresLayout->addWidget(this->spinGMRESNOuterIterations, gmresRowCount, 1, 1, 1);

    gmresRowCount ++;

    QLabel *labelGMRESCvgValue = new QLabel(tr("Convergence value:"));
    gmresLayout->addWidget(labelGMRESCvgValue, gmresRowCount, 0, 1, 1);

    this->editGMRESCvgValue = new ValueLineEdit(0.0,1.0);
    this->editGMRESCvgValue->setValue(solverConfGMRES.getSolverCvgValue());
    gmresLayout->addWidget(this->editGMRESCvgValue, gmresRowCount, 1, 1, 1);

    gmresRowCount ++;

    QLabel *labelGMRESOutputFrequency = new QLabel(tr("Solver output frequency:"));
    gmresLayout->addWidget(labelGMRESOutputFrequency, gmresRowCount, 0, 1, 1);

    this->spinGMRESOutputFrequency = new QSpinBox;
    this->spinGMRESOutputFrequency->setRange(0,INT_MAX);
    this->spinGMRESOutputFrequency->setValue(solverConfGMRES.getOutputFrequency());
    gmresLayout->addWidget(this->spinGMRESOutputFrequency, gmresRowCount, 1, 1, 1);

    gmresRowCount ++;

    // Button layout

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);

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
        RMatrixSolverConf &solverConfCG = Session::getInstance().getModel(this->modelID).getMatrixSolverConf(RMatrixSolverConf::CG);

        solverConfCG.setNOuterIterations(this->spinCGNIterations->value());
        solverConfCG.setSolverCvgValue(this->editCGCvgValue->getValue());
        solverConfCG.setOutputFrequency(this->spinCGOutputFrequency->value());

        RMatrixSolverConf &solverConfGMRES = Session::getInstance().getModel(this->modelID).getMatrixSolverConf(RMatrixSolverConf::GMRES);

        solverConfGMRES.setNInnerIterations(this->spinGMRESNInnerIterations->value());
        solverConfGMRES.setNOuterIterations(this->spinGMRESNOuterIterations->value());
        solverConfGMRES.setSolverCvgValue(this->editGMRESCvgValue->getValue());
        solverConfGMRES.setOutputFrequency(this->spinGMRESOutputFrequency->value());
    }

    return retVal;
}
