/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   application.cpp                                          *
 *  GROUP:  RangeTestApp                                             *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   22-th July 2017                                          *
 *                                                                   *
 *  DESCRIPTION: Application class definition                        *
 *********************************************************************/

#include <QTimer>

#include "application.h"

Application::Application(int &argc, char **argv) :
    QCoreApplication(argc,argv)
{
    QTimer::singleShot(0, this, SIGNAL(started()));
}
