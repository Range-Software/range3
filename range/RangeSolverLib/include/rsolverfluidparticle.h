/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverfluidparticle.h                                   *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   28-th January 2016                                       *
 *                                                                   *
 *  DESCRIPTION: Fluid particle dispersion solver class declaration  *
 *********************************************************************/

#ifndef RSOLVERFLUIDPARTICLE_H
#define RSOLVERFLUIDPARTICLE_H

#include "rmatrixmanager.h"
#include "rsolvergeneric.h"

class FluidParticleMatrixContainer;

class RSolverFluidParticle : public RSolverGeneric
{

    protected:

        //! Element particle concentration.
        RRVector elementConcentration;
        //! Element particle rate.
        RRVector elementRate;
        //! Element velocity.
        RSolverCartesianVector<RRVector> elementVelocity;

        //! Stream velocity.
        double streamVelocity;

        //! Node particle concentration.
        RRVector nodeConcentration;
        //! Node particle rate.
        RRVector nodeRate;
        //! Node velocity.
        RSolverCartesianVector<RRVector> nodeVelocity;

        //! Element density.
        RRVector elementDensity;
        //! Element density.
        RRVector elementDiffusion;

        //! Concentration convergence.
        double cvgC;

        //! Vector of element level shape function derivatives.
        std::vector<RElementShapeDerivation *> shapeDerivations;

        //! Stop-watches
        RStopWatch recoveryStopWatch;
        RStopWatch buildStopWatch;
        RStopWatch assemblyStopWatch;
        RStopWatch solverStopWatch;
        RStopWatch updateStopWatch;

    private:

        //! Internal initialization function.
        void _init(const RSolverFluidParticle *pSolver = nullptr);

    public:

        //! Constructor.
        explicit RSolverFluidParticle(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData);

        //! Copy constructor.
        RSolverFluidParticle(const RSolverFluidParticle &solver);

        //! Destructor.
        ~RSolverFluidParticle();

        //! Assignment operator.
        RSolverFluidParticle &operator =(const RSolverFluidParticle &solver);

        //! Check if solver has converged.
        bool hasConverged(void) const;

    protected:

        //! Generate node rate input vector.
        void generateNodeRateVector(void);

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

        //! Compute element shape derivatives.
        void computeShapeDerivatives(void);

        //! Clear element shape derivatives.
        void clearShapeDerivatives(void);

        //! Compute element matrix.
        void computeElement(unsigned int elementID, RRMatrix &Ae, RRVector &be, RMatrixManager<FluidParticleMatrixContainer> &matrixManager);

        //! Compute element matrix.
        void computeElementGeneral(unsigned int elementID, RRMatrix &Ae, RRVector &be, RMatrixManager<FluidParticleMatrixContainer> &matrixManager);

        //! Compute tetrahedra element matrix.
        void computeElementConstantDerivative(unsigned int elementID, RRMatrix &Ae, RRVector &be, RMatrixManager<FluidParticleMatrixContainer> &matrixManager);

        //! Assembly matrix.
        void assemblyMatrix(unsigned int elementID, const RRMatrix &Ae, const RRVector &be);

};

#endif // RSOLVERFLUIDPARTICLE_H
