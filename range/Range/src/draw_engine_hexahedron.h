/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_hexahedron.h                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th October 2014                                        *
 *                                                                   *
 *  DESCRIPTION: Draw engine hexahedron class declaration            *
 *********************************************************************/

#ifndef DRAW_ENGINE_HEXAHEDRON_H
#define DRAW_ENGINE_HEXAHEDRON_H

#include "draw_engine_object.h"

class DrawEngineHexahedron : public DrawEngineObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DrawEngineHexahedron(QObject *parent = nullptr);

    protected:

        //! Generate RAW model from input parameters.
        virtual RModelRaw generate() const;

};

#endif // DRAW_ENGINE_HEXAHEDRON_H
