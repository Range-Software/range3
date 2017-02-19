/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_stop_watch.h                                         *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   30-th December 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Stop-watch declaration                              *
 *********************************************************************/

#ifndef __RBL_STOP_WATCH_H__
#define __RBL_STOP_WATCH_H__

#include <ctime>

class RStopWatch
{

    protected:

        //! Start time.
        clock_t startTime;
        //! Pause time.
        clock_t pauseTime;

    private:

        //! Internal initialization function.
        void _init(const RStopWatch *pStopWatch = 0);

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

        //! Return elapsed clock.
        clock_t getClock(void) const;

        //! Return elapsed time in mili seconds.
        unsigned int getMiliSeconds(void) const;

        //! Return elapsed time in seconds.
        unsigned int getSeconds(void) const;

        //! Add elapsed time to stop watch.
        void addElapsedTime(clock_t elapsedTime);

};

#endif // __RBL_STOP_WATCH_H__
