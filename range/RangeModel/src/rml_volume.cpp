/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_volume.cpp                                           *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   8-th December 2011                                       *
 *                                                                   *
 *  DESCRIPTION: Volume class definition                             *
 *********************************************************************/

#include "rml_element.h"
#include "rml_volume.h"

const QString RVolume::defaultName("Volume");


void RVolume::_init (const RVolume *pVolume)
{
    if (pVolume)
    {
    }
} /* RVolume::_init */


RVolume::RVolume ()
{
    this->name = RVolume::defaultName;
    this->_init ();
} /* RVolume::RVolume */


RVolume::RVolume (const RVolume &volume) : RElementGroup (volume)
{
    this->_init (&volume);
} /* RVolume::RVolume */


RVolume::~RVolume ()
{
} /* RVolume::~RVolume */

double RVolume::findVolume(const std::vector<RNode> &nodes, const std::vector<RElement> &elements) const
{
    double totalVolume = 0.0;

    for (uint i=0;i<this->size();i++)
    {
        const RElement &rElement = elements[this->get(i)];

        double elementVolume = 0.0;
        if (rElement.findVolume(nodes,elementVolume))
        {
            totalVolume += elementVolume;
        }
    }

    return totalVolume;
} /* RVolume::findVolume */


RVolume & RVolume::operator = (const RVolume &volume)
{
    this->RElementGroup::operator = (volume);
    this->_init (&volume);
    return (*this);
} /* RVolume::operator = */

