/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_interpolated_entity.h                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   11-th June 2013                                          *
 *                                                                   *
 *  DESCRIPTION: OpenGL interpolated entity class declaration        *
 *********************************************************************/

#ifndef __GL_INTERPOLATED_ENTITY_H__
#define __GL_INTERPOLATED_ENTITY_H__

#include <rmlib.h>

#include "gl_entity.h"

class GLInterpolatedEntity : public GLEntity, public RInterpolatedEntity
{

    protected:

        // Environment settings
        GLboolean lightingEnabled;
        GLboolean normalize;
        GLfloat pointSize;
        GLfloat lineWidth;

    private:

        //! Internal initialization function.
        void _init(const GLInterpolatedEntity *pGlInterpolatedEntity = 0);

    public:

        //! Constructor.
        GLInterpolatedEntity(GLWidget *glWidget, const RInterpolatedEntity &iEntity, const SessionEntityID &entityID);

        //! Copy constructor.
        GLInterpolatedEntity(const GLInterpolatedEntity &glInterpolatedEntity);

        //! Destructor.
        ~GLInterpolatedEntity();

        //! Assignment operator.
        GLInterpolatedEntity & operator = (const GLInterpolatedEntity &glInterpolatedEntity);

    protected:

        //! Initialize scene.
        void initialize(void);

        //! Finalize scene.
        void finalize(void);

        //! Draw scene.
        void draw(void);

};

#endif // __GL_INTERPOLATED_ENTITY_H__
