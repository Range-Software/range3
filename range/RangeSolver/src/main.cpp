/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   main.cpp                                                 *
 *  GROUP:  RangeSolver                                              *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   12-th July 2013                                          *
 *                                                                   *
 *  DESCRIPTION: Main function                                       *
 *********************************************************************/

#include <QLocale>

#include <rblib.h>

#include "application.h"

int main(int argc, char *argv[])
{
    RArgumentsParser::printHeader("Solver");

    QLocale::setDefault(QLocale::c());

    int exitValue = 0;
    if ((exitValue = Application(argc, argv).exec()) != 0)
    {
        RLogger::info("Application has terminated with error code (%d).\n", exitValue);
    }

    RArgumentsParser::printFooter();
    return exitValue;
} /* main */
