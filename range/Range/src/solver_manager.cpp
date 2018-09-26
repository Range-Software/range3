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

const QString solverTaskServerName("RANGE_SOLVER_TASK_SERVER");

SolverManager::SolverManager(QObject *parent)
    : JobManager(parent)
    , taskServer(0)
{
    this->taskServerName = "RANGE_SOLVER_TASK_SERVER_" + Session::getInstance().getID();
    this->taskServer = new QLocalServer(this);
    if (!this->taskServer->listen(this->taskServerName))
    {
        RLogger::warning("Local server failed to listen: %s\n", this->taskServer->errorString().toUtf8().constData());
    }
    RLogger::info("Listening on: %s\n", this->taskServer->serverName().toUtf8().constData());
    QObject::connect(this->taskServer, &QLocalServer::newConnection, this, &SolverManager::onSolverTaskNewConnection);
}

SolverManager &SolverManager::getInstance(void)
{
    static SolverManager solverManager;
    return solverManager;
}

const QString &SolverManager::getTaskServerName(void) const
{
    return this->taskServerName;
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
            SolverTask *solverTask = dynamic_cast<SolverTask*>(task);
            RLogger::info("Stopping solver task (#%s).\n",solverTask->getTaskID().toString().toUtf8().constData());
            QString message = solverTask->getStopCommand();
            foreach (QLocalSocket *localSocket, this->taskClients)
            {
                QByteArray block;
                QDataStream out(&block, QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_5_6);
                RLogger::info("Sendig signal: %s\n",message.toUtf8().constData());
                out << quint32(message.size());
                out << message;
                localSocket->write(block);
                localSocket->flush();
            }
        }
    }
}

void SolverManager::killRunningTasks(void)
{
    if (!this->runningJobs.isEmpty())
    {
        foreach (Job *task, this->runningJobs)
        {
            SolverTask *solverTask = dynamic_cast<SolverTask*>(task);
            RLogger::info("Killing solver task (#%s).\n",solverTask->getTaskID().toString().toUtf8().constData());
            solverTask->kill();
        }
    }
}

const QString &SolverManager::getLastLogFileName(void) const
{
    return this->lastLogFileName;
}

void SolverManager::stopServer(void)
{
    QObject::disconnect(this->taskServer, &QLocalServer::newConnection, this, &SolverManager::onSolverTaskNewConnection);
    this->taskServer->close();
}

void SolverManager::onReadyReadStandardOutput(const QString &message)
{
    emit this->readyReadStandardOutput(message);
}

void SolverManager::onReadyReadStandardError(const QString &message)
{
    emit this->readyReadStandardError(message);
}

void SolverManager::onSolverTaskNewConnection(void)
{
    RLogger::info("Task client connected\n");
    while (this->taskServer->hasPendingConnections())
    {
        this->taskClients.append(this->taskServer->nextPendingConnection());
    }
}
