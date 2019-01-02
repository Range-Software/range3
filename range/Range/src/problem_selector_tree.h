/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   problem_selector_tree.h                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th November 2015                                       *
 *                                                                   *
 *  DESCRIPTION: Problem selector tree class declaration             *
 *********************************************************************/

#ifndef PROBLEM_SELECTOR_TREE_H
#define PROBLEM_SELECTOR_TREE_H

#include <rmlib.h>

#include <QTreeWidget>

class ProblemSelectorTree : public QTreeWidget
{

    Q_OBJECT

    protected:

        //! Problem tree widget.
        QTreeWidget *problemTree;

    public:

        //! Constructor.
        explicit ProblemSelectorTree(QWidget *parent = nullptr);

        //! Return problem mask for model id.
        RProblemTypeMask findProblemTypeMask(void);

        //! Check if some problem type is selected.
        bool checkProblemIsChecked(void) const;

    protected:

        //! Check checked item.
        void checkCheckedTreeWidgetItem(QTreeWidgetItem *item);

        //! Check unchecked item.
        void checkUncheckedTreeWidgetItem(QTreeWidgetItem *item);

    protected slots:

        //! Tree widget item has changed.
        void onTreeWidgetItemChanged(QTreeWidgetItem *item);

    signals:

        //! Widget changes signal.
        void changed(void);

};

#endif // PROBLEM_SELECTOR_TREE_H
