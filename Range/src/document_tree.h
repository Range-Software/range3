/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   document_tree.h                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   1-st October 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Document tree class declaration                     *
 *********************************************************************/

#ifndef __DOCUMENT_TREE_H__
#define __DOCUMENT_TREE_H__

#include <QTreeWidget>

class DocumentTree : public QTreeWidget
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DocumentTree(QWidget *parent = 0);

    private:

        //! Add document.
        void addDocument(QTreeWidgetItem *item, uint modelID, const QString &filename);
        
    protected slots:

        //! Populate.
        void populate(void);

        //! Update tree.
        void updateDocuments(void);

        //! Model has been added.
        void onModelAdded(uint modelID);

        //! Model has been changed.
        void onModelChanged(uint modelID);
        
        //! Model has been removed.
        void onModelRemoved(uint modelID);

        //! Catch tree-widget item doubleclick.
        void onItemDoubleClicked(QTreeWidgetItem *item, int column);

};

#endif // __DOCUMENT_TREE_H__
