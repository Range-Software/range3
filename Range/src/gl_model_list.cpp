/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_model_list.cpp                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   17-th May 2012                                           *
 *                                                                   *
 *  DESCRIPTION: OpenGL Model list class definition                  *
 *********************************************************************/

#include "gl_model_list.h"

void GLModelList::_init(const GLModelList *pGlModelList)
{
    this->clear();

    if (pGlModelList)
    {
        this->glPointList = pGlModelList->glPointList;
        this->glLineList = pGlModelList->glLineList;
        this->glSurfaceList = pGlModelList->glSurfaceList;
        this->glVolumeList = pGlModelList->glVolumeList;
        this->glVectorFieldList = pGlModelList->glVectorFieldList;
        this->glScalarFieldList = pGlModelList->glScalarFieldList;
        this->glStreamLineList = pGlModelList->glStreamLineList;
        this->glCutList = pGlModelList->glCutList;
        this->glIsoList = pGlModelList->glIsoList;
    }
}

GLModelList::GLModelList()
{
    this->_init();
}

GLModelList::GLModelList(const GLModelList &glModelList)
{
    this->_init(&glModelList);
}

GLModelList::~GLModelList()
{
}

GLModelList & GLModelList::operator =(const GLModelList &glModelList)
{
    this->_init(&glModelList);
    return (*this);
}

void GLModelList::clear(void)
{
    this->glPointList.clear();
    this->glLineList.clear();
    this->glSurfaceList.clear();
    this->glVolumeList.clear();
    this->glVectorFieldList.clear();
    this->glScalarFieldList.clear();
    this->glStreamLineList.clear();
    this->glCutList.clear();
    this->glIsoList.clear();
}

uint GLModelList::getNGlPointLists(void) const
{
    return (uint)this->glPointList.size();
}

void GLModelList::setNGlPointLists(uint nGlEntityLists)
{
    if (this->glPointList.size() != int(nGlEntityLists))
    {
        this->glPointList.clear();
        this->glPointList.resize(nGlEntityLists);
    }
}

const GLEntityList & GLModelList::getGlPointList(uint entityID) const
{
    R_ERROR_ASSERT(entityID < this->getNGlPointLists());
    return this->glPointList[entityID];
}

GLEntityList &GLModelList::getGlPointList(uint entityID)
{
    R_ERROR_ASSERT(entityID < this->getNGlPointLists());
    return this->glPointList[entityID];
}

void GLModelList::setGlPointList(uint entityID, const GLEntityList &glEntityList)
{
    R_ERROR_ASSERT(entityID < this->getNGlPointLists());
    this->glPointList[entityID] = glEntityList;
}

uint GLModelList::getNGlLineLists(void) const
{
    return (uint)this->glLineList.size();
}

void GLModelList::setNGlLineLists(uint nGlEntityLists)
{
    if (this->glLineList.size() != int(nGlEntityLists))
    {
        this->glLineList.clear();
        this->glLineList.resize(nGlEntityLists);
    }
}

const GLEntityList &GLModelList::getGlLineList(uint entityID) const
{
    R_ERROR_ASSERT(entityID < this->getNGlLineLists());
    return this->glLineList[entityID];
}

GLEntityList &GLModelList::getGlLineList(uint entityID)
{
    R_ERROR_ASSERT(entityID < this->getNGlLineLists());
    return this->glLineList[entityID];
}

void GLModelList::setGlLineList(uint entityID, const GLEntityList &glEntityList)
{
    R_ERROR_ASSERT(entityID < this->getNGlLineLists());
    this->glLineList[entityID] = glEntityList;
}

uint GLModelList::getNGlSurfaceLists(void) const
{
    return (uint)this->glSurfaceList.size();
}

void GLModelList::setNGlSurfaceLists(uint nGlEntityLists)
{
    if (this->glSurfaceList.size() != int(nGlEntityLists))
    {
        this->glSurfaceList.clear();
        this->glSurfaceList.resize(nGlEntityLists);
    }
}

const GLEntityList &GLModelList::getGlSurfaceList(uint entityID) const
{
    R_ERROR_ASSERT(entityID < this->getNGlSurfaceLists());
    return this->glSurfaceList[entityID];
}

GLEntityList &GLModelList::getGlSurfaceList(uint entityID)
{
    R_ERROR_ASSERT(entityID < this->getNGlSurfaceLists());
    return this->glSurfaceList[entityID];
}

void GLModelList::setGlSurfaceList(uint entityID, const GLEntityList &glEntityList)
{
    R_ERROR_ASSERT(entityID < this->getNGlSurfaceLists());
    this->glSurfaceList[entityID] = glEntityList;
}

uint GLModelList::getNGlVolumeLists(void) const
{
    return (uint)this->glVolumeList.size();
}

void GLModelList::setNGlVolumeLists(uint nGlEntityLists)
{
    if (this->glVolumeList.size() != int(nGlEntityLists))
    {
        this->glVolumeList.clear();
        this->glVolumeList.resize(nGlEntityLists);
    }
}

const GLEntityList &GLModelList::getGlVolumeList(uint entityID) const
{
    R_ERROR_ASSERT(entityID < this->getNGlVolumeLists());
    return this->glVolumeList[entityID];
}

GLEntityList &GLModelList::getGlVolumeList(uint entityID)
{
    R_ERROR_ASSERT(entityID < this->getNGlVolumeLists());
    return this->glVolumeList[entityID];
}

