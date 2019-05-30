/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_triangle.h                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th October 2014                                        *
 *                                                                   *
 *  DESCRIPTION: Draw engine triangle class declaration              *
 *********************************************************************/

#ifndef DRAW_ENGINE_TRIANGLE_H
#define DRAW_ENGINE_TRIANGLE_H

#include "draw_engine_object.h"

class DrawEngineTriangle : public DrawEngineObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DrawEngineTriangle(QObject *parent = nullptr);

    protected:

        //! Generate RAW model from input parameters.
        virtual RModelRaw generate() const;

};

#endif // DRAW_ENGINE_TRIANGLE_H
