/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_line.cpp                                             *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   8-th December 2011                                       *
 *                                                                   *
 *  DESCRIPTION: Line class definition                               *
 *********************************************************************/

#include "rml_element.h"
#include "rml_line.h"

const QString RLine::defaultName("Line");

RLine::RLine () : crossArea(0.0)
{
    this->name = RLine::defaultName;
    this->_init ();
} /* RLine::RLine */


RLine::RLine (const RLine &line) : RElementGroup (line)
{
    this->_init (&line);
} /* RLine::RLine */


RLine::~RLine ()
{
} /* RLine::~RLine */


void RLine::_init (const RLine *pLine)
{
    if (pLine)
    {
        this->setCrossArea (pLine->getCrossArea());
    }
} /* RLine::_init */


double RLine::getCrossArea (void) const
{
    return this->crossArea;
} /* RLine::getCrossArea */


void RLine::setCrossArea (double crossArea)
{
    this->crossArea = crossArea;
} /* RLine::setCrossArea */


double RLine::findLength(const std::vector<RNode> &nodes, const std::vector<RElement> &elements) const
{
    double lineLength = 0.0;

    for (uint i=0;i<this->size();i++)
    {
        const RElement &rElement = elements[this->get(i)];

        double elementLength = 0.0;
        if (rElement.findLength(nodes,elementLength))
        {
            lineLength += elementLength;
        }
    }

    return lineLength;
} /* RLine::findLength */


RLine & RLine::operator = (const RLine &line)
{
    this->RElementGroup::operator = (line);
    this->_init (&line);
    return (*this);
} /* RLine::operator = */

