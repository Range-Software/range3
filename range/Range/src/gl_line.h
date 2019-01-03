/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_line.h                                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th February 2015                                       *
 *                                                                   *
 *  DESCRIPTION: OpenGL line class declaration                       *
 *********************************************************************/

#ifndef GL_LINE_H
#define GL_LINE_H

#include <rblib.h>

#include "gl_object.h"

class GLLine : public GLObject
{

    private:

        // Environment settings
        GLboolean normalizeEnabled;
        GLboolean lightingEnabled;
        GLfloat lineWidth;

    protected:

        //! Line start.
        RR3Vector start;
        //! Line end.
        RR3Vector end;
        //! Width;
        GLfloat width;

    private:

        //! Internal initialization function.
        void _init (const GLLine *pGlLine = nullptr );

    public:

        //! Constructor.
        explicit GLLine(GLWidget *glWidget, const RR3Vector &start, const RR3Vector &end, GLfloat width);

        //! Copy constructor.
        GLLine(const GLLine &glLine);

        //! Destructor.
        ~GLLine();

        //! Assignment operator.
        GLLine & operator =(const GLLine &glLine);

    protected:

        //! Initialize scene.
        void initialize(void);

        //! Finalize scene.
        void finalize(void);

        //! Draw scene.
        void draw(void);

};

#endif // GL_LINE_H
