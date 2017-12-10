/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   model_entity_display_properties_tree.h                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   9-th February 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Entity display properties tree class declaration    *
 *********************************************************************/

#ifndef __MODEL_ENTITY_DISPLAY_PROPERTIES_TREE_H__
#define __MODEL_ENTITY_DISPLAY_PROPERTIES_TREE_H__

#include <QTreeWidget>

class ModelEntityDisplayPropertiesTree : public QTreeWidget
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit ModelEntityDisplayPropertiesTree(QWidget *parent = 0);

    protected:

        //! Populate model.
        void populate(void);

    private slots:

        //! Model selection has changed.
        void onModelSelectionChanged(uint);

        //! Model has changed.
        void onModelChanged(uint);

        //! Problem has changed.
        void onProblemChanged(uint);

        //! Item data has changed.
        void onItemChanged(QTreeWidgetItem *item, int column);

        //! Color has changed.
        void onColorChanged(QColor color);

};

#endif /* __MODEL_ENTITY_DISPLAY_PROPERTIES_TREE_H__ */
