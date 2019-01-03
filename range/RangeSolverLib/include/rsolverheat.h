/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolverheat.h                                            *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   12-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Heat-transfer solver class declaration              *
 *********************************************************************/

#ifndef RSOLVERHEAT_H
#define RSOLVERHEAT_H

#include <rblib.h>

#include "rsolvergeneric.h"

class RSolverHeat : public RSolverGeneric
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
        //! Element heat vector.
        RRVector elementHeat;
        //! Element radiation heat vector.
        RRVector elementRadiativeHeat;
        //! Element joule heat.
        RRVector elementJouleHeat;
        //! Element heat flux vector.
        std::vector<RR3Vector> elementHeatFlux;

    private:

        //! Internal initialization function.
        void _init(const RSolverHeat *pHeatSolver = nullptr);

    public:

        //! Constructor.
        explicit RSolverHeat(RModel *pModel, const QString &modelFileName, const QString &convergenceFileName, RSolverSharedData &sharedData);

        //! Copy constructor.
        RSolverHeat(const RSolverHeat &heatSolver);

        //! Destructor.
        ~RSolverHeat();

        //! Assignment operator.
        RSolverHeat & operator =(const RSolverHeat &heatSolver);

        //! Check if solver has converged.
        bool hasConverged(void) const;

    protected:

        //! Find temperature scale.
        double findTemperatureScale(void) const;

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
        void assemblyMatrix(unsigned int elementID, const RRMatrix &Me, const RRMatrix &Ke, const RRVector &fe);

        //! Get simple convection BC values.
        bool getSimpleConvection(const RElementGroup &elementGroup, double &htc, double &htt);

        //! Get forced convection BC values.
        bool getForcedConvection(const RElementGroup &elementGroup, double &htc, double &htt);

        //! Get natural convection BC values.
        bool getNaturalConvection(const RElementGroup &elementGroup, unsigned int elementId, double &htc, double &htt);

};

#endif // RSOLVERHEAT_H
