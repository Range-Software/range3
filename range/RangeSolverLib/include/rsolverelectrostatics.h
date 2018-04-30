/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverelectrostatics.h                                  *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   19-th November 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Electrostatics solver class declaration             *
 *********************************************************************/

#ifndef __RSOLVERELECTROSTATICS_H__
#define __RSOLVERELECTROSTATICS_H__

#include "rsolvergeneric.h"

class RSolverElectrostatics : public RSolverGeneric
{

    protected:

        //! Node electric potential.
        RRVector nodeElectricPotential;
        //! Element electric field.
        std::vector<RR3Vector> elementElectricField;
        //! Element current density.
        std::vector<RR3Vector> elementCurrentDensity;
        //! Element electric energy.
        RRVector elementElectricEnergy;
        //! Element electric resistivity.
        RRVector elementElectricResistivity;
        //! Element joule heat.
        RRVector elementJouleHeat;
        //! Element relative permittivity.
        RRVector elementRelativePermittivity;
        //! Element electric conductivity.
        RRVector elementElectricConductivity;

    private:

        //! Internal initialization function.
        void _init(const RSolverElectrostatics *pSolver = 0);

    public:

        //! Constructor.
        explicit RSolverElectrostatics(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData);

        //! Copy constructor.
        RSolverElectrostatics(const RSolverElectrostatics &solver);

        //! Destructor.
        ~RSolverElectrostatics();

        //! Assignment operator.
        RSolverElectrostatics & operator =(const RSolverElectrostatics &solver);

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

#endif // __RSOLVERELECTROSTATICS_H__
