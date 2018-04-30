/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   solver_process.cpp                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   10-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Solver process class definition                     *
 *********************************************************************/

#include <rmlib.h>

#include "solver_process.h"

SolverProcess::SolverProcess(QObject *parent) :
    QProcess(parent)
{
}
