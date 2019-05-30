/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_arrow.h                                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   30-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: OpenGL arrow class declaration                      *
 *********************************************************************/

#ifndef GL_ARROW_H
#define GL_ARROW_H

#include <rblib.h>

#include "gl_object.h"

class GLArrow : public GLObject
{

    private:

        // Environment settings
        GLboolean normalizeEnabled;
        GLboolean cullFaceEnabled;
        GLfloat lineWidth;

    protected:

        //! Vector position.
        RR3Vector position;
        //! Vector direction.
        RR3Vector direction;
        //! Draw tip.
        bool showHead;
        //! Draw from.
        bool drawFrom;
        //! Scale.
        double scale;

    private:

        //! Internal initialization function.
        void _init ( const GLArrow *pGlArrow = nullptr );

    public:

        //! Constructor.
        explicit GLArrow(GLWidget *glWidget, const RR3Vector &position, const RR3Vector &direction, bool showHead, bool drawFrom, double scale = 1.0);

        //! Copy constructor.
        GLArrow(const GLArrow &glArrow);

        //! Destructor.
        virtual ~GLArrow() {}

        //! Assignment operator.
        GLArrow & operator = (const GLArrow &glArrow);

    protected:

        //! Initialize scene.
        void initialize(void);

        //! Finalize scene.
        void finalize(void);

        //! Draw scene.
        void draw(void);

        //! Draw arrow shaft.
        void drawShaft(void) const;

        //! Draw arrow head.
        void drawHead(void);

};

#endif // GL_ARROW_H
