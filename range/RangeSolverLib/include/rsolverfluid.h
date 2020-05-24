/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverfluid.h                                           *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   20-th November 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Fluid solver class declaration                      *
 *********************************************************************/

#ifndef RSOLVERFLUID_H
#define RSOLVERFLUID_H

#include "rmatrixmanager.h"
#include "rsolvergeneric.h"

class FluidMatrixContainer;

class RSolverFluid : public RSolverGeneric
{

    protected:

        //! Node height.
        RRVector freePressureNodeHeight;
        //! Element scales.
        RRVector elementScales;

        //! Element pressure.
        RRVector elementPressure;
        //! Element velocity.
        RSolverCartesianVector<RRVector> elementVelocity;
        //! Element acceleration.
        RSolverCartesianVector<RRVector> elementGravity;

        //! Node pressure.
        RRVector nodePressure;
        //! Node velocity.
        RSolverCartesianVector<RRVector> nodeVelocity;
        //! Node velocity.
        RSolverCartesianVector<RRVector> nodeVelocityOld;
        //! Node acceleration.
        RSolverCartesianVector<RRVector> nodeAcceleration;

        //! Stream velocity.
        double streamVelocity;
        double invStreamVelocity;

        //! Element density.
        RRVector elementDensity;
        //! Element viscosity.
        RRVector elementViscosity;

        //! Average density.
        double avgRo;
        //! Average dynamic viscosity.
        double avgU;

        //! Velocity convergence.
        double cvgV;
        //! Pressure convergence.
        double cvgP;

        //! Vector of surface normals.
        std::vector<RR3Vector> elementNormals;
        //! Element gravity magnitude.
        RRVector elementGravityMagnitude;
        //! Vector of element level shape function derivatives.
        std::vector<RElementShapeDerivation *> shapeDerivations;

        //! Stop-watches
        RStopWatch recoveryStopWatch;
        RStopWatch buildStopWatch;
        RStopWatch solverStopWatch;
        RStopWatch updateStopWatch;

    private:

        //! Internal initialization function.
        void _init(const RSolverFluid *pSolver = nullptr);

    public:

        //! Constructor.
        explicit RSolverFluid(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData);

        //! Copy constructor.
        RSolverFluid(const RSolverFluid &solver);

        //! Destructor.
        ~RSolverFluid();

        //! Assignment operator.
        RSolverFluid & operator =(const RSolverFluid &solver);

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

        //! Find input vectors.
        void findInputVectors(void);

        //! Generate node book.
        void generateNodeBook(void);

        //! Compute free pressure node height.
        void computeFreePressureNodeHeight(void);

        //! Compute element shape derivatives.
        void computeShapeDerivatives(void);

        //! Clear element shape derivatives.
        void clearShapeDerivatives(void);

        //! Compute element matrix.
        void computeElement(unsigned int elementID, RRMatrix &Ae, RRVector &be, RMatrixManager<FluidMatrixContainer> &matrixManager);

        //! Compute element matrix.
        void computeElementGeneral(unsigned int elementID, RRMatrix &Ae, RRVector &be, RMatrixManager<FluidMatrixContainer> &matrixManager);

        //! Compute tetrahedra element matrix.
        void computeElementConstantDerivative(unsigned int elementID, RRMatrix &Ae, RRVector &be, RMatrixManager<FluidMatrixContainer> &matrixManager);

        //! Find time scale.
        double findTimeScale(void) const;

        //! Find Re scale.
        double findReScale(void) const;

        //! Find weight scale.
        double findWeightScale(void) const;

        //! Compute element scales.
        void computeElementScales(void);

        //! Find element free pressure.
        void computeElementFreePressure(RRVector &values, RBVector &setValues);

        //! Assembly matrix.
        void assemblyMatrix(unsigned int elementID, const RRMatrix &Ae, const RRVector &fe);

        //! Assembly matrix.
        void assemblyMatrix(unsigned int elementID, const RRMatrix &Ae, const RRVector &fe, RSparseMatrix &Ap, RRVector &bp);

        //! Apply local rotations.
        void applyLocalRotations(unsigned int elementID, RRMatrix &Ae);

    public:

        //! Find stream velocity.
        static double computeStreamVelocity(const RModel &rModel,
                                            const RSolverCartesianVector<RRVector> &nodeVelocity,
                                            bool averageBased,
                                            double streamVelocity = 0.0);

};

#endif // RSOLVERFLUID_H
