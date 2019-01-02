/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolvermagnetostatics.h                                  *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   20-th November 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Magnetostatics solver class declaration             *
 *********************************************************************/

#ifndef RSOLVERMAGNETOSTATICS_H
#define RSOLVERMAGNETOSTATICS_H

#include "rsolvergeneric.h"

class RSolverMagnetostatics : public RSolverGeneric
{

    protected:

        //! Node current density.
        RSolverCartesianVector<RRVector> nodeCurrentDensity;
        //! Node current density.
        RSolverCartesianVector<RRVector> nodeMagneticField;

    private:

        //! Internal initialization function.
        void _init(const RSolverMagnetostatics *pSolver = nullptr);

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

#endif // RSOLVERMAGNETOSTATICS_H
