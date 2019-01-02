/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   solver_start_dialog.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   19-th September 2017                                     *
 *                                                                   *
 *  DESCRIPTION: Solver start dialog class declaration               *
 *********************************************************************/

#ifndef SOLVER_START_DIALOG_H
#define SOLVER_START_DIALOG_H

#include <QDialog>
#include <QCheckBox>

#include "solver_task.h"

class SolverStartDialog : public QDialog
{

    protected:

        //! Model IDs.
        uint modelID;
        //! List of warnings.
        QStringList warnings;
        //! List of errors.
        QStringList errors;
        //! Restart solver.
        QCheckBox *restartSolverCheck;
        //! Pointer to solvr task object.
        SolverTask *pSolverTask;

    public:

        //! Constructor.
        explicit SolverStartDialog(uint modelID, QWidget *parent = nullptr);

        //! Execute dialog.
        int exec(void);

};

#endif // SOLVER_START_DIALOG_H
