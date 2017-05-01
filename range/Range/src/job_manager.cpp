/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   job_manager.cpp                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   9-th January 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Job manager class definition                        *
 *********************************************************************/

#include <QVector>

#include <rblib.h>

#include "job_manager.h"

JobManager::JobManager(QObject *parent)
    : QObject(parent)
    , jobIsStarting(false)
{
}

JobManager & JobManager::getInstance(void)
{
    static JobManager jobManager;
    return jobManager;
}

void JobManager::submit(Job *job)
{
    this->waitingJobs.enqueue(job);
    this->processWaitingJobs();
    this->processFinishedJobs();
}

uint JobManager::getNWaiting(void) const
{
    return (uint)this->waitingJobs.size();
}

uint JobManager::getNRunning(void) const
{
    QList<Job*>::const_iterator iter;
    uint nRunning = 0;

    for (iter = this->runningJobs.begin();
         iter != this->runningJobs.end();
         ++iter)
    {
        if (!(*iter)->isFinished())
        {
            nRunning ++;
        }
    }
    return nRunning;
}

QVector<uint> JobManager::getRunningIDs(void) const
{
    QVector<uint> runningIDs;

    QList<Job*>::const_iterator iter;
    for (iter = this->runningJobs.begin();
         iter != this->runningJobs.end();
         ++iter)
    {
        if (!(*iter)->isFinished())
        {
            runningIDs.push_back((*iter)->getID());
        }
    }

    return runningIDs;
}

void JobManager::onJobBlocking(bool blocking)
{
    emit this->jobBlocking(blocking);
}

void JobManager::onJobStarted(void)
{
    this->jobIsStarting = false;
    emit this->jobStarted();
}

void JobManager::onJobFinished(void)
{
    emit this->jobFinished();
    this->processWaitingJobs();
    this->processFinishedJobs();
}

void JobManager::processWaitingJobs(void)
{
    if (this->getNRunning() == 0 && this->getNWaiting() > 0 && !this->jobIsStarting)
    {
        this->startJob(this->waitingJobs.dequeue());
    }
}

void JobManager::processFinishedJobs(void)
{
    this->removeFinishedJobs();
}

void JobManager::startJob(Job *job)
{
    this->jobIsStarting = true;

    job->moveToThread(&this->runningThread);

    QObject::connect(job,&Job::isBlocking,this,&JobManager::onJobBlocking);
    QObject::connect(&this->runningThread,&QThread::started,this,&JobManager::onJobStarted);
    QObject::connect(&this->runningThread,&QThread::started,job,&Job::process);
    QObject::connect(job,&Job::finished,&this->runningThread,&QThread::quit);
    QObject::connect(&this->runningThread,&QThread::finished,this,&JobManager::onJobFinished);

    this->runningJobs.append(job);

    this->runningThread.start();
}

Job * JobManager::findFinishedJob(void) const
{
    QList<Job*>::const_iterator iter;
    for (iter = this->runningJobs.begin();
         iter != this->runningJobs.end();
         ++iter)
    {
        if ((*iter)->isFinished())
        {
            return (*iter);
        }
    }
    return 0;
}

uint JobManager::removeFinishedJobs()
{
    Job *job;
    uint nRemovedTotal = 0;
    int nRemoved = 0;

    while ((job = this->findFinishedJob()) != 0)
    {
        nRemoved = this->runningJobs.removeAll(job);
        if (job->getAutoDelete())
        {
            delete job;
        }
        if (nRemoved == 0)
        {
            RLogger::error("No job could be found in the running queue.\n");
        }
        else if (nRemoved > 1)
        {
            RLogger::error("Multiple occurences of one jobe wre found in the running queue.\n");
            nRemovedTotal += (uint) nRemoved;
        }
        else if (nRemoved < 0)
        {
            RLogger::error("Unknown error while removing the job from the running queue.\n");
        }
    }

    return nRemovedTotal;
}
