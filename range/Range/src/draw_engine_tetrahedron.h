/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_tetrahedron.h                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   17-th Jun 2015                                           *
 *                                                                   *
 *  DESCRIPTION: Draw engine tetrahedron class declaration           *
 *********************************************************************/

#ifndef DRAW_ENGINE_TRAHEDRON_H
#define DRAW_ENGINE_TRAHEDRON_H

#include "draw_engine_object.h"

class DrawEngineTetrahedron : public DrawEngineObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DrawEngineTetrahedron(QObject *parent = nullptr);

    protected:

        //! Generate RAW model from input parameters.
        virtual RModelRaw generate() const;

};

#endif // DRAW_ENGINE_TRAHEDRON_H
