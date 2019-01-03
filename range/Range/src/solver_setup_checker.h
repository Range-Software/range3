/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   solver_setup_checker.h                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   2-nd June 2016                                           *
 *                                                                   *
 *  DESCRIPTION: Solver setup checker class declaration              *
 *********************************************************************/

#ifndef SOLVER_SETUP_CHECKER_H
#define SOLVER_SETUP_CHECKER_H

#include <rmlib.h>

class SolverSetupChecker
{

    protected:

        //! Const reference to model.
        const RModel &rModel;

    private:

        //! Internal initialization function.
        void _init(const SolverSetupChecker *pSolverSetupChecker = nullptr);

    public:

        //! Constructor.
        explicit SolverSetupChecker(const RModel &rModel);

        //! Copy constructor.
        SolverSetupChecker(const SolverSetupChecker &solverSetupChecker);

        //! Destructor.
        ~SolverSetupChecker();

        //! Assignment operator.
        SolverSetupChecker &operator =(const SolverSetupChecker &solverSetupChecker);

        //! Perform check.
        void perform(QStringList &warnings,QStringList &errors) const;

    private:

        //! Check elements.
        void checkElements(QStringList &warnings,QStringList &errors) const;

        //! Check assigned materials.
        void checkMaterials(QStringList &warnings,QStringList &errors) const;

        //! Check boundary conditions.
        void checkBoundaryConditions(QStringList &warnings,QStringList &errors) const;

};

#endif // SOLVER_SETUP_CHECKER_H
