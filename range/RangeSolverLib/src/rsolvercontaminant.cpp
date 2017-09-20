/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolvercontaminant.cpp                                   *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   28-th January 2016                                       *
 *                                                                   *
 *  DESCRIPTION: Contaminant solver class definition                 *
 *********************************************************************/

#include "rsolvercontaminant.h"

void RSolverContaminant::_init(const RSolverContaminant *pSolver)
{
    if (pSolver)
    {

    }
}

RSolverContaminant::RSolverContaminant(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData)
    : RSolverGeneric(pModel,modelFileName,convergenceFileName,sharedData)
{
    this->problemType = R_PROBLEM_CONTAMINANT;
    this->_init();
}

RSolverContaminant::RSolverContaminant(const RSolverContaminant &solver)
    : RSolverGeneric(solver)
{
    this->_init(&solver);
}

RSolverContaminant::~RSolverContaminant()
{

}

RSolverContaminant &RSolverContaminant::operator =(const RSolverContaminant &solver)
{
    RSolverGeneric::operator =(solver);
    this->_init(&solver);
    return (*this);
}

bool RSolverContaminant::hasConverged(void) const
{
    return true;
}

void RSolverContaminant::updateScales(void)
{

}

void RSolverContaminant::recover(void)
{
    this->recoverVariable(R_VARIABLE_PARTICLE_CONCENTRATION,R_VARIABLE_APPLY_NODE,this->pModel->getNNodes(),0,this->nodeConcentration,RVariable::getInitValue(R_VARIABLE_PARTICLE_CONCENTRATION));
}

void RSolverContaminant::prepare(void)
{
    RLogger::info("Building matrix system\n");
    RLogger::indent();

    bool abort = false;

    if (abort)
    {
        RLogger::unindent();
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to prepare matrix system.");
    }

    RLogger::unindent();
}

void RSolverContaminant::solve(void)
{

}

void RSolverContaminant::process(void)
{

}

void RSolverContaminant::store(void)
{

}

void RSolverContaminant::statistics(void)
{

}
