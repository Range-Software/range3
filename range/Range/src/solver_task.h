/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   solver_task.h                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   10-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Solver task class declaration                       *
 *********************************************************************/

#ifndef SOLVER_TASK_H
#define SOLVER_TASK_H

#include "application_settings.h"
#include "job.h"
#include "solver_process.h"
#include "solver_task_id.h"

class SolverTask : public Job
{

    Q_OBJECT

    protected:

        //! Application settings.
        const ApplicationSettings *applicationSettings;
        //! Model ID
        uint modelID;
        //! Solver task ID.
        SolverTaskID taskID;
        //! Solver process.
        SolverProcess *solverProcess;
        //! Solver executable.
        QString solverExecutable;
        //! Solver executable arguments.
        QStringList solverArguments;
        //! Model file name.
        QString modelFileName;
        //! Log file name.
        QString logFileName;
        //! Convergence file name.
        QString convergenceFileName;
        //! Monitoring file name.
        QString monitoringFileName;

    public:

        //! Constructor.
        explicit SolverTask(const ApplicationSettings *applicationSettings, uint modelID, QObject *parent = nullptr);

        //! Return model ID.
        uint getModelID(void) const;

        //! Return const reference to solver task ID.
        const SolverTaskID & getTaskID(void) const;

        //! Return reference to solver task ID.
        SolverTaskID & getTaskID(void);

        //! Return const reference to solver task ID.
        const QString getStopCommand(void) const;

        //! Kill task.
        void kill(void);

        //! Return log file name.
        const QString & getLogFileName(void) const;

        //! Return command line.
        QString getCommandLine(void) const;

    protected:

        //! Run job.
        void run(void);

    private slots:

        //! Catch ready read standard output signal.
        void onProcessReadyReadStandardOutput(void);

        //! Catch ready read standard error signal.
        void onProcessReadyReadStandardError(void);

    signals:

        //! Message on standard output is availabe.
        void readyReadStandardOutput(const QString &message);

        //! Message on standard error is availabe.
        void readyReadStandardError(const QString &message);
        
};

#endif // SOLVER_TASK_H
