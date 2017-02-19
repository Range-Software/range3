/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   geometry_rotate_widget.h                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th November 2014                                       *
 *                                                                   *
 *  DESCRIPTION: Geometry rotate widget class declaration            *
 *********************************************************************/

#ifndef __GEOMETRY_ROTATE_WIDGET_H__
#define __GEOMETRY_ROTATE_WIDGET_H__

#include <QWidget>

#include <rblib.h>

#include "position_widget.h"
#include "value_line_edit.h"

class GeometryRotateWidget : public QWidget
{

    Q_OBJECT

    private:

        static const RR3Vector defaultAngles;

    protected:

        PositionWidget *rotationCenter;
        ValueLineEdit *xAngleLineEdit;
        ValueLineEdit *yAngleLineEdit;
        ValueLineEdit *zAngleLineEdit;

    public:

        //! Constructor.
        explicit GeometryRotateWidget(const RR3Vector &center = RR3Vector(), const RR3Vector &rotation = RR3Vector(), QWidget *parent = 0);

    signals:

        //! Rotation changed.
        void rotationChanged(const RR3Vector &center, const RR3Vector &angles);

    protected slots:

        //! Reset angles clicked.
        void resetAngles(void);

        //! Angle value changed.
        void onAngleValueChanged(double value);

        //! Position changed.
        void onPositionChanged(const RR3Vector &position);

};

#endif // __GEOMETRY_ROTATE_WIDGET_H__