void GLModelList::setGlVolumeList(uint entityID, const GLEntityList &glEntityList)
{
    R_ERROR_ASSERT(entityID < this->getNGlVolumeLists());
    this->glVolumeList[entityID] = glEntityList;
}

uint GLModelList::getNGlVectorFieldLists(void) const
{
    return this->glVectorFieldList.size();
}

void GLModelList::setNGlVectorFieldLists(uint nGlEntityLists)
{
    if (this->glVectorFieldList.size() != int(nGlEntityLists))
    {
        this->glVectorFieldList.clear();
        this->glVectorFieldList.resize(nGlEntityLists);
    }
}

const GLEntityList &GLModelList::getGlVectorFieldList(uint entityID) const
{
    R_ERROR_ASSERT(entityID < this->getNGlVectorFieldLists());
    return this->glVectorFieldList[entityID];
}

GLEntityList &GLModelList::getGlVectorFieldList(uint entityID)
{
    R_ERROR_ASSERT(entityID < this->getNGlVectorFieldLists());
    return this->glVectorFieldList[entityID];
}

void GLModelList::setGlVectorFieldList(uint entityID, const GLEntityList &glEntityList)
{
    R_ERROR_ASSERT(entityID < this->getNGlVectorFieldLists());
    this->glVectorFieldList[entityID] = glEntityList;
}

uint GLModelList::getNGlScalarFieldLists(void) const
{
    return this->glScalarFieldList.size();
}

void GLModelList::setNGlScalarFieldLists(uint nGlEntityLists)
{
    if (this->glScalarFieldList.size() != int(nGlEntityLists))
    {
        this->glScalarFieldList.clear();
        this->glScalarFieldList.resize(nGlEntityLists);
    }
}

const GLEntityList &GLModelList::getGlScalarFieldList(uint entityID) const
{
    R_ERROR_ASSERT(entityID < this->getNGlScalarFieldLists());
    return this->glScalarFieldList[entityID];
}

GLEntityList &GLModelList::getGlScalarFieldList(uint entityID)
{
    R_ERROR_ASSERT(entityID < this->getNGlScalarFieldLists());
    return this->glScalarFieldList[entityID];
}

void GLModelList::setGlScalarFieldList(uint entityID, const GLEntityList &glEntityList)
{
    R_ERROR_ASSERT(entityID < this->getNGlScalarFieldLists());
    this->glScalarFieldList[entityID] = glEntityList;
}

uint GLModelList::getNGlStreamLineLists(void) const
{
    return this->glStreamLineList.size();
}

void GLModelList::setNGlStreamLineLists(uint nGlEntityLists)
{
    if (this->glStreamLineList.size() != int(nGlEntityLists))
    {
        this->glStreamLineList.clear();
        this->glStreamLineList.resize(nGlEntityLists);
    }
}

const GLEntityList &GLModelList::getGlStreamLineList(uint entityID) const
{
    R_ERROR_ASSERT(entityID < this->getNGlStreamLineLists());
    return this->glStreamLineList[entityID];
}

GLEntityList &GLModelList::getGlStreamLineList(uint entityID)
{
    R_ERROR_ASSERT(entityID < this->getNGlStreamLineLists());
    return this->glStreamLineList[entityID];
}

void GLModelList::setGlStreamLineList(uint entityID, const GLEntityList &glEntityList)
{
    R_ERROR_ASSERT(entityID < this->getNGlStreamLineLists());
    this->glStreamLineList[entityID] = glEntityList;
}

uint GLModelList::getNGlCutLists(void) const
{
    return this->glCutList.size();
}

void GLModelList::setNGlCutLists(uint nGlEntityLists)
{
    if (this->glCutList.size() != int(nGlEntityLists))
    {
        this->glCutList.clear();
        this->glCutList.resize(nGlEntityLists);
    }
}

const GLEntityList &GLModelList::getGlCutList(uint entityID) const
{
    R_ERROR_ASSERT(entityID < this->getNGlCutLists());
    return this->glCutList[entityID];
}

GLEntityList &GLModelList::getGlCutList(uint entityID)
{
    R_ERROR_ASSERT(entityID < this->getNGlCutLists());
    return this->glCutList[entityID];
}

void GLModelList::setGlCutList(uint entityID, const GLEntityList &glEntityList)
{
    R_ERROR_ASSERT(entityID < this->getNGlCutLists());
    this->glCutList[entityID] = glEntityList;
}

uint GLModelList::getNGlIsoLists(void) const
{
    return this->glIsoList.size();
}

void GLModelList::setNGlIsoLists(uint nGlEntityLists)
{
    if (this->glIsoList.size() != int(nGlEntityLists))
    {
        this->glIsoList.clear();
        this->glIsoList.resize(nGlEntityLists);
    }
}

const GLEntityList &GLModelList::getGlIsoList(uint entityID) const
{
    R_ERROR_ASSERT(entityID < this->getNGlIsoLists());
    return this->glIsoList[entityID];
}

GLEntityList &GLModelList::getGlIsoList(uint entityID)
{
    R_ERROR_ASSERT(entityID < this->getNGlIsoLists());
    return this->glIsoList[entityID];
}

void GLModelList::setGlIsoList(uint entityID, const GLEntityList &glEntityList)
{
    R_ERROR_ASSERT(entityID < this->getNGlIsoLists());
    this->glIsoList[entityID] = glEntityList;
}
