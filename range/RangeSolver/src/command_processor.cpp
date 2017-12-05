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

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <rblib.h>

#include "command_processor.h"

CommandProcessor::CommandProcessor(QCoreApplication *application) :
    QObject(application),
    enabled(false)
{
#ifdef _WIN32
    this->stdinNotifier = new QWinEventNotifier(GetStdHandle(STD_INPUT_HANDLE),this);
    this->connect(this->stdinNotifier,SIGNAL(activated(HANDLE)),this,SLOT(readStdin(HANDLE)));
    this->stdinNotifier->setEnabled(true);
#else
    this->stdinNotifier = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read,this);

    QObject::connect(this->stdinNotifier,
                     &QSocketNotifier::activated,
                     this,
                     &CommandProcessor::readStdin);
#endif
}

void CommandProcessor::setEnabled(bool enabled)
{
    this->enabled = enabled;
}

void CommandProcessor::readStdin(HANDLE socket)
{
    if (!this->enabled)
    {
        return;
    }

    this->disconnect(this->stdinNotifier,SIGNAL(activated(HANDLE)),this,SLOT(readStdin(HANDLE)));

    QTextStream textStream(stdin,QIODevice::ReadOnly);
    QString line = textStream.readLine();
    RLogger::info("Received command: \'%s\'\n",line.toUtf8().constData());
    if (line.contains("STOP"))
    {
        RApplicationState::getInstance().setStateType(R_APPLICATION_STATE_STOP);
    }
    else
    {
        this->connect(this->stdinNotifier,SIGNAL(activated(HANDLE)),this,SLOT(readStdin(HANDLE)));
    }
}
