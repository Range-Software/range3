/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   material_tree.h                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   13-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Material tree class declaration                     *
 *********************************************************************/

#ifndef MATERIAL_TREE_H
#define MATERIAL_TREE_H

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
        explicit MaterialTree(QWidget *parent = nullptr);

    protected:

        //! Populate model.
        void populate(void);

    public slots:

        //! Catch materialSelected signal.
        void onMaterialSelected(const QString &materialName);

    protected slots:

        //! Catch onCurrentItemChanged signal.
        void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *);

        //! Catch valueChanged signal from MaterialPropertyLineEditor.
        void onValueChanged(RMaterialPropertyType type, double value);

        //! Edit values button was clicked.
        void onButtonValueClicked(int id);

        //! Add new property.
        void onPropertyAdd(void);

        //! Delete selected property.
        void onPropertyDelete(void);
        
};

#endif // MATERIAL_TREE_H
