/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_tree.h                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   13-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Material tree class declaration                     *
 *********************************************************************/

#ifndef __MATERIAL_TREE_H__
#define __MATERIAL_TREE_H__

#include <QTreeWidget>
#include <QAction>

#include <rmlib.h>

class MaterialTree : public QTreeWidget
{

    Q_OBJECT

    protected:

        //! Material name.
        QString materialName;
        //! Add material property action.
        QAction *actionAddProperty;
        //! Remove material property action.
        QAction *actionRemoveProperty;

    public:

        //! Constructor.
        explicit MaterialTree(QWidget *parent = 0);

    protected:

        //! Populate model.
        void populate(void);

    public slots:

        //! Catch materialSelected signal.
        void onMaterialSelected(const QString &materialName);

    protected slots:

        //! Catch onCurrentItemChanged signal.
        void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

        //! Catch valueChanged signal from MaterialPropertyLineEditor.
        void onValueChanged(RMaterialPropertyType type, double value);

        //! Edit values button was clicked.
        void onButtonValueClicked(int id);

        //! Add new property.
        void onPropertyAdd(void);

        //! Delete selected property.
        void onPropertyDelete(void);
        
};

#endif // __MATERIAL_TREE_H__
