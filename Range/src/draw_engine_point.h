/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_point.h                                      *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th October 2014                                        *
 *                                                                   *
 *  DESCRIPTION: Draw engine point class declaration                 *
 *********************************************************************/

#ifndef __DRAW_ENGINE_POINT_H__
#define __DRAW_ENGINE_POINT_H__

#include "draw_engine_object.h"

class DrawEnginePoint : public DrawEngineObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DrawEnginePoint(QObject *parent = 0);

    protected:

        //! Generate RAW model from input parameters.
        virtual RModelRaw generate(void) const;

};

#endif // __DRAW_ENGINE_POINT_H__
