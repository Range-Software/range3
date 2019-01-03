/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   problem_selector_dialog.h                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   4-th February 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Problem selector dialog class declaration           *
 *********************************************************************/

#ifndef PROBLEM_SELECTOR_DIALOG_H
#define PROBLEM_SELECTOR_DIALOG_H

#include <QDialog>

#include "problem_selector_tree.h"

class ProblemSelectorDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Problem selector tree widget.
        ProblemSelectorTree *problemTree;
        //! OK button.
        QPushButton *okButton;
        //! Cancel button.
        QPushButton *cancelButton;

    public:

        //! Constructor.
        explicit ProblemSelectorDialog(QWidget *parent = nullptr);

        //! Execute dialog.
        RProblemTypeMask exec(void);

    protected slots:

        //! Handle ProblemSelectorTree::changed signal.
        void onProblemTreeChanged(void);

};

#endif /* PROBLEM_SELECTOR_DIALOG_H */
