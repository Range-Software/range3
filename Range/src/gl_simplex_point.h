/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_simplex_point.h                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   14-th August 2015                                        *
 *                                                                   *
 *  DESCRIPTION: OpenGL simplex point class declaration              *
 *********************************************************************/

#ifndef __GL_SIMPLEX_POINT_H__
#define __GL_SIMPLEX_POINT_H__

#include "gl_simplex.h"

class GLSimplexPoint : public GLSimplex
{

    protected:

        //! Volume.
        double volume;

    private:

        //! Internal initialization function.
        void _init(const GLSimplexPoint *pGlPoint = 0 );

    public:

        //! Constructor.
        explicit GLSimplexPoint(GLWidget *glWidget, const std::vector<RR3Vector> &nodes, double volume = 0.0);

        //! Copy constructor.
        GLSimplexPoint(const GLSimplexPoint &glPoint);

        //! Destructor.
        ~GLSimplexPoint();

        //! Assignment operator.
        GLSimplexPoint &operator =(const GLSimplexPoint &glPoint);

    protected:

        //! Initialize scene.
        void initialize(void);

        //! Finalize scene.
        void finalize(void);

        //! Draw scene.
        void draw(void);

        //! Draw normal.
        void drawNormal(const RModelRaw &sphereModel,
                        bool volumeElement,
                        bool useTexture);

        //! Draw wired.
        void drawWired(const RModelRaw &sphereModel,
                       bool volumeElement,
                       bool useTexture);

        //! Draw nodes.
        void drawNodes(const RModelRaw &sphereModel,
                       bool volumeElement);

};

#endif // __GL_SIMPLEX_POINT_H__
