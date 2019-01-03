/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   problem_task_tree.h                                      *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th November 2015                                       *
 *                                                                   *
 *  DESCRIPTION: Problem task tree class declaration                 *
 *********************************************************************/

#ifndef PROBLEM_TASK_TREE_H
#define PROBLEM_TASK_TREE_H

#include <QTreeWidget>
#include <QPushButton>

#include <rml_model.h>

class ProblemTaskTree : public QWidget
{

    Q_OBJECT

    protected:

        enum ColumnType
        {
            NAME = 0,
            VALUE,
            N_COLUMNS
        };

        //! Tree widget.
        QTreeWidget *treeWidget;
        //! Up button.
        QPushButton *upButton;
        //! Down button.
        QPushButton *downButton;
        //! Indent button.
        QPushButton *indentButton;
        //! Unindent button.
        QPushButton *unindentButton;
        //! Delete button.
        QPushButton *deleteButton;

    public:

        //! Constructor.
        explicit ProblemTaskTree(const RProblemTaskItem &rTaskTree, QWidget *parent = nullptr);

        //! Return problem task tree.
        RProblemTaskItem getProblemTaskTree(void) const;

    protected:

        //! Add item to tree widget.
        void addTaskItemToWidget(QTreeWidgetItem *parent, const RProblemTaskItem &taskItem);

        //! Remove item.
        void removeItem(QTreeWidgetItem *item);

        //! Add widget item to task tree.
        static void addWidgetItemToTree(RProblemTaskItem &taskItem, const QTreeWidgetItem *item);

    protected slots:

        //! Handle selection changed signal.
        void onItemSelectionChanged(void);

        //! Handle changed signal.
        void onItemChanged(QTreeWidgetItem *item, int column);

        //! Handle double click signal.
        void onItemDoubleClicked(QTreeWidgetItem *item, int column);

        //! Up button clicked.
        void onUpButtonClicked(void);

        //! Down button clicked.
        void onDownButtonClicked(void);

        //! Indent button clicked.
        void onIndentButtonClicked(void);

        //! Unindent button clicked.
        void onUnindentButtonClicked(void);

        //! Delete button clicked.
        void onDeleteButtonClicked(void);

        //! Add problem type button clicked.
        void onAddProblemButtonClicked(void);

    signals:

        //! Tree changed signal.
        void changed(void);

};

#endif // PROBLEM_TASK_TREE_H
