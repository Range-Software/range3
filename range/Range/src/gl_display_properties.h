/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_display_properties.h                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th April 2016                                          *
 *                                                                   *
 *  DESCRIPTION: OpenGL display properties class declaration         *
 *********************************************************************/

#ifndef GL_DISPLAY_PROPERTIES_H
#define GL_DISPLAY_PROPERTIES_H

#include <rmlib.h>

class GLDisplayProperties : public RGLDisplayProperties
{

    protected:

        //! File name.
        QString fileName;

    private:

        //! Internal initialization function.
        void _init(const GLDisplayProperties *pDisplayProperties = nullptr);

    public:

        //! Constructor.
        GLDisplayProperties();

        //! Copy constructor.
        GLDisplayProperties(const GLDisplayProperties &displayProperties);

        //! Destructor.
        ~GLDisplayProperties();

        //! Assignment operator.
        GLDisplayProperties & operator =(const GLDisplayProperties &displayProperties);

        //! Return const reference to file name.
        const QString &getFileName(void) const;

        //! Set file name.
        void setFileName(const QString &fileName);

        //! Load display prperties.
        void load(const QString &fileName);

        //! Store display properties.
        void store(void) const;

};

#endif // GL_DISPLAY_PROPERTIES_H
