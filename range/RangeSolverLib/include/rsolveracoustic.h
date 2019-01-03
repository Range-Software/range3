/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolveracoustic.h                                        *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   20-th August 2015                                        *
 *                                                                   *
 *  DESCRIPTION: Acoustic solver class declaration                   *
 *********************************************************************/

#ifndef RSOLVERACOUSTIC_H
#define RSOLVERACOUSTIC_H

#include "rsolvergeneric.h"

class RSolverAcoustic : public RSolverGeneric
{

    protected:

        //! Element modulus of elasticity vector.
        RRVector elementElasticityModulus;
        //! Element density vector.
        RRVector elementDensity;
        //! Element damping factor.
        RRVector elementDampingFactor;

        //! Node velocity potential.
        RRVector nodeVelocityPotential;
        //! Node velocity potential - old.
        RRVector nodeVelocityPotentialOld;
        //! Node velocity potential - velocity.
        RRVector nodeVelocityPotentialVelocity;
        //! Node velocity potential - acceleration.
        RRVector nodeVelocityPotentialAcceleration;
        //! Node acoustic pressure.
        RRVector nodeAcousticPressure;
        //! Element acoustic pressure.
        RSolverCartesianVector<RRVector> elementAcousticParticleVelocity;

    private:

        //! Internal initialization function.
        void _init(const RSolverAcoustic *pAcousticSolver = nullptr);

    public:

        //! Constructor.
        explicit RSolverAcoustic(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData);

        //! Copy constructor.
        RSolverAcoustic(const RSolverAcoustic &acousticSolver);

        //! Destructor.
        ~RSolverAcoustic();

        //! Assignment operator.
        RSolverAcoustic & operator =(const RSolverAcoustic &acousticSolver);

        //! Check if solver has converged.
        bool hasConverged(void) const;

    protected:

        double findSoundSpeedScale(void) const;

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

        //! Process absorbing boundary.
        void processAbsorbingBoundary(void);

        //! Process acoustic pressure.
        void processAcousticPressure(void);

        //! Process acoustic particle velocity.
        void processAcousticParticleVelocity(void);

        //! Store solver results.
        void store(void);

        //! Process statistics.
        void statistics(void);

        //! Assembly matrix
        void assemblyMatrix(unsigned int elementID, const RRMatrix &Me, const RRMatrix &Ce, const RRMatrix &Ke, const RRVector &fe);

        //! Find absorbing boundary nodes.
        std::vector<bool> findAbsorbingBoundaryNodes(void) const;

        //! Find absorbing boundary elements.
        std::vector<bool> findAbsorbingBoundaryElements(void) const;

        //! Find absorbing boundary nodes.
        std::vector<RR3Vector> findAbsorbingBoundaryNormals(const std::vector<bool> &absorbingBoundaryNodes) const;

};

#endif // RSOLVERACOUSTIC_H
