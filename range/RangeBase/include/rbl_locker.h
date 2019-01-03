/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_locker.h                                             *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   15-th February 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Locker class declaration                            *
 *********************************************************************/

#ifndef RBL_LOCKER_H
#define RBL_LOCKER_H

#include <QMutex>


typedef void (*RLockHandler) ( bool lock );


//! Locker class.
class RLocker
{
    private:

        //! Internal initialization function.
        void _init ( const RLocker *pLocker = nullptr );

    protected:

        //! Mutex.
        QMutex mutex;
        //! Lock handler.
        RLockHandler lockHandler;

    public:

        //! Constructor.
        RLocker ();

        //! Copy constructor.
        RLocker ( const RLocker &locker );

        //! Destructor.
        ~RLocker ();

        //! Assignment operator.
        RLocker & operator = ( const RLocker &locker );

        //! Return static instance to locker.
        static RLocker & getInstance ( void );

        //! Return lock handler function.
        RLockHandler getLockHandler ( void ) const;

        //! Set lock handler function.
        void setLockHandler ( RLockHandler lockHandler );

        //! Set lock.
        void setLock ( bool setLock );

        //! Convenience function to lock.
        static void lock ( void );

        //! Convenience function to unlock.
        static void unlock ( void );

};

#endif /* RBL_LOCKER_H */
