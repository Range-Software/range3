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

int main (int argc, char *argv[])
{
    QLocale::setDefault(QLocale("en_US"));

    if (int exitValue = Application(argc, argv).exec() != 0)
    {
        RLogger::info ("Application has terminated with error code (%d).\n", exitValue);
        return exitValue;
    }
    return 0;
} /* main */
