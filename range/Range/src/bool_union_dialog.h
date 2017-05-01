/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   bool_union_dialog.h                                      *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th March 2015                                         *
 *                                                                   *
 *  DESCRIPTION: Bool operation - union dialog class declaration     *
 *********************************************************************/

#ifndef __BOOL_UNION_DIALOG_H__
#define __BOOL_UNION_DIALOG_H__

#include <QDialog>

#include "session_entity_id.h"

class BoolUnionDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Model ID.
        uint modelID;
        //! Selected entity IDs.
        const QList<SessionEntityID> &entityIDs;

    public:

        //! Constructor.
        explicit BoolUnionDialog(uint modelID, const QList<SessionEntityID> &entityIDs, QWidget *parent = 0);

        //! Execute dialog.
        int exec(void);

};

#endif // __BOOL_UNION_DIALOG_H__
