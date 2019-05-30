/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverstress.h                                          *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   27-th May 2015                                           *
 *                                                                   *
 *  DESCRIPTION: Stress-strain solver class declaration              *
 *********************************************************************/

#ifndef RSOLVERSTRESS_H
#define RSOLVERSTRESS_H

#include <rblib.h>

#include "rsolvergeneric.h"

class RSolverStress : public RSolverGeneric
{

    protected:

        //! Element modulus of elasticity vector.
        RRVector elementElasticityModulus;
        //! Element Poisson ratio.
        RRVector elementPoissonRatio;
        //! Element density vector.
        RRVector elementDensity;
        //! Element thermal expansion coefficient vector.
        RRVector elementThermalExpansion;
        //! Element environment temperature vector.
        RRVector elementEnvironmentTemperature;
        //! Node displacement vector.
        RSolverCartesianVector<RRVector> nodeDisplacement;
        //! Node initial displacement vector.
        RSolverCartesianVector<RRVector> nodeInitialDisplacement;
        //! Node force vector.
        RSolverCartesianVector<RRVector> nodeForce;
        //! Node acceleration vector.
        RSolverCartesianVector<RRVector> nodeAcceleration;
        //! Node pressure.
        RRVector nodePressure;
        //! Element normal stress.
        RRVector elementNormalStress;
        //! Element shear stress.
        RRVector elementShearStress;
        //! Element VonMisses stress.
        RRVector elementVonMisses;

        //! Eigen values.
        RRVector d;
        //! Eigen vectors.
        RRMatrix ev;

    private:

        //! Internal initialization function.
        void _init(const RSolverStress *pStressSolver = nullptr);

    public:

        //! Constructor.
        explicit RSolverStress(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData, bool modalAnalysis);

        //! Copy constructor.
        RSolverStress(const RSolverStress &stressSolver);

        //! Destructor.
        ~RSolverStress();

        //! Assignment operator.
        RSolverStress & operator =(const RSolverStress &stressSolver);

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

        //! Solve stress-strain problem.
        void solveStressStrain(void);

        //! Solve eigen-value problem.
        void solveEigenValue(void);

        //! Set displacemen.
        void setDisplacement(const RRVector &v);

        //! Process solver results.
        void process(void);

        //! Store solver results.
        void store(void);

        //! Process statistics.
        void statistics(void);

        //! Generate node book.
        void generateNodeBook(void);

        //! Assembly matrix
        void assemblyMatrix(unsigned int elementID, const RRMatrix &Me, const RRMatrix &Ke, const RRVector &fe);

        //! Apply local rotations to matrix.
        void applyLocalRotations(unsigned int elementID, RRMatrix &Ae);

        //! Apply local rotations to vector.
        void applyLocalRotations(unsigned int elementID, RRVector &fe);

};

#endif // RSOLVERSTRESS_H
