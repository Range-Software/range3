/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_interpolated_entity.cpp                              *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   31-st May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Interpolated entity class definition                *
 *********************************************************************/

#include "rml_interpolated_entity.h"

void RInterpolatedEntity::_init(const RInterpolatedEntity *pInterpolatedEntity)
{
    if (pInterpolatedEntity)
    {

    }
}

RInterpolatedEntity::RInterpolatedEntity()
{
    this->_init();
}

RInterpolatedEntity::RInterpolatedEntity(const RInterpolatedEntity &interpolatedEntity)
    : REntityGroup(interpolatedEntity)
    , std::vector<RInterpolatedElement> (interpolatedEntity)
{
    this->_init(&interpolatedEntity);
}

RInterpolatedEntity::~RInterpolatedEntity()
{
}

RInterpolatedEntity &RInterpolatedEntity::operator =(const RInterpolatedEntity &interpolatedEntity)
{
    this->REntityGroup::operator = (interpolatedEntity);
    this->std::vector<RInterpolatedElement>::operator = (interpolatedEntity);
    this->_init(&interpolatedEntity);
    return (*this);
}
