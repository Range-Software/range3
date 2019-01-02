/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   job.h                                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   9-th January 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Job class declaration                               *
 *********************************************************************/

#ifndef JOB_H
#define JOB_H

#include <QObject>

class Job : public QObject
{

    Q_OBJECT

    protected:

        //! Job ID.
        uint id;

        //! Indicates whether the job is finished or not.
        bool jobFinished;

        //! Whether the job should be automatically deleted after
        //! completion.
        bool autoDelete;

        //! Whether the job is blocking.
        bool blocking;

    public:

        //! Constructor.
        explicit Job(QObject *parent = nullptr);

        //! Return job ID.
        uint getID(void);

        //! Return information on whether the job is finished or not.
        bool isFinished(void) const;

        //! Return information on whether the job should be
        //! automatically deleted after completion.
        bool getAutoDelete(void) const;

        //! Set whether the job should be automatically deleted after
        //! completion.
        void setAutoDelete(bool autoDelete);

        //! Set whether the job is blocking.
        void setBlocking(bool blocking);

    public slots:

        //! Process job.
        void process(void);

    signals:

        //! Emitted when job blocking state changed.
        void isBlocking(bool blocking);

        //! Emitted when job is started.
        void started(void);

        //! Emitted when job is finished.
        void finished(void);

    protected:

        //! Execute job.
        //! Called by process() and is responsible for calling run()
        //! and for emmiting signals.
        void exec(void);

        //! Function performing actual job.
        //! Inheriting class should reimplement this function.
        virtual void run(void);
};

#endif /* JOB_H */
