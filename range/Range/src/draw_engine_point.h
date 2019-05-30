/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_point.h                                      *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th October 2014                                        *
 *                                                                   *
 *  DESCRIPTION: Draw engine point class declaration                 *
 *********************************************************************/

#ifndef DRAW_ENGINE_POINT_H
#define DRAW_ENGINE_POINT_H

#include "draw_engine_object.h"

class DrawEnginePoint : public DrawEngineObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DrawEnginePoint(QObject *parent = nullptr);

    protected:

        //! Generate RAW model from input parameters.
        virtual RModelRaw generate() const;

};

#endif // DRAW_ENGINE_POINT_H
