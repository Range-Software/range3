/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   problem_selector_dialog.h                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   4-th February 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Problem selector dialog class declaration           *
 *********************************************************************/

#ifndef __PROBLEM_SELECTOR_DIALOG_H__
#define __PROBLEM_SELECTOR_DIALOG_H__

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
        explicit ProblemSelectorDialog(QWidget *parent = 0);

        //! Execute dialog.
        RProblemTypeMask exec(void);

    protected slots:

        //! Handle ProblemSelectorTree::changed signal.
        void onProblemTreeChanged(void);

};

#endif /* __PROBLEM_SELECTOR_DIALOG_H__ */
