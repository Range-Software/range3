/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   job.cpp                                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   9-th January 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Job class definition                                *
 *********************************************************************/

#include <omp.h>

#include "job.h"
#include "job_manager.h"
#include "main_settings.h"
#include "session.h"

static uint lastID = 0;

Job::Job(QObject *parent)
    : QObject(parent)
    , id(++lastID)
    , jobFinished(false)
    , autoDelete(true)
    , blocking(true)
{
    R_LOG_TRACE;
}

uint Job::getID(void)
{
    R_LOG_TRACE;
    return this->id;
}

bool Job::isFinished(void) const
{
    R_LOG_TRACE;
    return this->jobFinished;
}

bool Job::getAutoDelete(void) const
{
    R_LOG_TRACE;
    return this->autoDelete;
}

void Job::setAutoDelete(bool autoDelete)
{
    R_LOG_TRACE;
    this->autoDelete = autoDelete;
}

void Job::setBlocking(bool blocking)
{
    R_LOG_TRACE;
    this->blocking = blocking;
}

void Job::exec(void)
{
    R_LOG_TRACE_IN;
    Session::getInstance().lock();
    emit this->started();
    if (this->blocking)
    {
        emit this->isBlocking(true);
    }
    Session::getInstance().unlock();
    omp_set_num_threads(MainSettings::getInstance().getApplicationSettings()->getNThreads());
    this->run();
    Session::getInstance().lock();
    this->jobFinished = true;
    if (this->blocking)
    {
        emit this->isBlocking(false);
    }
    emit this->finished();
    Session::getInstance().unlock();
    R_LOG_TRACE_OUT;
}

void Job::process(void)
{
    R_LOG_TRACE_IN;
    this->exec();
    R_LOG_TRACE_OUT;
}

void Job::run(void)
{
    R_LOG_TRACE;
}
