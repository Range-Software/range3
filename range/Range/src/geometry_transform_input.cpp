/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   geometry_transform_input.cpp                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   7-th November 2014                                       *
 *                                                                   *
 *  DESCRIPTION: Geometry transform input class definition           *
 *********************************************************************/

#include "geometry_transform_input.h"


void GeometryTransformInput::_init(const GeometryTransformInput *pGeometryTransformInput)
{
    if (pGeometryTransformInput)
    {
        this->translation = pGeometryTransformInput->translation;
        this->rotation = pGeometryTransformInput->rotation;
        this->rotationCenter = pGeometryTransformInput->rotationCenter;
        this->scale = pGeometryTransformInput->scale;
        this->scaleCenter = pGeometryTransformInput->scaleCenter;
        this->applyTo = pGeometryTransformInput->applyTo;
        this->includeSharedNodes = pGeometryTransformInput->includeSharedNodes;
        this->splitSharedNodes = pGeometryTransformInput->splitSharedNodes;
        this->sweepSharedNodes = pGeometryTransformInput->sweepSharedNodes;
        this->nSweepSteps = pGeometryTransformInput->nSweepSteps;
    }
}

GeometryTransformInput::GeometryTransformInput()
    : translation(0.0,0.0,0.0)
    , rotation(0.0,0.0,0.0)
    , rotationCenter(0.0,0.0,0.0)
    , scale(1.0,1.0,1.0)
    , scaleCenter(0.0,0.0,0.0)
    , applyTo(GeometryTransformInput::ApplyToAll)
    , includeSharedNodes(true)
    , splitSharedNodes(true)
    , sweepSharedNodes(true)
    , nSweepSteps(1)
{
    this->_init();
}

GeometryTransformInput::GeometryTransformInput(const GeometryTransformInput &geometryTransformInput)
{
    this->_init(&geometryTransformInput);
}

GeometryTransformInput::~GeometryTransformInput()
{

}

GeometryTransformInput &GeometryTransformInput::operator =(const GeometryTransformInput &geometryTransformInput)
{
    this->_init(&geometryTransformInput);
    return (*this);
}

const RR3Vector &GeometryTransformInput::getTranslation(void) const
{
    return this->translation;
}

RR3Vector &GeometryTransformInput::getTranslation(void)
{
    return this->translation;
}

void GeometryTransformInput::setTranslation(const RR3Vector &translation)
{
    this->translation = translation;
}

const RR3Vector &GeometryTransformInput::getRotation(void) const
{
    return this->rotation;
}

RR3Vector &GeometryTransformInput::getRotation(void)
{
    return this->rotation;
}

void GeometryTransformInput::setRotation(const RR3Vector &rotation)
{
    this->rotation = rotation;
}

const RR3Vector &GeometryTransformInput::getRotationCenter(void) const
{
    return this->rotationCenter;
}

RR3Vector &GeometryTransformInput::getRotationCenter(void)
{
    return this->rotationCenter;
}

void GeometryTransformInput::setRotationCenter(const RR3Vector &rotationCenter)
{
    this->rotationCenter = rotationCenter;
}

const RR3Vector &GeometryTransformInput::getScale(void) const
{
    return this->scale;
}

RR3Vector &GeometryTransformInput::getScale(void)
{
    return this->scale;
}

void GeometryTransformInput::setScale(const RR3Vector &scale)
{
    this->scale = scale;
}

const RR3Vector &GeometryTransformInput::getScaleCenter(void) const
{
    return this->scaleCenter;
}

RR3Vector &GeometryTransformInput::getScaleCenter(void)
{
    return this->scaleCenter;
}

void GeometryTransformInput::setScaleCenter(const RR3Vector &scaleCenter)
{
    this->scaleCenter = scaleCenter;
}

GeometryTransformInput::ApplyTo GeometryTransformInput::getApplyTo(void) const
{
    return applyTo;
}

void GeometryTransformInput::setApplyTo(const ApplyTo &applyTo)
{
    this->applyTo = applyTo;
}

bool GeometryTransformInput::getIncludeSharedNodes(void) const
{
    return this->includeSharedNodes;
}

void GeometryTransformInput::setIncludeSharedNodes(bool includeSharedNodes)
{
    this->includeSharedNodes = includeSharedNodes;
}

bool GeometryTransformInput::getSplitSharedNodes(void) const
{
    return this->splitSharedNodes;
}

void GeometryTransformInput::setSplitSharedNodes(bool splitSharedNodes)
{
    this->splitSharedNodes = splitSharedNodes;
}

bool GeometryTransformInput::getSweepSharedNodes(void) const
{
    return this->sweepSharedNodes;
}

void GeometryTransformInput::setSweepSharedNodes(bool sweepSharedNodes)
{
    this->sweepSharedNodes = sweepSharedNodes;
}

uint GeometryTransformInput::getNSweepSteps(void) const
{
    return this->nSweepSteps;
}

void GeometryTransformInput::setNSweepSteps(uint nSweepSteps)
{
    this->nSweepSteps = nSweepSteps;
}

bool GeometryTransformInput::isTranslateActive(void) const
{
    return (this->translation[0] != 0.0 || this->translation[1] != 0.0 || this->translation[2] != 0.0);
}

bool GeometryTransformInput::isRotateActive(void) const
{
    return (this->rotation[0] != 0.0 || this->rotation[1] != 0.0 || this->rotation[2] != 0.0);
}

bool GeometryTransformInput::isScaleActive(void) const
{
    return (this->scale[0] != 1.0 || this->scale[1] != 1.0 || this->scale[2] != 1.0);
}
