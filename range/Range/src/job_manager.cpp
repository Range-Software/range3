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
    R_LOG_TRACE;
}

JobManager & JobManager::getInstance(void)
{
    R_LOG_TRACE;
    static JobManager jobManager;
    return jobManager;
}

void JobManager::submit(Job *job)
{
    R_LOG_TRACE_IN;
    this->waitingJobs.enqueue(job);
    this->processWaitingJobs();
    this->processFinishedJobs();
    R_LOG_TRACE_OUT;
}

uint JobManager::getNWaiting(void) const
{
    R_LOG_TRACE;
    return (uint)this->waitingJobs.size();
}

uint JobManager::getNRunning(void) const
{
    R_LOG_TRACE_IN;
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
    R_LOG_TRACE_OUT;
    return nRunning;
}

QVector<uint> JobManager::getRunningIDs(void) const
{
    R_LOG_TRACE_IN;
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

    R_LOG_TRACE_OUT;
    return runningIDs;
}

void JobManager::onJobBlocking(bool blocking)
{
    R_LOG_TRACE_IN;
    emit this->jobBlocking(blocking);
    R_LOG_TRACE_OUT;
}

void JobManager::onJobStarted(void)
{
    R_LOG_TRACE_IN;
    this->jobIsStarting = false;
    emit this->jobStarted();
    R_LOG_TRACE_OUT;
}

void JobManager::onJobFinished(void)
{
    R_LOG_TRACE_IN;
    emit this->jobFinished();
    this->processWaitingJobs();
    this->processFinishedJobs();
    R_LOG_TRACE_OUT;
}

void JobManager::processWaitingJobs(void)
{
    R_LOG_TRACE_IN;
    if (this->getNRunning() == 0 && this->getNWaiting() > 0 && !this->jobIsStarting)
    {
        this->startJob(this->waitingJobs.dequeue());
    }
    R_LOG_TRACE_OUT;
}

void JobManager::processFinishedJobs(void)
{
    R_LOG_TRACE_IN;
    this->removeFinishedJobs();
    R_LOG_TRACE_OUT;
}

void JobManager::startJob(Job *job)
{
    R_LOG_TRACE_IN;
    this->jobIsStarting = true;

    job->moveToThread(&this->runningThread);

    QObject::connect(job,&Job::isBlocking,this,&JobManager::onJobBlocking);
    QObject::connect(&this->runningThread,&QThread::started,this,&JobManager::onJobStarted);
    QObject::connect(&this->runningThread,&QThread::started,job,&Job::process);
    QObject::connect(job,&Job::finished,&this->runningThread,&QThread::quit);
    QObject::connect(&this->runningThread,&QThread::finished,this,&JobManager::onJobFinished);

    this->runningJobs.append(job);

    this->runningThread.start();
    R_LOG_TRACE_OUT;
}

Job * JobManager::findFinishedJob(void) const
{
    R_LOG_TRACE_IN;
    QList<Job*>::const_iterator iter;
    for (iter = this->runningJobs.begin();
         iter != this->runningJobs.end();
         ++iter)
    {
        if ((*iter)->isFinished())
        {
            R_LOG_TRACE_OUT;
            return (*iter);
        }
    }
    R_LOG_TRACE_OUT;
    return 0;
}

uint JobManager::removeFinishedJobs()
{
    R_LOG_TRACE_IN;
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

    R_LOG_TRACE_OUT;
    return nRemovedTotal;
}
