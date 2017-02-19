/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rhemicubepixel.cpp                                       *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   24-th July 2014                                          *
 *                                                                   *
 *  DESCRIPTION: View-factor hemi cube pixel class definition        *
 *********************************************************************/

#include <cfloat>

#include "rhemicubepixel.h"


void RHemiCubePixel::_init(const RHemiCubePixel *pHemiCubePixel)
{
    if (pHemiCubePixel)
    {
        this->position = pHemiCubePixel->position;
        this->color = pHemiCubePixel->color;
        this->depth = pHemiCubePixel->depth;
        this->weight = pHemiCubePixel->weight;
    }
}

RHemiCubePixel::RHemiCubePixel()
    : color(RConstants::eod)
    , depth(DBL_MAX)
    , weight(1.0)
{
    this->_init();
}

RHemiCubePixel::RHemiCubePixel(const RHemiCubePixel &hemiCubePixel)
{
    this->_init(&hemiCubePixel);
}

RHemiCubePixel::~RHemiCubePixel()
{
}

RHemiCubePixel &RHemiCubePixel::operator =(const RHemiCubePixel &hemiCubePixel)
{
    this->_init(&hemiCubePixel);
    return (*this);
}

const RR3Vector &RHemiCubePixel::getPosition(void) const
{
    return this->position;
}

RR3Vector &RHemiCubePixel::getPosition(void)
{
    return this->position;
}

void RHemiCubePixel::setPosition(const RR3Vector &position)
{
    this->position = position;
}

uint RHemiCubePixel::getColor(void) const
{
    return this->color;
}

void RHemiCubePixel::setColor(uint color)
{
    this->color = color;
}

double RHemiCubePixel::getDepth(void) const
{
    return this->depth;
}

void RHemiCubePixel::setDepth(double depth)
{
    this->depth = depth;
}

double RHemiCubePixel::getWeight(void) const
{
    return this->weight;
}

void RHemiCubePixel::setWeight(double weight)
{
    this->weight = weight;
}
