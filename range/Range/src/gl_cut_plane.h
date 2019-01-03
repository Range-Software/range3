/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_cut_plane.h                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   28-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: OpenGL cut plane class declaration                  *
 *********************************************************************/

#ifndef GL_CUT_PLANE_H
#define GL_CUT_PLANE_H

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
        void _init ( const GLCutPlane *pGlCutPlane = nullptr );

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

#endif // GL_CUT_PLANE_H
