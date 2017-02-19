/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   ec_manager_tree.h                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   7-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Environment condition manager tree class declaration*
 *********************************************************************/

#ifndef __EC_MANAGER_TREE_H__
#define __EC_MANAGER_TREE_H__

#include <QTreeWidget>

#include <rmlib.h>

#include "session_entity_id.h"

class ECManagerTree : public QTreeWidget
{
    Q_OBJECT

    private:

        //! List of selected types.
        //! Used only for reselecting already selected items
        //! when model selection has changed.
        QList<REnvironmentConditionType> selectedTypes;
        //! If set to true populat function will return immediately.
        bool doNotPopulate;

    public:

        //! Constructor.
        explicit ECManagerTree(QObject *parent = 0);

    protected:

        //! Populate tree.
        void populate(void);

        //! Return map of applied environment conditions to number of occurences based on model ID list.
        QMap<REnvironmentConditionType,uint> getEcMap(const QList<SessionEntityID> &entityIDs) const;

    signals:

        //! Environment condition was selected.
        void ecSelected(REnvironmentConditionType type, bool applied);

    protected slots:

        //! Called on entity selection changed.
        void onModelSelectionChanged(uint modelID);

        //! Called on problem mask changed.
        void onProblemChanged(uint modelID);

        //! Called when data has been changed.
        void onItemChanged(QTreeWidgetItem *item, int column);

        //! Called when item selection has changed.
        void onItemSelectionChanged(void);

        //! Called when ecSelected signal is emitted.
        void onEcSelected(void);

};

#endif /* __EC_MANAGER_TREE_H__ */
