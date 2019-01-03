/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   model_tree.h                                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   25-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Model tree class declaration                        *
 *********************************************************************/

#ifndef MODEL_TREE_H
#define MODEL_TREE_H

#include <QTreeWidget>

#include <rmlib.h>

#include "action_list.h"

class ModelTree : public QTreeWidget
{

    Q_OBJECT

    private:

        //! Certain signals are ignored.
        bool ignoreSignals;

    public:
        explicit ModelTree(ActionList *actionList,
                           QObject    *parent = nullptr);

        //! Return list of selected model IDs.
        QList<uint> getSelectedModelIDs(void) const;

        //! Ignore certain signals.
        void setIgnoreSignals(bool ignoreSignals);

        //! Populate.
        void populate(void);

    protected:

        //! Return all items in the tree widget.
        QList<QTreeWidgetItem*> getAllItems(QTreeWidgetItem *parent = nullptr) const;

        //! List of actions.
        ActionList *actionList;

        //! Select selected items in session.
        void selectSelected(void);

        //! Expand selected items in session.
        void expandSelected(void);

        //! Select selected items in session.
        void checkVisible(void);

        //! Insert model to the tree.
        //! If such model identified by model ID is already present in the tree it will be replaced.
        void insertModel(uint modelID);

        //! Remove model from the tree.
        void removeModel(uint modelID);

        //! Find model item.
        QTreeWidgetItem *findModelItem(uint modelID, bool create = false);

        //! Find entity groupt item.
        QTreeWidgetItem *findEntityGroupItem(QTreeWidgetItem *parentItem, int entityGroupType, bool create = false);

        //! Find entity item.
        QTreeWidgetItem *findEntityItem(QTreeWidgetItem *parentItem, uint entityID, bool create = false);

    public slots:

        //! New model has been added.
        void onModelAdded(uint modelID);

        //! Model has been removed.
        void onModelRemoved(uint modelID);

        //! Model has been changed.
        void onModelChanged(uint modelID);

    protected slots:

        //! Model selection has canged.
        void onSelectionChanged(void);

        //! Called when tree item gets changed.
        void onItemChanged (QTreeWidgetItem *item, int column);

//        //! Model selection has changed.
//        void onModelSelectionChanged(uint modelID);

        //! Specified entity was selected.
        void onEntitySelectionChanged(uint              modelID,
                                      REntityGroupType elementGrpType,
                                      uint              entityID,
                                      bool              selected);

        //! Specified entity visibility has changed.
        void onEntityVisibilityChanged(uint              modelID,
                                       REntityGroupType elementGrpType,
                                       uint              entityID,
                                       bool              visible);

        //! Data has changed.
        void onDataChanged(QModelIndex topleft,
                           QModelIndex bottomRight);

        //! Item was expanded.
        void onItemExpanded(QTreeWidgetItem *item);

        //! Catch doubleClicked signal.
        void onItemDoubleClicked(QTreeWidgetItem *item, int column);
};

#endif /* MODEL_TREE_H */
