/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   main.cpp                                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   28-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Main function                                       *
 *********************************************************************/

#include <rblib.h>

#include "application.h"

int main(int argc, char *argv[])
{
    RArgumentsParser::printHeader("GUI");

    QLocale::setDefault(QLocale::c());

    int exitValue = 0;
    if ((exitValue = Application(argc, argv).exec()) != 0)
    {
        RLogger::info("Application has terminated with error code (%d).\n", exitValue);
    }

    RArgumentsParser::printFooter();
    return exitValue;
} /* main */
