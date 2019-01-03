/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_rotation_sphere.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   29-th June 2017                                          *
 *                                                                   *
 *  DESCRIPTION: OpenGL rotation sphere class declaration            *
 *********************************************************************/

#ifndef GL_ROTATION_SPHERE_H
#define GL_ROTATION_SPHERE_H

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
        void _init(const GLRotationSphere *pGlRotationSphere = nullptr);

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

#endif // GL_ROTATION_SPHERE_H
