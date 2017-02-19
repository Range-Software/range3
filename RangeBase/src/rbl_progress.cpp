/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_progress.cpp                                         *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   13-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Progress class definition                           *
 *********************************************************************/

#include <cstdlib>
#include <cmath>

#include "rbl_progress.h"
#include "rbl_logger.h"


RProgress::RProgress () : lastFraction(0.0)
                        , printToLog(false)
                        , progressHandler(0)
{
    this->_init();
} /* RProgress:: */


RProgress::RProgress (const RProgress &progress)
{
    this->_init(&progress);
} /* RProgress::RProgress (copy) */


RProgress::~RProgress ()
{
} /* RProgress::~RProgress */


RProgress & RProgress::operator = (const RProgress &progress)
{
    this->_init(&progress);
    return (*this);
} /* RProgress::operator = */


void RProgress::_init (const RProgress *pProgress)
{
    if (pProgress)
    {
        this->setPrintToLog(pProgress->getPrintToLog());
        this->setProgressPrintHandler(pProgress->getProgressPrintHandler());
        this->setProgressInitializeHandler(pProgress->getProgressInitializeHandler());
        this->setProgressFinalizeHandler(pProgress->getProgressFinalizeHandler());
    }
} /* RProgress::_init (const RProgress *pProgress) */


RProgress & RProgress::getInstance ()
{
    static RProgress progress;
    return progress;
} /* RProgress::getInstance */


bool RProgress::getPrintToLog (void) const
{
    return this->printToLog;
} /* RProgress::getPrintToLog */


void RProgress::setPrintToLog (bool printToLog)
{
    this->printToLog = printToLog;
} /* RProgress::setPrintToLog */


RProgressPrintHandler RProgress::getProgressPrintHandler (void) const
{
    return this->progressHandler;
} /* RProgress::getProgressHandler */


void RProgress::setProgressPrintHandler (RProgressPrintHandler progressHandler)
{
    this->progressHandler = progressHandler;
} /* RProgress::setProgressHandler */


RProgressInitializeHandler RProgress::getProgressInitializeHandler (void) const
{
    return this->progressInitializeHandler;
} /* RProgress::getProgressInitializeHandler */


void RProgress::setProgressInitializeHandler (RProgressInitializeHandler progressInitializeHandler)
{
    this->progressInitializeHandler = progressInitializeHandler;
} /* RProgress::setProgressInitializeHandler */


RProgressFinalizeHandler RProgress::getProgressFinalizeHandler (void) const
{
    return this->progressFinalizeHandler;
} /* RProgress::getProgressFinalizeHandler */


void RProgress::setProgressFinalizeHandler (RProgressFinalizeHandler progressFinalizeHandler)
{
    this->progressFinalizeHandler = progressFinalizeHandler;
} /* RProgress::setProgressFinalizeHandler */


void RProgress::initialize (const QString &message,
                            bool           pulseType)
{
    this->pulseType = pulseType;
    this->lastFraction = 0.0;
    this->print(0.0);
    if (this->printToLog)
    {
        bool addNewLine = RLogger::getInstance().getAddNewLine();
        RLogger::getInstance().setAddNewLine(false);
        RLogger::getInstance().print(message + " ");
        RLogger::getInstance().setAddNewLine(addNewLine);
    }
    if (this->progressInitializeHandler)
    {
        this->progressInitializeHandler(message,this->pulseType);
    }
} /* RProgress::initialize */


void RProgress::finalize (const QString &message)
{
    this->print(1.0);
    if (this->printToLog)
    {
        bool addNewLine = RLogger::getInstance().getAddNewLine();
        RLogger::getInstance().setAddNewLine(false);
        RLogger::getInstance().print(" " + message + "\n");
        RLogger::getInstance().setAddNewLine(addNewLine);
    }
    if (this->progressFinalizeHandler)
    {
        this->progressFinalizeHandler(message);
    }
} /* RProgress::finalize */


void RProgress::print (double fraction)
{
    if (this->printToLog)
    {
        bool addNewLine = RLogger::getInstance().getAddNewLine();
        RLogger::getInstance().setAddNewLine(false);
        if (this->pulseType)
        {
            uint indentLevel = RLogger::getInstance().getIndentLevel();
            RLogger::getInstance().setIndentLevel(0);
            RLogger::getInstance().print('.');
            RLogger::getInstance().setIndentLevel(indentLevel);
        }
        else
        {
            unsigned int nMarks = (unsigned int)floor(fraction * 10);
            unsigned int nLastMarks = (unsigned int)floor(this->lastFraction * 10);
            for (unsigned int i=nLastMarks;i<nMarks;i++)
            {
                uint indentLevel = RLogger::getInstance().getIndentLevel();
                RLogger::getInstance().setIndentLevel(0);
                RLogger::getInstance().print("#");
                RLogger::getInstance().setIndentLevel(indentLevel);
            }
            if (nMarks-nLastMarks >= 1)
            {
                this->lastFraction = fraction;
            }
        }
        RLogger::getInstance().setAddNewLine(addNewLine);
    }
    if (this->progressHandler)
    {
        this->progressHandler(fraction);
    }
} /* RProgress::print */


void RProgressInitialize (const QString &message,
                          bool               pulseType)
{
    RProgress::getInstance().initialize(message,pulseType);
} /* RProgressInitialize */


void RProgressFinalize (const QString &message)
{
    RProgress::getInstance().finalize(message);
} /* RProgressFinalize */


void RProgressPrint (double fraction)
{
    RProgress::getInstance().print(fraction);
} /* RProgressPrint */


void RProgressPrint (unsigned int step,
                     unsigned int nSteps)
{
    RProgress::getInstance().print((double)step/(double)nSteps);
} /* RProgressPrint */


void RProgressPrintToLog (bool printToLog)
{
    RProgress::getInstance().setPrintToLog(printToLog);
} /* RProgressPrintToLog */
