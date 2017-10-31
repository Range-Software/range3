/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   solver_input.cpp                                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   5-th February 2014                                       *
 *                                                                   *
 *  DESCRIPTION: Solver input class definition                       *
 *********************************************************************/

#include "solver_input.h"

void SolverInput::_init(const SolverInput *pSolverInput)
{
    if (pSolverInput)
    {
        this->modelFileName = pSolverInput->modelFileName;
        this->convergenceFileName = pSolverInput->convergenceFileName;
        this->moduleLicenseFileName = pSolverInput->moduleLicenseFileName;
        this->account = pSolverInput->account;
        this->password = pSolverInput->password;
        this->restart = pSolverInput->restart;
    }
}

SolverInput::SolverInput(const QString &modelFileName,
                         const QString &moduleLicenseFileName,
                         const QString &account,
                         const QString &password)
    : modelFileName(modelFileName)
    , moduleLicenseFileName(moduleLicenseFileName)
    , account(account)
    , password(password)
    , restart(false)
{
    this->_init();
}

SolverInput::SolverInput(const SolverInput &solverInput)
{
    this->_init(&solverInput);
}

SolverInput::~SolverInput()
{

}

SolverInput &SolverInput::operator =(const SolverInput &solverInput)
{
    this->_init(&solverInput);
    return (*this);
}

void SolverInput::setConvergenceFileName(const QString &convergenceFileName)
{
    this->convergenceFileName = convergenceFileName;
}

void SolverInput::setMonitoringFileName(const QString &monitoringFileName)
{
    this->monitoringFileName = monitoringFileName;
}

void SolverInput::setNThreads(uint nThreads)
{
    this->nThreads = nThreads;
}

void SolverInput::setRestart(bool restart)
{
    this->restart = restart;
}
