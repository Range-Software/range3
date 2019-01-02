/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_element.h                                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   2-nd March 2012                                          *
 *                                                                   *
 *  DESCRIPTION: OpenGL element class declaration                    *
 *********************************************************************/

#ifndef GL_ELEMENT_H
#define GL_ELEMENT_H

#include <rmlib.h>

#include "gl_object.h"
#include "gl_element_base.h"

class GLElement : public GLElementBase, public RElement
{

    protected:

        //! Point volume.
        double pointVolume;
        //! Line cross area.
        double lineCrossArea;
        //! Surface thickness.
        double surfaceThickness;

    private:

        //! Internal initialization function.
        void _init(const GLElement *pGlElement = nullptr);

    public:

        //! Constructor.
        GLElement(GLWidget *glWidget, const Model *pModel, uint elementID, const REntityGroupData &elementGroupData, const QColor &color, GLElementDrawMode drawMode = GL_ELEMENT_DRAW_NORMAL);

        //! Copy constructor.
        GLElement(const GLElement &glElement);

        //! Destructor.
        ~GLElement();

        //! Assignment operator.
        GLElement & operator = (const GLElement &glElement);

        //! Set point volume.
        void setPointVolume(double pointVolume);

        //! Set line cross area.
        void setLineCrossArea(double lineCrossArea);

        //! Set surface thickness.
        void setSurfaceThickness(double surfaceThickness);

    protected:

        //! Draw scene.
        void draw(void);

        //! Draw point.
        void drawPoint(void);

        //! Draw line.
        void drawLine(void);

        //! Draw triangle.
        void drawTriangle(void);

        //! Draw quadrilateral.
        void drawQuadrilateral(void);

        //! Draw tetrahedra.
        void drawTetrahedra(void);

};

#endif /* GL_ELEMENT_H */
