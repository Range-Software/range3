/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_cylinder.h                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th October 2014                                        *
 *                                                                   *
 *  DESCRIPTION: Draw engine cylinder class declaration              *
 *********************************************************************/

#ifndef DRAW_ENGINE_CYLINDER_H
#define DRAW_ENGINE_CYLINDER_H

#include "draw_engine_object.h"

class DrawEngineCylinder : public DrawEngineObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DrawEngineCylinder(QObject *parent = nullptr);

    protected:

        //! Generate RAW model from input parameters.
        virtual RModelRaw generate() const;

};

#endif // DRAW_ENGINE_CYLINDER_H
