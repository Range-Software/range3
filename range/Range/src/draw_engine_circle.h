/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_circle.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   12-th February 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Draw engine circle class declaration                *
 *********************************************************************/

#ifndef __DRAW_ENGINE_CIRCLE_H__
#define __DRAW_ENGINE_CIRCLE_H__

#include "draw_engine_object.h"

class DrawEngineCircle : public DrawEngineObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DrawEngineCircle(QObject *parent = 0);

    protected:

        //! Generate RAW model from input parameters.
        virtual RModelRaw generate(void) const;

};

#endif // __DRAW_ENGINE_CIRCLE_H__
