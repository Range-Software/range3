/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   solver_task_id.h                                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   16-th August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Solver task ID class declaration                    *
 *********************************************************************/

#ifndef __SOLVER_TASK_ID_H__
#define __SOLVER_TASK_ID_H__

#include <QString>

class SolverTaskID
{

    protected:

        //! Year.
        int year;
        //! Month.
        int month;
        //! Day.
        int day;
        //! Hour.
        int hour;
        //! Minute.
        int minute;
        //! Counter.
        unsigned int counter;

    private:

        //! Internal initialization function.
        void _init ( const SolverTaskID *pSolverTaskID = 0 );

    public:

        //! Constructor.
        SolverTaskID();

        //! Copy constructor.
        SolverTaskID(const SolverTaskID &solverTaskID);

        //! Destructor.
        ~SolverTaskID();

        //! Assignment operator.
        SolverTaskID & operator =(const SolverTaskID &solverTaskID);

        //! Compare equal operator.
        bool operator ==(const SolverTaskID &solverTaskID) const;

        //! Generate new ID.
        void generate(void);

        //! Return true if ID is valid.
        bool isValid(void) const;

        //! Convert to string.
        const QString toString(void) const;

};

#endif // __SOLVER_TASK_ID_H__
