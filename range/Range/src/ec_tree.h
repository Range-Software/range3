/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   ec_tree.h                                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   7-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Environment condition tree class declaration        *
 *********************************************************************/

#ifndef __EC_TREE_H__
#define __EC_TREE_H__

#include <QTreeWidget>

#include <rmlib.h>

class ECTree : public QTreeWidget
{
    Q_OBJECT

    protected:

        //! Environment condition.
        REnvironmentCondition ec;

    public:

        //! Constructor.
        explicit ECTree(QWidget *parent = 0);

    protected:

        //! Populate model.
        void populate(void);

        //! Update selected entities with internal environment condition.
        void updateSelectedEntities(void) const;

    signals:

    public slots:

        //! Environment condition was selected.
        void onEcSelected(REnvironmentConditionType ecType, bool applied);

    protected slots:

        //! Environment condition value has changed.
        void onEcValueChanged(RVariableType variableType, double value);

};

#endif /* __EC_TREE_H__ */
