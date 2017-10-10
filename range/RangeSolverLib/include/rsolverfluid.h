/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverfluid.h                                           *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   20-th November 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Fluid solver class declaration                      *
 *********************************************************************/

#ifndef __RSOLVERFLUID_H__
#define __RSOLVERFLUID_H__

#include "rsolvergeneric.h"

class MatrixManager;

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
        struct { RRVector x, y, z; } elementVelocity;
        //! Element acceleration.
        struct { RRVector x, y, z; } elementGravity;

        //! Node pressure.
        RRVector nodePressure;
        //! Node velocity.
        struct { RRVector x, y, z; } nodeVelocity;
        //! Node velocity.
        struct { RRVector x, y, z; } nodeVelocityOld;
        //! Node acceleration.
        struct { RRVector x, y, z; } nodeAcceleration;

        //! Stream velocity.
        double streamVelocity;

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
        void _init(const RSolverFluid *pSolver = 0);

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
        void computeElement(unsigned int elementID, RRMatrix &Ae, RRVector &be, MatrixManager &matrixManager);

        //! Compute element matrix.
        void computeElementGeneral(unsigned int elementID, RRMatrix &Ae, RRVector &be, MatrixManager &matrixManager);

        //! Compute tetrahedra element matrix.
        void computeElementConstantDerivative(unsigned int elementID, RRMatrix &Ae, RRVector &be, MatrixManager &matrixManager);

        //! Find Re scale.
        double findReScale(void) const;

        //! Find weight scale.
        double findWeightScale(void) const;

        //! Compute element scales.
        void computeElementScales(void);

        //! Find stream velocity.
        double computeStreamVelocity(bool averageBased) const;

        //! Find element free pressure.
        void computeElementFreePressure(RRVector &values, RBVector &setValues);

        //! Assembly matrix.
        void assemblyMatrix(unsigned int elementID, const RRMatrix &Ae, const RRVector &fe);

        //! Apply local rotations.
        void applyLocalRotations(unsigned int elementID, RRMatrix &Ae);

};

#endif // __RSOLVERFLUID_H__
