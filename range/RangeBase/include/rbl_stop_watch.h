/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_stop_watch.h                                         *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   30-th December 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Stop-watch declaration                              *
 *********************************************************************/

#ifndef RBL_STOP_WATCH_H
#define RBL_STOP_WATCH_H

#include <QtGlobal>

class RStopWatch
{

    protected:

        //! Start time.
        qint64 startTime;
        //! Pause time.
        qint64 pauseTime;

    private:

        //! Internal initialization function.
        void _init(const RStopWatch *pStopWatch = nullptr);

    public:

        //! Constructor.
        RStopWatch();

        //! Copy constructor.
        RStopWatch(const RStopWatch &stopWatch);

        //! Destructor.
        ~RStopWatch();

        //! Assignment operator.
        RStopWatch &operator =(const RStopWatch &stopWatch);

        //! Reset stop-watch.
        void reset(void);

        //! Pause stop-watch.
        void pause(void);

        //! Resume stop-watch.
        void resume(void);

        //! Return elapsed time in mili seconds.
        qint64 getMiliSeconds(void) const;

        //! Add elapsed time to stop watch.
        void addElapsedTime(qint64 elapsedTime);

};

#endif // RBL_STOP_WATCH_H
