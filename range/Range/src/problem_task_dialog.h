/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   problem_task_dialog.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th November 2015                                       *
 *                                                                   *
 *  DESCRIPTION: Problem task dialog class declaration               *
 *********************************************************************/

#ifndef PROBLEM_TASK_DIALOG_H
#define PROBLEM_TASK_DIALOG_H

#include <QDialog>

#include "problem_task_tree.h"

class ProblemTaskDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Model ID.
        uint modelID;
        //! Problem task tree widget.
        ProblemTaskTree *taskTree;
        //! OK button.
        QPushButton *okButton;
        //! Cancel button.
        QPushButton *cancelButton;

    public:

        //! Constructor.
        explicit ProblemTaskDialog(uint modelID, QWidget *parent = nullptr);

        //! Execute dialog.
        int exec(void);

    protected slots:

        //! Handle close event.
        void closeEvent(QCloseEvent *e);

        //! Handle key press event.
        void keyPressEvent(QKeyEvent *e);

        //! handle ProblemtaskTree::changed() signal.
        void onProblemTaskTreeChanged(void);

};

#endif // PROBLEM_TASK_DIALOG_H
