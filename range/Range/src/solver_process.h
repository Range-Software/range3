/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   solver_process.h                                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   10-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Solver process class declaration                    *
 *********************************************************************/

#ifndef SOLVER_PROCESS_H
#define SOLVER_PROCESS_H

#include <QProcess>

class SolverProcess : public QProcess
{
    Q_OBJECT

    public:

        //! Constructor.
        explicit SolverProcess(QObject *parent = nullptr);

    signals:

    public slots:

};

#endif // SOLVER_PROCESS_H
