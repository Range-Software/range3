/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverfluidheat.h                                       *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   28-th January 2016                                       *
 *                                                                   *
 *  DESCRIPTION: Fluid - heat transfer solver class declaration      *
 *********************************************************************/

#ifndef RSOLVERFLUIDHEAT_H
#define RSOLVERFLUIDHEAT_H

#include "rmatrixmanager.h"
#include "rsolvergeneric.h"

class FluidHeatMatrixContainer;

class RSolverFluidHeat : public RSolverGeneric
{

    protected:

        //! Element heat capacity vector.
        RRVector elementCapacity;
        //! Element thermal conduction vector.
        RRVector elementConduction;
        //! Element density vector.
        RRVector elementDensity;
        //! Node temperature.
        RRVector nodeTemperature;
        //! Node heat vector.
        RRVector nodeHeat;
        //! Element heat vector.
        RRVector elementHeat;
        //! Element radiation heat vector.
        RRVector elementRadiativeHeat;
        //! Element joule heat.
        RRVector elementJouleHeat;
        //! Element heat flux vector.
        std::vector<RR3Vector> elementHeatFlux;

        //! Element velocity.
        RSolverCartesianVector<RRVector> elementVelocity;
        //! Node velocity.
        RSolverCartesianVector<RRVector> nodeVelocity;

        //! Stream velocity.
        double streamVelocity;

        //! Temperature convergence.
        double cvgT;

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
        void _init(const RSolverFluidHeat *pSolver = nullptr);

    public:

        //! Constructor.
        explicit RSolverFluidHeat(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData);

        //! Copy constructor.
        RSolverFluidHeat(const RSolverFluidHeat &solver);

        //! Destructor.
        ~RSolverFluidHeat();

        //! Assignment operator.
        RSolverFluidHeat &operator =(const RSolverFluidHeat &solver);

        //! Check if solver has converged.
        bool hasConverged(void) const;

    protected:

        //! Find temperature scale.
        double findTemperatureScale(void) const;

        //! Generate node heat input vector.
        void generateNodeHeatVector(void);

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
        void computeElement(unsigned int elementID, RRMatrix &Ae, RRVector &be, RMatrixManager<FluidHeatMatrixContainer> &matrixManager);

        //! Compute element matrix.
        void computeElementGeneral(unsigned int elementID, RRMatrix &Ae, RRVector &be, RMatrixManager<FluidHeatMatrixContainer> &matrixManager);

        //! Compute tetrahedra element matrix.
        void computeElementConstantDerivative(unsigned int elementID, RRMatrix &Ae, RRVector &be, RMatrixManager<FluidHeatMatrixContainer> &matrixManager);

        //! Assembly matrix.
        void assemblyMatrix(unsigned int elementID, const RRMatrix &Ae, const RRVector &be);

};

#endif // RSOLVERFLUIDHEAT_H
