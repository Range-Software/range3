/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   locker.cpp                                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   15-rd February 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Locker functions                                    *
 *********************************************************************/

#include <rblib.h>

#include "locker.h"

static void lockHandler (bool lock);


void Locker::initialize(void)
{
    RLocker::getInstance().setLockHandler(lockHandler);
}

Locker & Locker::getInstance(void)
{
    static Locker locker;
    return locker;
}

QMutex * Locker::getMutexPtr(void)
{
    return &this->mutex;
}

static void lockHandler (bool lock)
{
    if (lock)
    {
        Locker::getInstance().getMutexPtr()->lock();
    }
    else
    {
        Locker::getInstance().getMutexPtr()->unlock();
    }
}
