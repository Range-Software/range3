/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_node.h                                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   2-nd March 2012                                          *
 *                                                                   *
 *  DESCRIPTION: OpenGL node class declaration                       *
 *********************************************************************/

#ifndef __GL_NODE_H__
#define __GL_NODE_H__

#include <rmlib.h>

#include "gl_object.h"

class GLNode : public GLObject, public RNode
{

    protected:

        // Environment settings
        GLboolean lightingEnabled;
        GLfloat pointSize;

    private:

        //! Internal initialization function.
        void _init(const GLNode *pGlNode = 0);

    public:

        //! Constructor.
        GLNode(GLWidget *glWidget);

        //! Copy constructor.
        GLNode(const GLNode &glNode);

        //! Destructor.
        ~GLNode();

        //! Assignment operator.
        GLNode & operator = (const GLNode &glNode);

    protected:

        //! Initialize scene.
        void initialize(void);

        //! Finalize scene.
        void finalize(void);

        //! Draw scene.
        void draw(void);

};

#endif /* __GL_NODE_H__ */
