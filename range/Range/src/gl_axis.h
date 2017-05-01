/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_axis.h                                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   23-rd February 2012                                      *
 *                                                                   *
 *  DESCRIPTION: OpenGL axis class declaration                       *
 *********************************************************************/

#ifndef __GL_AXIS_H__
#define __GL_AXIS_H__

#include "gl_object.h"

typedef enum _GLAxisType
{
    GL_AXIS_NONE = 0,
    GL_AXIS_GLOBAL,
    GL_AXIS_LOCAL,
    GL_AXIS_POSITION,
    GL_AXIS_N_TYPES
} GLAxisType;

class GLAxis : public GLObject
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

        //! Axis type.
        GLAxisType type;
        //! Axis size.
        float size;

    private:

        //! Internal initialization function.
        void _init ( const GLAxis *pGlAxis = 0 );

    public:

        //! Constructor.
        explicit GLAxis(GLWidget *glWidget, GLAxisType type = GL_AXIS_NONE);

        //! Copy constructor.
        GLAxis(const GLAxis &glAxis);

        //! Destructor.
        ~GLAxis();

        //! Assignment operator.
        GLAxis & operator = (const GLAxis &glAxis);

        //! Return axis type.
        GLAxisType getType(void) const;

        //! Set axis type.
        void setType(GLAxisType type);

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

#endif /* __GL_AXIS_H__ */
