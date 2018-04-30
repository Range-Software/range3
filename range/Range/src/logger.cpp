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

void Logger::initialize(void)
{
    RLogger::getInstance().setLevel(R_LOG_LEVEL_DETAIL);
    RLogger::getInstance().setHalted(true);
    RLogger::getInstance().setLogHandler(Logger::logHandler);
}

void Logger::halt(void)
{
    RLogger::getInstance().setHalted(true);
}

void Logger::unhalt(void)
{
    RLogger::getInstance().setHalted(false);
}

Logger & Logger::getInstance()
{
    static Logger logger;
    return logger;
}

void Logger::logHandler(const RMessage &message)
{
    switch (message.getType())
    {
        case R_MESSAGE_INFO:
            emit Logger::getInstance().infoPrinted(message);
            break;
        case R_MESSAGE_NOTICE:
            emit Logger::getInstance().noticePrinted(message);
            break;
        case R_MESSAGE_WARNING:
            emit Logger::getInstance().warningPrinted(message);
            break;
        case R_MESSAGE_ERROR:
            emit Logger::getInstance().errorPrinted(message);
            break;
        case R_MESSAGE_DEBUG:
            emit Logger::getInstance().infoPrinted(message);
            break;
        case R_MESSAGE_TRACE:
            emit Logger::getInstance().infoPrinted(message);
            break;
        default:
            emit Logger::getInstance().errorPrinted("UNKNOWN MESSAGE TYPE: " + message);
            break;
    }
}
