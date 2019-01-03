/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_application_state.h                                  *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   31-st March 2014                                         *
 *                                                                   *
 *  DESCRIPTION: Application state class declaration                 *
 *********************************************************************/

#ifndef RBL_APPLICATION_STATE_H
#define RBL_APPLICATION_STATE_H

typedef enum _RApplicationStateType
{
    R_APPLICATION_STATE_NONE = 0,
    R_APPLICATION_STATE_INIT,
    R_APPLICATION_STATE_RUN,
    R_APPLICATION_STATE_STOP,
    R_APPLICATION_STATE_N_TYPES
} RApplicationStateType;

class RApplicationState
{

    protected:

        //! Application state type.
        RApplicationStateType stateType;

    private:

        //! Internal initialization function.
        void _init ( const RApplicationState *pApplicationState = nullptr );

    public:

        //! Constructor.
        RApplicationState();

        //! Copy constructor.
        RApplicationState(const RApplicationState &applicationState);

        //! Destructor.
        ~RApplicationState();

        //! Assignment operator.
        RApplicationState & operator =(const RApplicationState &applicationState);

        //! Return static instance to application state.
        static RApplicationState & getInstance(void);

        //! Return application state type.
        RApplicationStateType getStateType(void) const;

        //! Set application state type.
        void setStateType(RApplicationStateType stateType);

};

#endif // RBL_APPLICATION_STATE_H
