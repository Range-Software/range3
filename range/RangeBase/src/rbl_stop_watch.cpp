/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_stop_watch.cpp                                       *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   30-th December 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Stop-watch definition                               *
 *********************************************************************/

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
    this->startTime = std::clock();
    this->pauseTime = this->startTime;
}

void RStopWatch::pause(void)
{
    this->pauseTime = std::clock();
}

void RStopWatch::resume(void)
{
    this->startTime += std::clock() - this->pauseTime;
}

clock_t RStopWatch::getClock(void) const
{
    return std::clock() - this->startTime;
}

unsigned int RStopWatch::getMiliSeconds(void) const
{
    return (unsigned int)(std::ceil((1000.0 * this->getClock()) / CLOCKS_PER_SEC));
}

unsigned int RStopWatch::getSeconds(void) const
{
    return (unsigned int)(std::ceil(double(this->getClock()) / CLOCKS_PER_SEC));
}

void RStopWatch::addElapsedTime(clock_t elapsedTime)
{
    this->startTime -= elapsedTime;
}

