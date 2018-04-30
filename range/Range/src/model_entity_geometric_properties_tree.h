/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   model_entity_geometric_properties_tree.h                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   27-th June 2013                                          *
 *                                                                   *
 *  DESCRIPTION: Entity geometric properties tree class declaration  *
 *********************************************************************/

#ifndef __MODEL_ENTITY_GEOMETRIC_PROPERTIES_TREE_H__
#define __MODEL_ENTITY_GEOMETRIC_PROPERTIES_TREE_H__

#include <QTreeWidget>

class ModelEntityGeometricPropertiesTree : public QTreeWidget
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit ModelEntityGeometricPropertiesTree(QWidget *parent = 0);

    protected:

        //! Populate model.
        void populate(void);

    private slots:

        //! Model selection has changed.
        void onModelSelectionChanged(uint);

        //! Item data has changed.
        void onItemChanged(QTreeWidgetItem *item, int column);

        //! Catch double click signal.
        void onItemDoubleClicked(QTreeWidgetItem *item, int column);
        
};

#endif // __MODEL_ENTITY_GEOMETRIC_PROPERTIES_TREE_H__
