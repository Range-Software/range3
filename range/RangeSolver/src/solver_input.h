/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   solver_input.h                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th February 2014                                       *
 *                                                                   *
 *  DESCRIPTION: Solver input class declaration                      *
 *********************************************************************/

#ifndef __SOLVER_INPUT_H__
#define __SOLVER_INPUT_H__

#include <QString>

class SolverTask;

class SolverInput
{

    protected:

        //! Model file.
        QString modelFileName;
        //! License file.
        QString moduleLicenseFileName;
        //! Range account.
        QString account;
        //! Range password.
        QString password;
        //! Convergence file.
        QString convergenceFileName;
        //! Monitoring file.
        QString monitoringFileName;
        //! Number of threads to use.
        uint nThreads;
        //! Force to restart solver
        bool restart;

    private:

        //! Internal initialization function.
        void _init(const SolverInput *pSolverInput = 0);

    public:

        //! Constructor.
        SolverInput(const QString &modelFileName,
                    const QString &moduleLicenseFileName,
                    const QString &account,
                    const QString &password);

        //! Copy constructor.
        SolverInput(const SolverInput &solverInput);

        //! Destructor.
        ~SolverInput();

        //! Assignment operator.
        SolverInput & operator =(const SolverInput &solverInput);

        //! Set convergence file name.
        void setConvergenceFileName(const QString &convergenceFileName);

        //! Set monitoring file name.
        void setMonitoringFileName(const QString &monitoringFileName);

        //! Set number of threads to use.
        void setNThreads(uint nThreads);

        //! Set force restart solver.
        void setRestart(bool restart);

        friend class SolverTask;

};

#endif // __SOLVER_INPUT_H__
