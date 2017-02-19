/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_point.h                                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   18-th February 2014                                      *
 *                                                                   *
 *  DESCRIPTION: OpenGL point class declaration                      *
 *********************************************************************/

#ifndef __GL_POINT_H__
#define __GL_POINT_H__

#include <rblib.h>

#include "gl_object.h"

class GLPoint : public GLObject
{

    private:

        // Environment settings
        GLfloat prevPointSize;
        GLboolean lightingEnabled;

    protected:

        //! Point position.
        RR3Vector position;
        //! Point size.
        float pointSize;

    private:

        //! Internal initialization function.
        void _init ( const GLPoint *pGlPoint = 0 );

    public:

        //! Constructor.
        explicit GLPoint(GLWidget *glWidget, const RR3Vector &position, float pointSize);

        //! Copy constructor.
        GLPoint(const GLPoint &glPoint);

        //! Destructor.
        ~GLPoint();

        //! Assignment operator.
        GLPoint & operator = (const GLPoint &glPoint);

    protected:

        //! Initialize scene.
        void initialize(void);

        //! Finalize scene.
        void finalize(void);

        //! Draw scene.
        void draw(void);

};

#endif // __GL_POINT_H__
