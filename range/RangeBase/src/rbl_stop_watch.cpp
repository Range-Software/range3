/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_stop_watch.cpp                                       *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   30-th December 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Stop-watch definition                               *
 *********************************************************************/

#include <QDateTime>
#include <cmath>

#include "rbl_stop_watch.h"

void RStopWatch::_init(const RStopWatch *pStopWatch)
{
    if (pStopWatch)
    {
        this->startTime = pStopWatch->startTime;
        this->pauseTime = pStopWatch->pauseTime;
    }
}

RStopWatch::RStopWatch()
{
    this->_init();
}

RStopWatch::RStopWatch(const RStopWatch &stopWatch)
{
    this->_init(&stopWatch);
}

RStopWatch::~RStopWatch()
{

}

RStopWatch &RStopWatch::operator =(const RStopWatch &stopWatch)
{
    this->_init(&stopWatch);
    return (*this);
}

void RStopWatch::reset(void)
{
    this->startTime = QDateTime::currentMSecsSinceEpoch();
    this->pauseTime = this->startTime;
}

void RStopWatch::pause(void)
{
    this->pauseTime = QDateTime::currentMSecsSinceEpoch();
}

void RStopWatch::resume(void)
{
    this->startTime += QDateTime::currentMSecsSinceEpoch() - this->pauseTime;
}

qint64 RStopWatch::getMiliSeconds(void) const
{
    return QDateTime::currentMSecsSinceEpoch() - this->startTime;
}

void RStopWatch::addElapsedTime(qint64 elapsedTime)
{
    this->startTime -= elapsedTime;
}

