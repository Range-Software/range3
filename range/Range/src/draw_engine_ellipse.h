/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_ellipse.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th August 2016                                        *
 *                                                                   *
 *  DESCRIPTION: Draw engine ellipse class declaration               *
 *********************************************************************/

#ifndef DRAW_ENGINE_ELLIPSE_H
#define DRAW_ENGINE_ELLIPSE_H

#include "draw_engine_object.h"

class DrawEngineEllipse : public DrawEngineObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DrawEngineEllipse(QObject *parent = nullptr);

    protected:

        //! Generate RAW model from input parameters.
        virtual RModelRaw generate() const;

};

#endif // DRAW_ENGINE_ELLIPSE_H
