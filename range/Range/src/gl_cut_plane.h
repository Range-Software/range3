/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_cut_plane.h                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   28-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: OpenGL cut plane class declaration                  *
 *********************************************************************/

#ifndef __GL_CUT_PLANE_H__
#define __GL_CUT_PLANE_H__

#include <rblib.h>

#include "gl_object.h"

class GLCutPlane : public GLObject, public RPlane
{

    private:

        // Environment settings.
        GLboolean depthTestEnabled;
        GLboolean lineSmoothEnabled;
        GLint lineSmoothHint;
        GLboolean normalizeEnabled;
        GLboolean lightingEnabled;
        GLfloat lineWidth;
        GLboolean cullFaceEnabled;

    protected:

        //! Axis size.
        float size;

    private:

        //! Internal initialization function.
        void _init ( const GLCutPlane *pGlCutPlane = 0 );

    public:

        //! Constructor.
        explicit GLCutPlane(GLWidget *glWidget, const RPlane &plane);

        //! Copy constructor.
        GLCutPlane(const GLCutPlane &glCutPlane);

        //! Destructor.
        ~GLCutPlane();

        //! Assignment operator.
        GLCutPlane & operator = (const GLCutPlane &glCutPlane);

        //! Return axis size.
        float getSize(void) const;

        //! Set axis size.
        void setSize(float size);

    protected:

        //! Initialize scene.
        void initialize(void);

        //! Finalize scene.
        void finalize(void);

        //! Draw scene.
        void draw(void);

};

#endif // __GL_CUT_PLANE_H__
