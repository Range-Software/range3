/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   ic_tree.h                                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Initial condition tree class declaration            *
 *********************************************************************/

#ifndef IC_TREE_H
#define IC_TREE_H

#include <QTreeWidget>

#include <rmlib.h>

class ICTree : public QTreeWidget
{
    Q_OBJECT

    protected:

        //! Initial condition.
        RInitialCondition ic;

    public:

        //! Constructor.
        explicit ICTree(QWidget *parent = nullptr);

    protected:

        //! Populate model.
        void populate(void);

        //! Update selected entities with internal initial condition.
        void updateSelectedEntities(void) const;

    signals:

    public slots:

        //! Initial condition was selected.
        void onIcSelected(RInitialConditionType icType, bool applied);

    protected slots:

        //! Initial condition value has changed.
        void onIcValueChanged(RVariableType variableType, double value);

};

#endif /* IC_TREE_H */
