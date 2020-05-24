/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_local_diraction.cpp                                  *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   23-rd May 2019                                           *
 *                                                                   *
 *  DESCRIPTION: Local direction class definition                    *
 *********************************************************************/

#include "rbl_local_direction.h"

void RLocalDirection::_init(const RLocalDirection *pLocalDirection)
{
    if (pLocalDirection)
    {
        this->position = pLocalDirection->position;
        this->direction = pLocalDirection->direction;
    }
}

RLocalDirection::RLocalDirection()
    : position(0.0,0.0,0.0)
    , direction(1.0,0.0,0.0)
{

}

RLocalDirection::RLocalDirection(const RR3Vector &position, const RR3Vector &direction)
    : position(position)
    , direction(direction)
{
    this->_init();
}

RLocalDirection::RLocalDirection(const RLocalDirection &rLocalDirection)
{
    this->_init(&rLocalDirection);
}

RLocalDirection &RLocalDirection::operator =(const RLocalDirection &rLocalDirection)
{
    this->_init(&rLocalDirection);
    return (*this);
}

const RR3Vector &RLocalDirection::getPosition() const
{
    return this->position;
}

const RR3Vector &RLocalDirection::getDirection() const
{
    return this->direction;
}
