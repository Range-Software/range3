/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_entity.cpp                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   5-th June 2013                                           *
 *                                                                   *
 *  DESCRIPTION: OpenGL entity class declaration                     *
 *********************************************************************/

#include "gl_entity.h"

void GLEntity::_init(const GLEntity *pGLEntity)
{
    if (pGLEntity)
    {
        this->entityID = pGLEntity->entityID;
    }
}

GLEntity::GLEntity(GLWidget *glWidget, const SessionEntityID &entityID)
    : GLObject(glWidget)
    , entityID(entityID)
{
    this->_init();
}

GLEntity::GLEntity(const GLEntity &gLEntity)
    : GLObject(gLEntity)
{
    this->_init(&gLEntity);
}

GLEntity::~GLEntity()
{
}

GLEntity &GLEntity::operator =(const GLEntity &gLEntity)
{
    this->GLObject::operator =(gLEntity);
    this->_init(&gLEntity);
    return (*this);
}

const SessionEntityID & GLEntity::getEntityID(void) const
{
    return this->entityID;
}

void GLEntity::initialize(void)
{
}

void GLEntity::finalize(void)
{
}

void GLEntity::draw(void)
{
}
