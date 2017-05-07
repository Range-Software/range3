/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_dimension.cpp                                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   6-th May 2017                                            *
 *                                                                   *
 *  DESCRIPTION: OpenGL dimension class definition                   *
 *********************************************************************/

#include "gl_arrow.h"
#include "gl_dimension.h"

void GLDimension::_init(const GLDimension *pGlDimension)
{
    if (pGlDimension)
    {

    }
}

GLDimension::GLDimension(GLWidget *glWidget,
                         double xMin,
                         double xMax,
                         double yMin,
                         double yMax,
                         double zMin,
                         double zMax)
    : GLObject(glWidget)
    , xMin(xMin)
    , xMax(xMax)
    , yMin(yMin)
    , yMax(yMax)
    , zMin(zMin)
    , zMax(zMax)
{
    this->_init();
}

GLDimension::GLDimension(const GLDimension &glDimension)
    : GLObject(glWidget)
{
    this->_init(&glDimension);
}

GLDimension::~GLDimension()
{

}

GLDimension &GLDimension::operator=(const GLDimension &glDimension)
{
    this->_init(&glDimension);
    return (*this);
}

void GLDimension::initialize(void)
{

}

void GLDimension::finalize(void)
{

}

void GLDimension::draw(void)
{
    RR3Vector position(xMin,yMin,zMin);
    RR3Vector direction(xMax-xMin,0.0,0.0);

    GLArrow a1(this->getGLWidget(),position,direction,true,true);
    a1.paint();
}
