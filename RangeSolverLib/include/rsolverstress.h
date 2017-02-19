/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverstress.h                                          *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   27-th May 2015                                           *
 *                                                                   *
 *  DESCRIPTION: Stress-strain solver class declaration              *
 *********************************************************************/

#ifndef __RSOLVERSTRESS_H__
#define __RSOLVERSTRESS_H__

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
        struct { RRVector x, y, z; } nodeDisplacement;
        //! Node initial displacement vector.
        struct { RRVector x, y, z; } nodeInitialDisplacement;
        //! Node force vector.
        struct { RRVector x, y, z; } nodeForce;
        //! Node acceleration vector.
        struct { RRVector x, y, z; } nodeAcceleration;
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
        void _init(const RSolverStress *pStressSolver = 0);

    public:

        //! Constructor.
        explicit RSolverStress(RModel *pModel, const QString &modelFileName, RSolverSharedData &sharedData, bool modalAnalysis);

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

        //! Apply local rotations.
        void applyLocalRotations(unsigned int elementID, RRMatrix &Ae);

};

#endif // __RSOLVERSTRESS_H__