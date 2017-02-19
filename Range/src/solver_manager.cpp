/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   solver_manager.cpp                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   10-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Solver manager class definition                     *
 *********************************************************************/

#include <rblib.h>

#include "session.h"
#include "solver_manager.h"

SolverManager::SolverManager(QObject *parent) :
    JobManager(parent)
{
}

SolverManager &SolverManager::getInstance(void)
{
    static SolverManager solverManager;
    return solverManager;
}

void SolverManager::submit(SolverTask *solverTask)
{
    if (!this->isSolverQueued(solverTask->getModelID()))
    {
        QObject::connect(solverTask,
                         &SolverTask::readyReadStandardOutput,
                         this,
                         &SolverManager::onReadyReadStandardOutput);
        QObject::connect(solverTask,
                         &SolverTask::readyReadStandardError,
                         this,
                         &SolverManager::onReadyReadStandardError);

        this->lastLogFileName = solverTask->getLogFileName();

        this->JobManager::submit(solverTask);
    }
}

bool SolverManager::isSolverQueued(uint modelID) const
{
    for (int i=0;i<this->waitingJobs.size();i++)
    {
        SolverTask *solverTask = static_cast<SolverTask*>(this->waitingJobs[i]);
        if (solverTask->getModelID() == modelID)
        {
            return true;
        }
    }
    return false;
}

void SolverManager::stopRunningTasks(void)
{
    if (!this->runningJobs.isEmpty())
    {
        foreach (Job *task, this->runningJobs)
        {
            ((SolverTask *)task)->stop();
        }
    }
}

void SolverManager::killRunningTasks(void)
{
    if (!this->runningJobs.isEmpty())
    {
        foreach (Job *task, this->runningJobs)
        {
            ((SolverTask *)task)->kill();
        }
    }
}

const QString &SolverManager::getLastLogFileName(void) const
{
    return this->lastLogFileName;
}

void SolverManager::onReadyReadStandardOutput(const QString &message)
{
    emit this->readyReadStandardOutput(message);
}

void SolverManager::onReadyReadStandardError(const QString &message)
{
    emit this->readyReadStandardError(message);
}
