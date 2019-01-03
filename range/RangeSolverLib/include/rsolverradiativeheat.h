/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverradiativeheat.h                                   *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th November 2015                                       *
 *                                                                   *
 *  DESCRIPTION: Radiative Heat-transfer solver class declaration    *
 *********************************************************************/

#ifndef RSOLVERRADIATIVEHEAT_H
#define RSOLVERRADIATIVEHEAT_H

#include <rblib.h>

#include "rsolvergeneric.h"


class RSolverRadiativeHeat : public RSolverGeneric
{

    protected:

        //! Element temperature.
        RRVector elementTemperature;
        //! Element heat vector.
        RRVector elementRadiativeHeat;
        //! Patch heat vector.
        RRVector patchHeat;
        //! View-factor matrix.
        RViewFactorMatrix viewFactorMatrix;
        //! Patch heat norm.
        double patchHeatNorm;
        //! Old patch heat norm.
        double oldPatchHeatNorm;

    private:

        //! Internal initialization function.
        void _init(const RSolverRadiativeHeat *pRadiativeHeatSolver = nullptr);

    public:

        //! Constructor.
        explicit RSolverRadiativeHeat(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData);

        //! Copy constructor.
        RSolverRadiativeHeat(const RSolverRadiativeHeat &radiativeHeatSolver);

        //! Destructor.
        ~RSolverRadiativeHeat();

        //! Assignment operator.
        RSolverRadiativeHeat &operator =(const RSolverRadiativeHeat &radiativeHeatSolver);

        //! Check if solver has converged.
        bool hasConverged(void) const;

    protected:

        //! Find temperature scale.
        double findTemperatureScale(void) const;

        //! Update scales.
        void updateScales(void);

        //! Recover previously computed results.
        void recover(void);

        //! Prepare view-factors.
        void prepareViewFactors(void);

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

        //! Check if view-factor header correspond with input.
        bool checkViewFactorHeader(const RViewFactorMatrixHeader &viewFactorMatrixHeader) const;

};

#endif // RSOLVERRADIATIVEHEAT_H
