/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_simplex_tetrahedra.h                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   12-th August 2015                                        *
 *                                                                   *
 *  DESCRIPTION: OpenGL simplex tetrahedra class declaration         *
 *********************************************************************/

#ifndef __GL_SIMPLEX_TETRAHEDRA_H__
#define __GL_SIMPLEX_TETRAHEDRA_H__

#include <vector>

#include "gl_simplex.h"

class GLSimplexTetrahedra : public GLSimplex
{

    private:

        //! Internal initialization function.
        void _init(const GLSimplexTetrahedra *pGlTetrahedra = 0 );

    public:

        //! Constructor.
        explicit GLSimplexTetrahedra(GLWidget *glWidget, const std::vector<RR3Vector> &nodes);

        //! Copy constructor.
        GLSimplexTetrahedra(const GLSimplexTetrahedra &glTetrahedra);

        //! Destructor.
        ~GLSimplexTetrahedra();

        //! Assignment operator.
        GLSimplexTetrahedra &operator =(const GLSimplexTetrahedra &glTetrahedra);

    protected:

        //! Initialize scene.
        void initialize(void);

        //! Finalize scene.
        void finalize(void);

        //! Draw scene.
        void draw(void);

        //! Draw normal.
        void drawNormal(bool useTexture);

        //! Draw wired.
        void drawWired(bool useTexture);

        //! Draw nodes.
        void drawNodes(void);

};

#endif // __GL_SIMPLEX_TETRAHEDRA_H__
