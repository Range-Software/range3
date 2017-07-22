/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   main.cpp                                                 *
 *  GROUP:  RangeTestApp                                             *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   22-th July 2017                                          *
 *                                                                   *
 *  DESCRIPTION: Main function                                       *
 *********************************************************************/

#include <QTimer>

#include <rmlib.h>

#include "application.h"
#include "main_task.h"

int main(int argc, char *argv[])
{
    int exitValue = 0;
    Application app(argc, argv);

    MainTask *mainTask = new MainTask(&app);
    QTimer::singleShot(0, mainTask, SLOT(run()));

    if ((exitValue = app.exec()) != 0)
    {
        RLogger::info("Application has terminated with error code (%d).\n", exitValue);
    }

    return exitValue;
}
