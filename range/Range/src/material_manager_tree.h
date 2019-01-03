/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_manager_tree.h                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   8-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Material manager tree class declaration             *
 *********************************************************************/

#ifndef MATERIAL_MANAGER_TREE_H
#define MATERIAL_MANAGER_TREE_H

#include <QTreeWidget>

#include <rmlib.h>

#include "action_list.h"

class MaterialManagerTree : public QTreeWidget
{
    Q_OBJECT

    protected:

        //! List of actions.
        ActionList *actionList;
        //! Problem type mask.
        RProblemTypeMask problemTypeMask;
        //! If set to true populat function will return immediately.
        bool doNotPopulate;

    public:

        //! Constructor.
        explicit MaterialManagerTree(ActionList *actionList,
                                     QWidget *parent = nullptr);

    protected:

        //! Populate tree.
        void populate(void);

        //! Update check states
        void updateCheckStates(void);

        //! Update problem type mask.
        void updateProblemTypeMask(void);

        //! Set item valid.
        void setItemValid(QTreeWidgetItem *item, bool valid);

        //! Add new material to the tree.
        void addMaterial(const QString &materialName, bool setSelected);

    signals:

        //! Signal that material selection has changed.
        void materialSelected(const QString &materialName);

    private slots:

        //! Called on entity selection changed.
        void onModelSelectionChanged(uint);

        //! Called when new model is added to session.
        void onModelAdded(uint modelID);

        //! Called when problem is changed.
        void onProblemChanged(uint);

        //! Catch materialAdded signal.
        void onMaterialAdded(const QString &materialName, bool isNew);

        //! Catch materialRemoved signal.
        void onMaterialRemoved(const QString &, uint removedID);

        //! Catch materialRenamed signal.
        void onMaterialRenamed(const QString &materialOldName,
                               const QString &materialNewName);

        //! Catch materialChanged signal.
        void onMaterialChanged(const QString &materialName);

        //! Called when data has been changed.
        void onItemChanged(QTreeWidgetItem *item, int);

        //! Catch doubleClicked signal.
        void onItemDoubleClicked(QTreeWidgetItem *item, int column);

        //! Catch itemSelectionChanged signal.
        void onItemSelectionChanged(void);

        //! Called when materialSelected signal is emitted.
        void onMaterialSelected(void);
    
};

#endif /* MATERIAL_MANAGER_TREE_H */
