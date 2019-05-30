/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_dimension.h                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th May 2017                                            *
 *                                                                   *
 *  DESCRIPTION: OpenGL dimension class declaration                  *
 *********************************************************************/

#ifndef GL_DIMENSION_H
#define GL_DIMENSION_H

#include "gl_object.h"


class GLDimension : public GLObject
{

    private:

        // Environment settings.

    public:

        static const double arrowScale;

    protected:

        double scale;
        double xMin;
        double xMax;
        double yMin;
        double yMax;
        double zMin;
        double zMax;

    private:

        //! Internal initialization function.
        void _init(const GLDimension *pGlDimension = nullptr);

    public:

        //! Constructor.
        explicit GLDimension(GLWidget *glWidget,
                             double scale,
                             double xMin,
                             double xMax,
                             double yMin,
                             double yMax,
                             double zMin,
                             double zMax);

        //! Copy constructor.
        GLDimension(const GLDimension &glDimension);

        //! Destructor.
        virtual ~GLDimension() {}

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

#endif // GL_DIMENSION_H
