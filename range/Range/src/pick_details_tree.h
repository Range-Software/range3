/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   pick_details_tree.h                                      *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   17-th June 2013                                          *
 *                                                                   *
 *  DESCRIPTION: Pick details tree class declaration                 *
 *********************************************************************/

#ifndef PICK_DETAILS_TREE_H
#define PICK_DETAILS_TREE_H

#include <QTreeWidget>

class PickDetailsTree : public QTreeWidget
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit PickDetailsTree(QWidget *parent = nullptr);

    protected:

        //! Populate the tree.
        void populate(void);
        
    protected slots:

        //! Pick list has changed.
        void onPickListChanged(void);

        //! Tree item wwas expanded.
        void onItemExpanded(QTreeWidgetItem *);

        //! Resize columns to content.
        void resizeColumnsToContent(void);
        
};

#endif // PICK_DETAILS_TREE_H
