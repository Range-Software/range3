/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_sphere.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th October 2014                                        *
 *                                                                   *
 *  DESCRIPTION: Draw engine sphere class declaration                *
 *********************************************************************/

#ifndef __DRAW_ENGINE_SPHERE_H__
#define __DRAW_ENGINE_SPHERE_H__

#include "draw_engine_object.h"

class DrawEngineSphere : public DrawEngineObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DrawEngineSphere(QObject *parent = 0);

        //! Generate sphere.
        static RModelRaw generate(const RR3Vector &center, double radius, uint nIter);

    protected:

        //! Generate RAW model from input parameters.
        virtual RModelRaw generate(void) const;

};

#endif // __DRAW_ENGINE_SPHERE_H__
