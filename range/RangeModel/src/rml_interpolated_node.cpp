/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_interpolated_node.cpp                                *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   31-st May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Interpolated node class definition                  *
 *********************************************************************/

#include "rml_interpolated_node.h"

void RInterpolatedNode::_init(const RInterpolatedNode *pInterpolatedNode)
{
    if (pInterpolatedNode)
    {
        this->elementID = pInterpolatedNode->elementID;
    }
}

RInterpolatedNode::RInterpolatedNode(unsigned int elementID)
    : elementID(elementID)
{
    this->_init();
}

RInterpolatedNode::RInterpolatedNode(unsigned int elementID, double x, double y, double z)
    : RNode(x,y,z)
    , elementID(elementID)
{
    this->_init();
}

RInterpolatedNode::RInterpolatedNode(unsigned int elementID, const RR3Vector &vector)
    : RNode(vector)
    , elementID(elementID)
{
    this->_init();
}

RInterpolatedNode::RInterpolatedNode(const RInterpolatedNode &interpolatedNode)
    : RNode(interpolatedNode)
{
    this->_init(&interpolatedNode);
}

RInterpolatedNode::~RInterpolatedNode()
{
}

RInterpolatedNode &RInterpolatedNode::operator =(const RInterpolatedNode &interpolatedNode)
{
    this->RNode::operator =(interpolatedNode);
    this->_init(&interpolatedNode);
    return (*this);
}

unsigned int RInterpolatedNode::getElementID(void) const
{
    return this->elementID;
}

void RInterpolatedNode::setElementID(unsigned int elementID)
{
    this->elementID = elementID;
}
