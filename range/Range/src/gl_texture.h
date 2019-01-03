/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_texture.h                                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   9-th May 2013                                            *
 *                                                                   *
 *  DESCRIPTION: OpenGL texture class declaration                    *
 *********************************************************************/

#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include <QOpenGLTexture>

#include <QString>
#include <QImage>

class GLTexture
{
    private:

        //! Texture.
        QOpenGLTexture *texture;
        //! Texture loaded flag.
        bool loaded;

    public:

        //! Constructor.
        GLTexture();

        //! Destructor.
        ~GLTexture();

        //! Load texture
        void load(const QString &file);

        //! Unload texture
        void unload(void);
};

#endif // GL_TEXTURE_H
