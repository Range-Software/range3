/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   coarsen_surface_dialog.h                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   17-th March 2015                                         *
 *                                                                   *
 *  DESCRIPTION: Coarsen surface dialog class declaration            *
 *********************************************************************/

#ifndef COARSEN_SURFACE_DIALOG_H
#define COARSEN_SURFACE_DIALOG_H

#include <QDialog>

#include "session_entity_id.h"
#include "value_line_edit.h"

class CoarsenSurfaceDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Model ID.
        uint modelID;
        //! Selected entity IDs.
        const QList<SessionEntityID> &entityIDs;
        //! Edge length.
        ValueLineEdit *edgeLengthEdit;
        //! Element area.
        ValueLineEdit *elementAreaEdit;

    public:

        //! Constructor.
        explicit CoarsenSurfaceDialog(uint modelID, const QList<SessionEntityID> &entityIDs, QWidget *parent = nullptr);

        //! Execute dialog.
        int exec(void);

    protected:

        //! Find shortest edge and smallest area.
        void findGeometryLimits(void);

};

#endif // COARSEN_SURFACE_DIALOG_H
