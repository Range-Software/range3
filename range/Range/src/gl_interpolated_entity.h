/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_interpolated_entity.h                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   11-th June 2013                                          *
 *                                                                   *
 *  DESCRIPTION: OpenGL interpolated entity class declaration        *
 *********************************************************************/

#ifndef GL_INTERPOLATED_ENTITY_H
#define GL_INTERPOLATED_ENTITY_H

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
        void _init(const GLInterpolatedEntity *pGlInterpolatedEntity = nullptr);

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

#endif // GL_INTERPOLATED_ENTITY_H
