/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   locker.h                                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   15-rd February 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Locker functions                                    *
 *********************************************************************/

#ifndef LOCKER_H
#define LOCKER_H

#include <QMutex>

class Locker
{

    protected:

        //! Constructor.
        inline Locker() { }

        //! Mutex.
        QMutex mutex;

    public:

        //! Initialize RLocker static instance.
        static void initialize(void);

        //! Return reference to static object.
        static Locker & getInstance(void);

        //! Return pointer to mutex.
        QMutex * getMutexPtr(void);
};

#endif /* LOCKER_H */
