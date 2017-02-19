/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   logger.cpp                                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   3-rd January 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Logger functions                                    *
 *********************************************************************/

#include <rblib.h>

#include "logger.h"

static void logHandler (const RMessage &message);

void Logger::initialize (void)
{
    RLogger::getInstance().setLevel(R_LOG_LEVEL_DETAIL);
    RLogger::getInstance().setHalted(true);
    RLogger::getInstance().setLogHandler(logHandler);
}

void Logger::halt (void)
{
    RLogger::getInstance().setHalted(true);
}

void Logger::unhalt (void)
{
    RLogger::getInstance().setHalted(false);
}

Logger & Logger::getInstance()
{
    static Logger logger;
    return logger;
}

void Logger::printInfo(const QString &message)
{
    emit this->infoPrinted(message);
}

void Logger::printWarning(const QString &message)
{
    emit this->warningPrinted(message);
}

void Logger::printError(const QString &message)
{
    emit this->errorPrinted(message);
}

static void logHandler (const RMessage &message)
{
    switch (message.getType())
    {
        case R_MESSAGE_INFO:
            emit Logger::getInstance().printInfo(message);
            break;
        case R_MESSAGE_WARNING:
            emit Logger::getInstance().printWarning(message);
            break;
        case R_MESSAGE_ERROR:
            emit Logger::getInstance().printError(message);
            break;
        case R_MESSAGE_DEBUG:
            emit Logger::getInstance().printInfo(message);
            break;
        case R_MESSAGE_TRACE:
            emit Logger::getInstance().printInfo(message);
            break;
        default:
            emit Logger::getInstance().printError("UNKNOWN MESSAGE TYPE: " + message);
            break;
    }
}
