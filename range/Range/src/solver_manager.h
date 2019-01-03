/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   solver_manager.h                                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   10-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Solver manager class declaration                    *
 *********************************************************************/

#ifndef SOLVER_MANAGER_H
#define SOLVER_MANAGER_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>

#include "job_manager.h"
#include "solver_task.h"

class SolverManager : public JobManager
{

    Q_OBJECT

    protected:

        //! Task server name.
        QString taskServerName;

        //! Task server.
        QLocalServer *taskServer;

        //! Task clients.
        QList<QLocalSocket*> taskClients;

        //! Last solver log file name.
        QString lastLogFileName;

    public:

        //! Constructor
        explicit SolverManager(QObject *parent = nullptr);

        //! Return static instance of the manager.
        static SolverManager & getInstance(void);

        //! Return task server name.
        const QString &getTaskServerName(void) const;

        //! Add new solver task.
        void submit(SolverTask *solverTask);

        //! Check if solver for given model ID is queued.
        bool isSolverQueued(uint modelID) const;

        //! Stop current solvers.
        void stopRunningTasks(void);

        //! Kill current solvers.
        void killRunningTasks(void);

        //! Return const reference to last log file name.
        const QString & getLastLogFileName(void) const;

        //! Stop server.
        void stopServer(void);

    signals:

        //! Message on standard output is availabe.
        void readyReadStandardOutput(const QString &message);

        //! Message on standard error is availabe.
        void readyReadStandardError(const QString &message);
        
    public slots:

        //! Message on standard output is availabe.
        void onReadyReadStandardOutput(const QString &message);

        //! Message on standard error is availabe.
        void onReadyReadStandardError(const QString &message);

    protected slots:

        //! Solver has connected.
        void onSolverTaskNewConnection(void);
        
};

#endif // SOLVER_MANAGER_H
