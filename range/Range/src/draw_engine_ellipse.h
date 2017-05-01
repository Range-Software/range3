/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_ellipse.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th August 2016                                        *
 *                                                                   *
 *  DESCRIPTION: Draw engine ellipse class declaration               *
 *********************************************************************/

#ifndef __DRAW_ENGINE_ELLIPSE_H__
#define __DRAW_ENGINE_ELLIPSE_H__

#include "draw_engine_object.h"

class DrawEngineEllipse : public DrawEngineObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DrawEngineEllipse(QObject *parent = 0);

    protected:

        //! Generate RAW model from input parameters.
        virtual RModelRaw generate(void) const;

};

#endif // __DRAW_ENGINE_ELLIPSE_H__
