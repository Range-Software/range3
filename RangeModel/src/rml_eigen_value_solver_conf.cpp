/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_eigen_value_solver_conf.cpp                          *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   1-st July 2015                                           *
 *                                                                   *
 *  DESCRIPTION: Eigen value solver class definition                 *
 *********************************************************************/


#include "rml_eigen_value_solver_conf.h"

typedef struct _REigenValueSolverDesc
{
    QString name;
} REigenValueSolverDesc;

static REigenValueSolverDesc eigenValueSolverDesc [] =
{
    { "None" },
    { "Lanczos" },
    { "Arnoldi" },
    { "Rayleigh" }
};


void REigenValueSolverConf::_init(const REigenValueSolverConf *pEigenValueSolverConf)
{
    if (pEigenValueSolverConf)
    {
        this->method = pEigenValueSolverConf->method;
        this->nIterations = pEigenValueSolverConf->nIterations;
        this->nEigenValues = pEigenValueSolverConf->nEigenValues;
        this->solverCvgValue = pEigenValueSolverConf->solverCvgValue;
        this->outputFrequency = pEigenValueSolverConf->outputFrequency;
    }
}

REigenValueSolverConf::REigenValueSolverConf(REigenValueSolverConf::Method method)
    : method(method)
{
    this->_init();
}

REigenValueSolverConf::REigenValueSolverConf(const REigenValueSolverConf &eigenValueSolverConf)
{
    this->_init(&eigenValueSolverConf);
}

REigenValueSolverConf::~REigenValueSolverConf()
{

}

REigenValueSolverConf &REigenValueSolverConf::operator =(const REigenValueSolverConf &eigenValueSolverConf)
{
    this->_init(&eigenValueSolverConf);
    return (*this);
}

REigenValueSolverConf::Method REigenValueSolverConf::getMethod(void) const
{
    return this->method;
}

void REigenValueSolverConf::setMethod(REigenValueSolverConf::Method method)
{
    this->method = method;
}

unsigned int REigenValueSolverConf::getNIterations(void) const
{
    return this->nIterations;
}

void REigenValueSolverConf::setNIterations(unsigned int nIterations)
{
    this->nIterations = nIterations;
}

unsigned int REigenValueSolverConf::getNEigenValues(void) const
{
    return this->nEigenValues;
}

void REigenValueSolverConf::setNEigenValues(unsigned int nEigenValues)
{
    this->nEigenValues = nEigenValues;
}

double REigenValueSolverConf::getSolverCvgValue(void) const
{
    return this->solverCvgValue;
}

void REigenValueSolverConf::setSolverCvgValue(double solverCvgValue)
{
    this->solverCvgValue = solverCvgValue;
}

unsigned int REigenValueSolverConf::getOutputFrequency(void) const
{
    return this->outputFrequency;
}

void REigenValueSolverConf::setOutputFrequency(unsigned int outputFrequency)
{
    this->outputFrequency = outputFrequency;
}

const QString &REigenValueSolverConf::getOutputFileName(void) const
{
    return this->outputFileName;
}

void REigenValueSolverConf::setOutputFileName(const QString &outputFileName)
{
    this->outputFileName = outputFileName;
}

const QString &REigenValueSolverConf::getName(REigenValueSolverConf::Method method)
{
    return eigenValueSolverDesc[method].name;
}
