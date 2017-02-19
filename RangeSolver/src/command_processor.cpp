/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   command_processor.cpp                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   26-th August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Command processor class definition                  *
 *********************************************************************/

#include <QTextStream>

#include <unistd.h>

#include <rblib.h>

#include "command_processor.h"

CommandProcessor::CommandProcessor(QCoreApplication *application) :
    QObject(application),
    enabled(false)
{
    this->stdinNotifier = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read,this);

    QObject::connect(this->stdinNotifier,
                     &QSocketNotifier::activated,
                     this,
                     &CommandProcessor::readStdin);
}

void CommandProcessor::setEnabled(bool enabled)
{
    this->enabled = enabled;
}

void CommandProcessor::readStdin(int socket)
{
    if (!this->enabled)
    {
        return;
    }

    RLogger::info("Processing input.\n");

    QTextStream textStream(stdin,QIODevice::ReadOnly);

    QString line;

    line = textStream.readLine();
    RLogger::info("Received command: \'%s\'\n",line.toUtf8().constData());

    RApplicationState::getInstance().setStateType(R_APPLICATION_STATE_STOP);
}
