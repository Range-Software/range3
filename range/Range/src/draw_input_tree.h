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
        QList<uint> getSelectedObjectIDs() const;

        //! Set vector to requested item.
        void setRequestedItemVectorValue(const RR3Vector &v);

        //! Set text to requested item.
        void setRequestedItemTextValue(const QString &text);

        //! Release requested item.
        void releaseRequestedItem();

    protected:

        //! Populate the tree.
        void populate();

        //! Return item's object ID.
        uint getItemObjectID(QTreeWidgetItem *item) const;

        //! Set item vector.
        static void setItemVectorValue(QTreeWidgetItem *item, const RR3Vector &v);

        //! Set item vector.
        static RR3Vector getItemVectorValue(QTreeWidgetItem *item);

        //! Convert string into a vector.
        static RR3Vector stringToVector(const QString &vectorStr);

        //! Set item text.
        static void setItemTextValue(QTreeWidgetItem *item, const QString &text);

        //! Set item text.
        static QString getItemTextValue(QTreeWidgetItem *item);

    signals:

        //! Emit selection changed signal.
        void selectionChanged(const QList<uint>&objectIDs);

        //! Position request signal.
        void positionRequest(const RR3Vector &position);

        //! Text request signal.
        void textRequest(const QString &text);

        //! Input parameter changed.
        void inputParameterChanged(uint objectID, uint parameterID);

    protected slots:

        //! Draw object was added.
        void onDrawObjectAdded();

        //! Draw object was removed.
        void onDrawObjectRemoved(uint position);

        //! All draw object were removed.
        void onDrawObjectsRemoved();

        //! Catch double click signal.
        void onItemDoubleClicked(QTreeWidgetItem *item, int column);

        //! Catch double click signal.
        void onItemChanged(QTreeWidgetItem *item, int column);

        //! Catch item selection changed signal.
        void onItemSelectionChanged();

};

#endif // __DRAW_INPUT_TREE_H__
