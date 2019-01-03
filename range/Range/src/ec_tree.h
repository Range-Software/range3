/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   ec_tree.h                                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   7-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Environment condition tree class declaration        *
 *********************************************************************/

#ifndef EC_TREE_H
#define EC_TREE_H

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
        explicit ECTree(QWidget *parent = nullptr);

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

#endif /* EC_TREE_H */
