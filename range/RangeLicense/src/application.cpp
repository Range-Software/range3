/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   application.cpp                                          *
 *  GROUP:  RangeLicense                                             *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   7-th September 2016                                      *
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
