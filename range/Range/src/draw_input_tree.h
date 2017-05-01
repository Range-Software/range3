/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_input_tree.h                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th September 2014                                     *
 *                                                                   *
 *  DESCRIPTION: Draw input tree class declaration                   *
 *********************************************************************/

#ifndef __DRAW_INPUT_TREE_H__
#define __DRAW_INPUT_TREE_H__

#include <QTreeWidget>

#include <rblib.h>

class DrawInputTree : public QTreeWidget
{

    Q_OBJECT

    protected:

        //! Request item.
        QTreeWidgetItem *requestedItem;

    public:

        //! Constructor.
        explicit DrawInputTree(QWidget *parent = 0);

        //! Return list of selected object IDs.
        QList<uint> getSelectedObjectIDs(void) const;

        //! Set vector to requested item.
        void setRequestedItemVectorValue(const RR3Vector &v);

        //! Release requested item.
        void releaseRequestedItem(void);

    protected:

        //! Populate the tree.
        void populate(void);

        //! Return item's object ID.
        uint getItemObjectID(QTreeWidgetItem *item) const;

        //! Set item vector.
        static void setItemVectorValue(QTreeWidgetItem *item, const RR3Vector &v);

        //! Set item vector.
        static RR3Vector getItemVectorValue(QTreeWidgetItem *item);

        //! Convert string into a vector.
        static RR3Vector stringToVector(const QString &vectorStr);

    signals:

        //! Emit selection changed signal.
        void selectionChanged(const QList<uint>&objectIDs);

        //! Position request signal.
        void positionRequest(const RR3Vector &position);

        //! Input parameter changed.
        void inputParameterChanged(uint objectID, uint parameterID);

    protected slots:

        //! Draw object was added.
        void onDrawObjectAdded(void);

        //! Draw object was removed.
        void onDrawObjectRemoved(uint position);

        //! All draw object were removed.
        void onDrawObjectsRemoved(void);

        //! Catch double click signal.
        void onItemDoubleClicked(QTreeWidgetItem *item, int column);

        //! Catch double click signal.
        void onItemChanged(QTreeWidgetItem *item, int column);

        //! Catch item selection changed signal.
        void onItemSelectionChanged(void);

};

#endif // __DRAW_INPUT_TREE_H__
