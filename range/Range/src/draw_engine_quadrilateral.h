/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_quadrilateral.h                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th October 2014                                        *
 *                                                                   *
 *  DESCRIPTION: Draw engine quadrilateral class declaration         *
 *********************************************************************/

#ifndef DRAW_ENGINE_QUADRILATERAL_H
#define DRAW_ENGINE_QUADRILATERAL_H

#include "draw_engine_object.h"

class DrawEngineQuadrilateral : public DrawEngineObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DrawEngineQuadrilateral(QObject *parent = nullptr);

    protected:

        //! Generate RAW model from input parameters.
        virtual RModelRaw generate() const;

};

#endif // DRAW_ENGINE_QUADRILATERAL_H
