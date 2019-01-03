/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   bool_difference_dialog.h                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th March 2015                                         *
 *                                                                   *
 *  DESCRIPTION: Bool operation - difference dialog class declaration*
 *********************************************************************/

#ifndef BOOL_DIFFERENCE_DIALOG_H
#define BOOL_DIFFERENCE_DIALOG_H

#include <QDialog>
#include <QListWidget>

#include "session_entity_id.h"

class BoolDifferenceDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Model ID.
        uint modelID;
        //! List of entity IDs where last one is cutting surface.
        QList<SessionEntityID> entityIDs;
        //! Entity list.
        QListWidget *listWidget;

    public:

        //! Constructor.
        explicit BoolDifferenceDialog(uint modelID, const QList<SessionEntityID> &entityIDs, QWidget *parent = nullptr);

        //! Execute dialog.
        int exec(void);

};

#endif // BOOL_DIFFERENCE_DIALOG_H
