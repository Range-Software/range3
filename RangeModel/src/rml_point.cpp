/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_point.cpp                                            *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   8-th December 2011                                       *
 *                                                                   *
 *  DESCRIPTION: Point class definition                              *
 *********************************************************************/

#include "rml_element.h"
#include "rml_point.h"

const QString RPoint::defaultName("Point");

RPoint::RPoint () : volume(0.0)
{
    this->name = RPoint::defaultName;
    this->_init ();
} /* RPoint::RPoint */


RPoint::RPoint (const RPoint &point) : RElementGroup (point)
{
    this->_init (&point);
} /* RPoint::RPoint */


RPoint::~RPoint ()
{
} /* RPoint::~RPoint */


void RPoint::_init (const RPoint *pPoint)
{
    if (pPoint)
    {
        this->setVolume (pPoint->getVolume());
    }
} /* RPoint::_init */


double RPoint::getVolume (void) const
{
    return this->volume;
} /* RPoint::get_volume */


void RPoint::setVolume (double volume)
{
    this->volume = volume;
} /* RPoint::set_volume */


RPoint & RPoint::operator = (const RPoint &point)
{
    this->RElementGroup::operator = (point);
    this->_init (&point);
    return (*this);
} /* RPoint::operator = */

