/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   solver_task.h                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   25-th August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Solver task class declaration                       *
 *********************************************************************/

#ifndef __SOLVER_TASK_H__
#define __SOLVER_TASK_H__

#include <QObject>
#include <QCoreApplication>

#include "solver_input.h"

class SolverTask : public QObject
{

    Q_OBJECT

    protected:

        //! Model file.
        QString modelFileName;
        //! Convergence file name.
        QString convergenceFileName;
        //! Monitoring file name.
        QString monitoringFileName;
        //! Number of threads to use.
        uint nThreads;
        //! Force to restart solver
        bool restart;
        //! Pointer to application object.
        QCoreApplication *app;

    public:

        //! Constructor.
        explicit SolverTask(const SolverInput &solverInput, QCoreApplication *app, QObject *parent = 0);

    signals:

        //! Signal that solver has finished.
        void finished(void);
        
    public slots:

        //! Run task.
        void run(void);

};

#endif // __SOLVER_TASK_H__
