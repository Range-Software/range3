/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_locker.cpp                                           *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   15-th February 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Locker class definition                             *
 *********************************************************************/

#include "rbl_locker.h"


RLocker::RLocker () : lockHandler(0)
{
    this->_init();
} /* RLocker::RLocker */


RLocker::RLocker (const RLocker &locker)
{
    this->_init(&locker);
} /* RLocker::RLocker (copy) */


RLocker::~RLocker ()
{
} /* RLocker::~RLocker */


RLocker & RLocker::operator = (const RLocker &locker)
{
    this->_init(&locker);
    return (*this);
} /* RLocker::operator = */


void RLocker::_init( const RLocker *pLocker)
{
    if (pLocker)
    {
        this->setLockHandler (pLocker->getLockHandler());
    }
} /* RLocker:_init */


RLocker & RLocker::getInstance (void)
{
    static RLocker locker;
    return locker;
} /* RLocker:getInstance */


RLockHandler RLocker::getLockHandler (void) const
{
    return this->lockHandler;
} /* RLocker:getLockHandler */


void RLocker::setLockHandler (RLockHandler lockHandler)
{
    this->lockHandler = lockHandler;
} /* RLocker:setLockHandler */


void RLocker::setLock (bool lock)
{
    if (this->lockHandler)
    {
        this->lockHandler(lock);
    }
    else
    {
        if (lock)
        {
            this->mutex.lock();
        }
        else
        {
            this->mutex.unlock();
        }
    }
} /* RLocker:setLock */


void RLocker::lock(void)
{
    RLocker::getInstance().setLock(true);
} /* RLocker:lock */


void RLocker::unlock(void)
{
    RLocker::getInstance().setLock(false);
} /* RLocker:unlock */
