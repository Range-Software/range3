/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   progress.cpp                                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   13-1h January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Progress functions                                  *
 *********************************************************************/

#include <string>
#include <rblib.h>

#include <qmath.h>

#include "progress.h"

static void progressPrintHandler (double fraction);

static void progressInitializeHandler (const QString &message,
                                       bool           pulseType);

static void progressFinalizeHandler (const QString &message);

void Progress::initialize (void)
{
    RProgress::getInstance().setProgressInitializeHandler(progressInitializeHandler);
    RProgress::getInstance().setProgressFinalizeHandler(progressFinalizeHandler);
    RProgress::getInstance().setProgressPrintHandler(progressPrintHandler);
}

Progress & Progress::getInstance(void)
{
    static Progress progress;
    return progress;
}

void Progress::print(double fraction)
{
    emit this->progress(fraction);
}

void Progress::emitProgressInitialize(const QString &message,
                                      bool           pulseType)
{
    emit this->progressInitialize(message,pulseType);
}

void Progress::emitProgressFinalize(const QString &message)
{
    emit this->progressFinalize(message);
}

static void progressPrintHandler (double fraction)
{
    static double lastValue = 0.0;
    if (qAbs(qFloor(fraction*100) - lastValue) >= 1)
    {
        lastValue = qFloor(fraction*100);
        Progress::getInstance().print(fraction);
    }
}

static void progressInitializeHandler (const QString &message,
                                       bool           pulseType)
{
    Progress::getInstance().emitProgressInitialize(message,pulseType);
}

static void progressFinalizeHandler (const QString &message)
{
    Progress::getInstance().emitProgressFinalize(message);
}
