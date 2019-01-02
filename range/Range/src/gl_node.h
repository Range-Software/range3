/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_node.h                                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   2-nd March 2012                                          *
 *                                                                   *
 *  DESCRIPTION: OpenGL node class declaration                       *
 *********************************************************************/

#ifndef GL_NODE_H
#define GL_NODE_H

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
        void _init(const GLNode *pGlNode = nullptr);

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

#endif /* GL_NODE_H */
