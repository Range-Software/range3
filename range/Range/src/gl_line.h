/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_line.h                                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th February 2015                                       *
 *                                                                   *
 *  DESCRIPTION: OpenGL line class declaration                       *
 *********************************************************************/

#ifndef __GL_LINE_H__
#define __GL_LINE_H__

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
        void _init (const GLLine *pGlLine = 0 );

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

#endif // __GL_LINE_H__
