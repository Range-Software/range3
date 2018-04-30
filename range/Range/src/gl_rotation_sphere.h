/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_rotation_sphere.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   29-th June 2017                                          *
 *                                                                   *
 *  DESCRIPTION: OpenGL rotation sphere class declaration            *
 *********************************************************************/

#ifndef __GL_ROTATION_SPHERE_H__
#define __GL_ROTATION_SPHERE_H__

#include "gl_object.h"

class GLRotationSphere : public GLObject
{

    private:

        // Environment settings
        GLboolean depthTestEnabled;
        GLboolean lineSmoothEnabled;
        GLint lineSmoothHint;
        GLboolean normalizeEnabled;
        GLboolean lightingEnabled;
        GLfloat lineWidth;
        GLboolean cullFaceEnabled;

    protected:

        //! Position.
        RR3Vector position;
        //! Scale.
        double scale;

    private:

        //! Internal initialization function.
        void _init(const GLRotationSphere *pGlRotationSphere = 0);

    public:

        //! Constructor.
        explicit GLRotationSphere(GLWidget *glWidget, const RR3Vector &position, double scale = 1.0);

        //! Copy constructor.
        GLRotationSphere(const GLRotationSphere &glRotationSphere);

        //! Destructor.
        ~GLRotationSphere();

        //! Assignment operator.
        GLRotationSphere &operator =(const GLRotationSphere &glRotationSphere);

    protected:

        //! Initialize scene.
        void initialize(void);

        //! Finalize scene.
        void finalize(void);

        //! Draw scene.
        void draw(void);

};

#endif // __GL_ROTATION_SPHERE_H__
