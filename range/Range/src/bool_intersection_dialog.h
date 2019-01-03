/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   bool_intersection_dialog.h                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th March 2015                                         *
 *                                                                   *
 *  DESCRIPTION: Bool operation - intersection dialog class          *
 *               declaration                                         *
 *********************************************************************/

#ifndef BOOL_INTERSECTION_DIALOG_H
#define BOOL_INTERSECTION_DIALOG_H

#include <QDialog>

#include "session_entity_id.h"

class BoolIntersectionDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Model ID.
        uint modelID;
        //! Selected entity IDs.
        const QList<SessionEntityID> &entityIDs;

    public:

        //! Constructor.
        explicit BoolIntersectionDialog(uint modelID, const QList<SessionEntityID> &entityIDs, QWidget *parent = nullptr);

        //! Execute dialog.
        int exec(void);

    signals:

    public slots:

};

#endif // BOOL_INTERSECTION_DIALOG_H
