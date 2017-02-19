/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_tetrahedron.h                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   17-th Jun 2015                                           *
 *                                                                   *
 *  DESCRIPTION: Draw engine tetrahedron class declaration           *
 *********************************************************************/

#ifndef __DRAW_ENGINE_TRAHEDRON_H__
#define __DRAW_ENGINE_TRAHEDRON_H__

#include "draw_engine_object.h"

class DrawEngineTetrahedron : public DrawEngineObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DrawEngineTetrahedron(QObject *parent = 0);

    protected:

        //! Generate RAW model from input parameters.
        virtual RModelRaw generate(void) const;

};

#endif // __DRAW_ENGINE_TRAHEDRON_H__
