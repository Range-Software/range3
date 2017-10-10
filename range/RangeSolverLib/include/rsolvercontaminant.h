/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolvercontaminant.h                                     *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   28-th January 2016                                       *
 *                                                                   *
 *  DESCRIPTION: Contaminant solver class declaration                *
 *********************************************************************/

#ifndef __RSOLVERCONTAMINANT_H__
#define __RSOLVERCONTAMINANT_H__

#include "rsolverfluid.h"

class RSolverContaminant : public RSolverFluid
{

    protected:

        //! Node particle concentration.
        RRVector nodeConcentration;

    private:

        //! Internal initialization function.
        void _init(const RSolverContaminant *pSolver = 0);

    public:

        //! Constructor.
        explicit RSolverContaminant(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData);

        //! Copy constructor.
        RSolverContaminant(const RSolverContaminant &solver);

        //! Destructor.
        ~RSolverContaminant();

        //! Assignment operator.
        RSolverContaminant &operator =(const RSolverContaminant &solver);

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

        //! Compute element matrix.
        void computeElement(unsigned int elementID, RRMatrix &Ae, RRVector &be, MatrixManager &matrixManager);

        //! Compute element matrix.
        void computeElementGeneral(unsigned int elementID, RRMatrix &Ae, RRVector &be, MatrixManager &matrixManager);

        //! Compute tetrahedra element matrix.
        void computeElementConstantDerivative(unsigned int elementID, RRMatrix &Ae, RRVector &be, MatrixManager &matrixManager);

        //! Assembly matrix.
        void assemblyMatrix(unsigned int elementID, const RRMatrix &Ae, const RRVector &be);

};

#endif // __RSOLVERCONTAMINANT_H__
