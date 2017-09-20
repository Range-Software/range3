/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolvermagnetostatics.h                                  *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   20-th November 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Magnetostatics solver class declaration             *
 *********************************************************************/

#ifndef __RSOLVERMAGNETOSTATICS_H__
#define __RSOLVERMAGNETOSTATICS_H__

#include "rsolvergeneric.h"

class RSolverMagnetostatics : public RSolverGeneric
{

    protected:

        //! Node current density.
        struct { RRVector x, y, z; } nodeCurrentDensity;
        //! Node current density.
        struct { RRVector x, y, z; } nodeMagneticField;

    private:

        //! Internal initialization function.
        void _init(const RSolverMagnetostatics *pSolver = 0);

    public:

        //! Constructor.
        explicit RSolverMagnetostatics(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData);

        //! Copy constructor.
        RSolverMagnetostatics(const RSolverMagnetostatics &solver);

        //! Destructor.
        ~RSolverMagnetostatics();

        //! Assignment operator.
        RSolverMagnetostatics & operator =(const RSolverMagnetostatics &solver);

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

        //! Assembly matrix
        void assemblyMatrix(unsigned int elementID, const RRMatrix &Ke, const RRVector &fe);

};

#endif // __RSOLVERMAGNETOSTATICS_H__
