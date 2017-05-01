/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   bc_tree.h                                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   9-th February 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Boundary condition tree class declaration           *
 *********************************************************************/

#ifndef __BC_TREE_H__
#define __BC_TREE_H__

#include <QTreeWidget>

#include <rmlib.h>

class BCTree : public QTreeWidget
{
    Q_OBJECT

    protected:

        //! Boundary condition.
        RBoundaryCondition bc;

    public:

        //! Constructor.
        explicit BCTree(QWidget *parent = 0);

    protected:

        //! Populate model.
        void populate(void);

        //! Update selected entities with internal boundary condition.
        void updateSelectedEntities(void) const;

    signals:

    public slots:

        //! Boundary condition was selected.
        void onBcSelected(RBoundaryConditionType bcType, bool applied);

    protected slots:

        //! Boundary condition value has changed.
        void onBcValueChanged(RVariableType variableType, double value);

        //! Edit values button was clicked.
        void onButtonValueClicked(int id);

};

#endif /* __BC_TREE_H__ */
