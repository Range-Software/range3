/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_dimension.h                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th May 2017                                            *
 *                                                                   *
 *  DESCRIPTION: OpenGL dimension class declaration                  *
 *********************************************************************/

#ifndef __GL_DIMENSION_H__
#define __GL_DIMENSION_H__

#include "gl_object.h"


class GLDimension : public GLObject
{

    private:

        // Environment settings.

    protected:

        double xMin;
        double xMax;
        double yMin;
        double yMax;
        double zMin;
        double zMax;

    private:

        //! Internal initialization function.
        void _init(const GLDimension *pGlDimension = 0);

    public:

        //! Constructor.
        explicit GLDimension(GLWidget *glWidget,
                             double xMin,
                             double xMax,
                             double yMin,
                             double yMax,
                             double zMin,
                             double zMax);

        //! Copy constructor.
        GLDimension(const GLDimension &glDimension);

        //! Destructor.
        ~GLDimension();

        //! Assignment operator.
        GLDimension &operator=(const GLDimension &glDimension);

    protected:

        //! Initialize scene.
        void initialize(void);

        //! Finalize scene.
        void finalize(void);

        //! Draw scene.
        void draw(void);

};

#endif // __GL_DIMENSION_H__
