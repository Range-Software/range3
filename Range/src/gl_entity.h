/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_entity.h                                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   5-th June 2013                                           *
 *                                                                   *
 *  DESCRIPTION: OpenGL entity class declaration                     *
 *********************************************************************/

#ifndef __GL_ENTITY_H__
#define __GL_ENTITY_H__

#include "gl_object.h"
#include "gl_texture.h"
#include "model.h"
#include "session_entity_id.h"

class GLEntity : public GLObject
{

    protected:

        //! entity ID.
        SessionEntityID entityID;
        //! GL texture.
        GLTexture texture;

    private:

        //! Internal initialization function.
        void _init(const GLEntity *pGlEntity = 0);

    public:

        //! Constructor.
        GLEntity(GLWidget *glWidget, const SessionEntityID &entityID);

        //! Copy constructor.
        GLEntity(const GLEntity &glEntity);

        //! Destructor.
        ~GLEntity();

        //! Assignment operator.
        GLEntity & operator = (const GLEntity &glEntity);

        //! Return const reference to entity ID.
        const SessionEntityID &getEntityID(void) const;

    protected:

        //! Initialize scene.
        void initialize(void);

        //! Finalize scene.
        void finalize(void);

        //! Draw scene.
        void draw(void);
};

#endif // GL_ENTITY_H
