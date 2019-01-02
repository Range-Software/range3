/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   job_manager.h                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   9-th January 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Job manager class declaration                       *
 *********************************************************************/

#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#include <QObject>
#include <QQueue>
#include <QList>
#include <QThread>

#include "job.h"

class JobManager : public QObject
{
    Q_OBJECT

    protected:

        //! Executing thread.
        QThread runningThread;
        //! Waiting jobs to be processed.
        QQueue<Job*> waitingJobs;
        //! Running jobs.
        QList<Job*> runningJobs;
        //! Job is starting - indicator.
        bool jobIsStarting;

    public:

        //! Constructor.
        explicit JobManager(QObject *parent = nullptr);

        //! Return static instance of the manager.
        static JobManager & getInstance(void);

        //! Add new job.
        void submit(Job *job);

        //! Return number of waiting jobs.
        uint getNWaiting(void) const;

        //! Return number of running jobs.
        uint getNRunning(void) const;

        //! Get running IDs;
        QVector<uint> getRunningIDs(void) const;

    signals:

        //! Signal emitted when job is blocking.
        void jobBlocking(bool blocking);

        //! Signal emitted when new job is started.
        void jobStarted(void);

        //! Signal emitted when job is finished.
        void jobFinished(void);

    protected slots:

        //! Called when blocking() signal is emitted.
        void onJobBlocking(bool blocking);

        //! Called when started() signal is emitted.
        void onJobStarted(void);

        //! Called when finished() signal is emitted.
        void onJobFinished(void);

    protected:

        //! Process waiting jobs.
        //! If no queued job is running then start first in the list.
        void processWaitingJobs(void);

        //! Process finished jobs.
        //! Remove all jobs which are finished from running queue.
        void processFinishedJobs(void);

        //! Start job.
        void startJob(Job *job);

        //! Find finished job in the running queue.
        Job * findFinishedJob(void) const;

        //! Remove finished jobs from the running queue.
        uint removeFinishedJobs(void);

};

#endif /* JOB_MANAGER_H */
