/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_matrix_solver.cpp                                    *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   16-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Matrix Solver class definition                      *
 *********************************************************************/

#include <rblib.h>

#include "rml_matrix_solver_conf.h"

typedef struct _RMatrixSolverDesc
{
    QString name;
    QString id;
} RMatrixSolverDesc;

static RMatrixSolverDesc matrixSolverDesc [] =
{
    { "None", "NONE" },
    { "Conjugate Gradient", "mxs-CG" },
    { "Generalized Minimal Residual", "mxs-GMRES" }
};

void RMatrixSolverConf::_init(const RMatrixSolverConf *pMatrixSolver)
{
    if (pMatrixSolver)
    {
        this->type = pMatrixSolver->type;
        this->nInnerIterations = pMatrixSolver->nInnerIterations;
        this->nOuterIterations = pMatrixSolver->nOuterIterations;
        this->solverCvgValue = pMatrixSolver->solverCvgValue;
        this->outputFrequency = pMatrixSolver->outputFrequency;
        this->outputFileName = pMatrixSolver->outputFileName;
    }
}

RMatrixSolverConf::RMatrixSolverConf(RMatrixSolverType type)
    : type(type)
    , nInnerIterations(10)
    , nOuterIterations(1000)
    , solverCvgValue(RConstants::eps)
    , outputFrequency(100)
{
    switch (this->type)
    {
        case CG:
        {
            nInnerIterations = 10;
            nOuterIterations = 1000;
            break;
        }
        case GMRES:
        {
            nInnerIterations = 10;
            nOuterIterations = 10;
            break;
        }
    }
    this->_init();
}

RMatrixSolverConf::RMatrixSolverConf(const RMatrixSolverConf &matrixSolver)
{
    this->_init(&matrixSolver);
}

RMatrixSolverConf::~RMatrixSolverConf()
{
}

RMatrixSolverConf &RMatrixSolverConf::operator =(const RMatrixSolverConf &matrixSolver)
{
    this->_init(&matrixSolver);
    return (*this);
}

RMatrixSolverType RMatrixSolverConf::getType(void) const
{
    return this->type;
}

void RMatrixSolverConf::setType(RMatrixSolverType type)
{
    this->type = type;
}

unsigned int RMatrixSolverConf::getNInnerIterations(void) const
{
    return this->nInnerIterations;
}

void RMatrixSolverConf::setNInnerIterations(unsigned int nInnerIterations)
{
    this->nInnerIterations = nInnerIterations;
}

unsigned int RMatrixSolverConf::getNOuterIterations(void) const
{
    return this->nOuterIterations;
}

void RMatrixSolverConf::setNOuterIterations(unsigned int nOuterIterations)
{
    this->nOuterIterations = nOuterIterations;
}

double RMatrixSolverConf::getSolverCvgValue(void) const
{
    return this->solverCvgValue;
}

void RMatrixSolverConf::setSolverCvgValue(double solverCvgValue)
{
    this->solverCvgValue = solverCvgValue;
}

unsigned int RMatrixSolverConf::getOutputFrequency(void) const
{
    return this->outputFrequency;
}

void RMatrixSolverConf::setOutputFrequency(unsigned int outputFrequency)
{
    this->outputFrequency = outputFrequency;
}

const QString &RMatrixSolverConf::getOutputFileName(void) const
{
    return this->outputFileName;
}

void RMatrixSolverConf::setOutputFileName(const QString &outputFileName)
{
    this->outputFileName = outputFileName;
}

const QString &RMatrixSolverConf::getName(RMatrixSolverType type)
{
    return matrixSolverDesc[type].name;
}

const QString &RMatrixSolverConf::getId(RMatrixSolverType type)
{
    return matrixSolverDesc[type].id;
}
