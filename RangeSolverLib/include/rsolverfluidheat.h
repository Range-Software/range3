/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverfluidheat.h                                       *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   28-th January 2016                                       *
 *                                                                   *
 *  DESCRIPTION: Fluid - heat transfer solver class declaration      *
 *********************************************************************/

#ifndef __RSOLVERFLUIDHEAT_H__
#define __RSOLVERFLUIDHEAT_H__

#include "rsolverfluid.h"

class RSolverFluidHeat : public RSolverFluid
{

    private:

        //! Internal initialization function.
        void _init(const RSolverFluidHeat *pSolver = 0);

    public:

        //! Constructor.
        explicit RSolverFluidHeat(RModel *pModel, const QString &modelFileName, RSolverSharedData &sharedData);

        //! Copy constructor.
        RSolverFluidHeat(const RSolverFluidHeat &solver);

        //! Destructor.
        ~RSolverFluidHeat();

        //! Assignment operator.
        RSolverFluidHeat &operator =(const RSolverFluidHeat &solver);

        //! Check if solver has converged.
        bool hasConverged(void) const;

    protected:

        //! Update scales.
        void updateScales(void);

        //! Recover previously computed results.
        void recover(void);

        //! Prepare solver.
        void prepare(void);

        //! Run matrix solver.
        void solve(void);

        //! Process solver results.
        void process(void);

        //! Store solver results.
        void store(void);

        //! Process statistics.
        void statistics(void);

};

#endif // __RSOLVERFLUIDHEAT_H__
