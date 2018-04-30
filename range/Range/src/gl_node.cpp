/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_node.cpp                                              *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   2-nd March 2012                                          *
 *                                                                   *
 *  DESCRIPTION: OpenGL node class definition                        *
 *********************************************************************/

#include "gl_node.h"

GLNode::GLNode(GLWidget *glWidget) : GLObject(glWidget)
{
    this->_init();
}

GLNode::GLNode(const GLNode &glNode)
    : GLObject(glNode)
    , RNode(glNode)
{
    this->_init(&glNode);
}

GLNode::~GLNode()
{
}

GLNode &GLNode::operator =(const GLNode &glNode)
{
    this->GLObject::operator =(glNode);
    this->RNode::operator =(glNode);
    this->_init(&glNode);
    return (*this);
}

void GLNode::_init(const GLNode *pGlNode)
{
    if (pGlNode)
    {

    }
}

void GLNode::initialize(void)
{
    GL_SAFE_CALL(glGetBooleanv(GL_LIGHTING, &this->lightingEnabled));
    GL_SAFE_CALL(glGetFloatv(GL_POINT_SIZE, &this->pointSize));

    GL_SAFE_CALL(glDisable(GL_LIGHTING));
    GL_SAFE_CALL(glPointSize(10.0f));
}

void GLNode::finalize(void)
{
    if (this->lightingEnabled)
    {
        GL_SAFE_CALL(glEnable(GL_LIGHTING));
    }
    GL_SAFE_CALL(glPointSize(this->pointSize));
}

void GLNode::draw(void)
{
    GLFunctions::begin(GL_POINTS);
    GL_SAFE_CALL(glVertex3d(this->getX(),this->getY(),this->getZ()));
    GLFunctions::end();
}
