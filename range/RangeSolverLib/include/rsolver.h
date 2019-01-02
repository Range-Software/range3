/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolver.h                                                *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   11-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Range solver class declaration                      *
 *********************************************************************/

#ifndef RSOLVER_H
#define RSOLVER_H

#include <rmlib.h>

#include "rsolvergeneric.h"
#include "rsolvershareddata.h"

class RSolver
{

    protected:

        //! Pointer to model object.
        RModel *pModel;
        //! Model file name.
        QString modelFileName;
        //! Convergence file name.
        QString convergenceFileName;
        //! Shared variables.
        RSolverSharedData sharedData;
        //! Map of solvers.
        QMap<RProblemTypeMask,RSolverGeneric*> solvers;
        //! Map of sover type and execution count.
        QMap<RProblemType,uint> solversExecutionCount;

    private:

        //! Internal initialization function.
        void _init(const RSolver *pSolver = nullptr);

    public:

        //! Constructor.
        RSolver(RModel &model, const QString &modelFileName, const QString &convergenceFileName);

        //! Copy constructor.
        RSolver(const RSolver &solver);

        //! Destructor.
        ~RSolver();

        //! Assignment operator.
        RSolver & operator =(const RSolver &solver);

        //! Run solver.
        void run(void);

    protected:

        //! Run single solver.
        void runSingle(void);

        //! Run single solver for given problem task.
        //! Return task convergence status (true = converged).
        bool runProblemTask(const RProblemTaskItem &problemTaskItem, uint taskIteration);

};

#endif // RSOLVER_H
