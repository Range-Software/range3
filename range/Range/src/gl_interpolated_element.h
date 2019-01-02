/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_interpolated_element.h                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   14-th June 2013                                          *
 *                                                                   *
 *  DESCRIPTION: OpenGL interpolated element class declaration       *
 *********************************************************************/

#ifndef GL_INTERPOLATED_ELEMENT_H
#define GL_INTERPOLATED_ELEMENT_H

#include <rmlib.h>

#include "gl_element_base.h"

class GLInterpolatedElement : public GLElementBase, public RInterpolatedElement
{

    private:

        //! Internal initialization function.
        void _init(const GLInterpolatedElement *pGlElement = nullptr);

    public:

        //! Constructor.
        GLInterpolatedElement(GLWidget *glWidget,
                              const Model *pModel,
                              const RInterpolatedElement &iElement,
                              uint elementID,
                              const REntityGroupData &elementGroupData,
                              GLElementDrawMode drawMode = GL_ELEMENT_DRAW_NORMAL);

        //! Copy constructor.
        GLInterpolatedElement(const GLInterpolatedElement &glElement);

        //! Destructor.
        ~GLInterpolatedElement();

        //! Assignment operator.
        GLInterpolatedElement & operator = (const GLInterpolatedElement &glElement);

    protected:

        //! Draw scene.
        void draw(void);

        //! Draw normal scene.
        void drawNormal(void);

};

#endif // GL_INTERPOLATED_ELEMENT_H
