/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   application.cpp                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   12-th July 2013                                          *
 *                                                                   *
 *  DESCRIPTION: Application class definition                        *
 *********************************************************************/

#include <QTimer>

#include "application.h"

Application::Application(int &argc, char **argv) :
    QCoreApplication(argc,argv)
{
    // Needed for printf functions family to work correctly.
    setlocale(LC_ALL,"C");
    QTimer::singleShot(0, this, SIGNAL(started()));
}
