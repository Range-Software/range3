/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_point.cpp                                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   18-th February 2014                                      *
 *                                                                   *
 *  DESCRIPTION: OpenGL point class definition                       *
 *********************************************************************/

#include "gl_point.h"

void GLPoint::_init(const GLPoint *pGlPoint)
{
    if (pGlPoint)
    {
        this->position = pGlPoint->position;
        this->pointSize = pGlPoint->pointSize;
    }
}

GLPoint::GLPoint(GLWidget *glWidget, const RR3Vector &position, float pointSize)
    : GLObject(glWidget)
    , position(position)
    , pointSize(pointSize)
{
    this->_init();
}

GLPoint::GLPoint(const GLPoint &glPoint)
    : GLObject(glPoint)
{
    this->_init(&glPoint);
}

GLPoint::~GLPoint()
{
}

GLPoint &GLPoint::operator =(const GLPoint &glPoint)
{
    this->GLObject::operator =(glPoint);
    this->_init(&glPoint);
    return (*this);
}

void GLPoint::initialize(void)
{
    // Save current settings
    GL_SAFE_CALL(glGetFloatv(GL_POINT_SIZE, &this->prevPointSize));
    GL_SAFE_CALL(glGetBooleanv(GL_LIGHTING, &this->lightingEnabled));
    // Initialize environment
    GL_SAFE_CALL(glPointSize(this->pointSize));
    GL_SAFE_CALL(glDisable(GL_LIGHTING));
}

void GLPoint::finalize(void)
{
    GL_SAFE_CALL(glPointSize(this->prevPointSize));

    if (this->lightingEnabled)
    {
        GL_SAFE_CALL(glEnable(GL_LIGHTING));
    }
    else
    {
        GL_SAFE_CALL(glDisable(GL_LIGHTING));
    }
}

void GLPoint::draw(void)
{
    GLFunctions::begin(GL_POINTS);
    GL_SAFE_CALL(glVertex3d(this->position[0],this->position[1],this->position[2]));
    GLFunctions::end();
}
