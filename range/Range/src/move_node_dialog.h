/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   move_node_dialog.h                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   20-th February 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Move node dialog class declaration                  *
 *********************************************************************/

#ifndef __MOVE_NODE_DIALOG_Hv
#define MOVE_NODE_DIALOG_H

#include <QDialog>
#include <QPushButton>

#include <rblib.h>

#include "position_widget.h"
#include "session_node_id.h"

class MoveNodeDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Mode ID.
        uint modelID;
        //! List of node IDs.
        const QList<uint> &nodeIDs;
        //! Possition widgets.
        QList<PositionWidget*> positionWidgets;
        //! OK button.
        QPushButton *okButton;

    public:

        //! Constructor.
        explicit MoveNodeDialog(uint modelID, const QList<uint> &nodeIDs, QWidget *parent = nullptr);

        //! Execute dialog.
        int exec(void);

    protected:

        //! Return map of node IDs to new positions.
        QMap<SessionNodeID,RR3Vector> findNodeIdPositionMap(void) const;

    private slots:

        //! Position changed.
        void onPositionChanged(const RR3Vector &);

};

#endif // MOVE_NODE_DIALOG_H
