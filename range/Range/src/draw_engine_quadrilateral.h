/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_quadrilateral.h                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th October 2014                                        *
 *                                                                   *
 *  DESCRIPTION: Draw engine quadrilateral class declaration         *
 *********************************************************************/

#ifndef __DRAW_ENGINE_QUADRILATERAL_H__
#define __DRAW_ENGINE_QUADRILATERAL_H__

#include "draw_engine_object.h"

class DrawEngineQuadrilateral : public DrawEngineObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DrawEngineQuadrilateral(QObject *parent = 0);

    protected:

        //! Generate RAW model from input parameters.
        virtual RModelRaw generate(void) const;

};

#endif // __DRAW_ENGINE_QUADRILATERAL_H__
