/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   point_inside_surface_dialog.h                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   27-th March 2015                                         *
 *                                                                   *
 *  DESCRIPTION: Check if point is inside surface dialog class       *
 *               declaration                                         *
 *********************************************************************/

#ifndef __POINT_INSIDE_SURFACE_DIALOG_H__
#define __POINT_INSIDE_SURFACE_DIALOG_H__

#include <QDialog>

#include "session_entity_id.h"
#include "position_widget.h"

class PointInsideSurfaceDialog : public QDialog
{

    Q_OBJECT

    protected:

        //! Model ID.
        uint modelID;
        //! Selected entity IDs.
        const QList<SessionEntityID> &entityIDs;
        //! Point position widget.
        PositionWidget *position;

    public:

        //! Constructor.
        explicit PointInsideSurfaceDialog(uint modelID, const QList<SessionEntityID> &entityIDs, QWidget *parent = 0);

        //! Execute dialog.
        int exec(void);

};

#endif // __POINT_INSIDE_SURFACE_DIALOG_H__
