/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_manager_tree.h                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   8-th August 2012                                         *
 *                                                                   *
 *  DESCRIPTION: Material manager tree class declaration             *
 *********************************************************************/

#ifndef __MATERIAL_MANAGER_TREE_H__
#define __MATERIAL_MANAGER_TREE_H__

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
                                     QWidget *parent = 0);

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
        void onModelSelectionChanged(uint modelID);

        //! Called when new model is added to session.
        void onModelAdded(uint modelID);

        //! Called when problem is changed.
        void onProblemChanged(uint modelID);

        //! Catch materialAdded signal.
        void onMaterialAdded(const QString &materialName, bool isNew);

        //! Catch materialRemoved signal.
        void onMaterialRemoved(const QString &materialName,uint removedID);

        //! Catch materialRenamed signal.
        void onMaterialRenamed(const QString &materialOldName,
                               const QString &materialNewName);

        //! Catch materialChanged signal.
        void onMaterialChanged(const QString &materialName);

        //! Called when data has been changed.
        void onItemChanged(QTreeWidgetItem *item, int column);

        //! Catch doubleClicked signal.
        void onItemDoubleClicked(QTreeWidgetItem *item, int column);

        //! Catch itemSelectionChanged signal.
        void onItemSelectionChanged(void);

        //! Called when materialSelected signal is emitted.
        void onMaterialSelected(void);
    
};

#endif /* __MATERIAL_MANAGER_TREE_H__ */
