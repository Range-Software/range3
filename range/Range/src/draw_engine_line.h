/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_line.h                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th October 2014                                        *
 *                                                                   *
 *  DESCRIPTION: Draw engine line class declaration                  *
 *********************************************************************/

#ifndef __DRAW_ENGINE_LINE_H__
#define __DRAW_ENGINE_LINE_H__

#include "draw_engine_object.h"

class DrawEngineLine : public DrawEngineObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DrawEngineLine(QObject *parent = 0);

    protected:

        //! Generate RAW model from input parameters.
        virtual RModelRaw generate(void) const;

};

#endif // __DRAW_ENGINE_LINE_H__
