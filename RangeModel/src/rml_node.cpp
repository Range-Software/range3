/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_node.source                                          *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   8-th December 2011                                       *
 *                                                                   *
 *  DESCRIPTION: Node class definition                               *
 *********************************************************************/

#include <cmath>
#include <rblib.h>

#include "rml_node.h"


RNode::RNode ()
    : x(0.0)
    , y(0.0)
    , z(0.0)
{
    this->_init ();
} /* RNode::RNode */


RNode::RNode (double x,
              double y,
              double z)
    : x(x)
    , y(y)
    , z(z)
{
    this->_init ();
}

RNode::RNode (const RR3Vector &vector)
    : x(vector[0])
    , y(vector[1])
    , z(vector[2])
{
    this->_init ();
} /* RNode::RNode */


RNode::RNode (const RNode &node)
{
    this->_init (&node);
} /* RNode::RNode - copy */


RNode::~RNode ()
{
} /* RNode::~RNode */


void RNode::_init (const RNode *pNode)
{
    if (pNode)
    {
        this->x = pNode->x;
        this->y = pNode->y;
        this->z = pNode->z;
    }
} /* RNode::_init */


double RNode::getDistance (const RNode &node) const
{
    double x, y, z;
    double dx, dy, dz;
    double dl;

    x = this->getX();
    y = this->getY();
    z = this->getZ();

    dx = x - node.getX();
    dy = y - node.getY();
    dz = z - node.getZ();

    dl = sqrt (dx*dx + dy*dy + dz*dz);

    return dl;
} /* RNode::getDistance */


void RNode::transform(const RRMatrix &R, const RR3Vector &t, bool translateFirst)
{
    if (translateFirst)
    {
        this->translate(t);
    }
    double xt = R[0][0] * this->getX() + R[0][1] * this->getY() + R[0][2] * this->getZ();
    double yt = R[1][0] * this->getX() + R[1][1] * this->getY() + R[1][2] * this->getZ();
    double zt = R[2][0] * this->getX() + R[2][1] * this->getY() + R[2][2] * this->getZ();

    this->set(xt,yt,zt);
    if (!translateFirst)
    {
        this->translate(t);
    }
} /* RNode::transform */


void RNode::translate(const RR3Vector &t)
{
    this->set(this->getX()+t[0],this->getY()+t[1],this->getZ()+t[2]);
} /* RNode::translate */


void RNode::scale(double scale)
{
    this->set(this->getX()*scale,this->getY()*scale,this->getZ()*scale);
} /* RNode::scale */


void RNode::scale(const RR3Vector &scaleVector)
{
    this->set(this->getX()*scaleVector[0],this->getY()*scaleVector[1],this->getZ()*scaleVector[2]);
} /* RNode::scale */


RNode & RNode::operator = (const RNode &node)
{
    this->_init (&node);
    return (*this);
} /* RNode::operator = */


bool RNode::operator == (const RNode &node) const
{
    if (this->getX() == node.getX() &&
        this->getY() == node.getY() &&
        this->getZ() == node.getZ())
    {
        return true;
    }
    else
    {
        return false;
    }
} /*RNode::operator == */


bool RNode::operator != (const RNode &node) const
{
    return !((*this) == node);
} /*RNode::operator == */


bool RNode::operator <(const RNode &node) const
{
    if (this->x < node.x)
    {
        return true;
    }
    if (this->y < node.y)
    {
        return true;
    }
    if (this->z < node.z)
    {
        return true;
    }
    return false;
} /*RNode::operator < */


RR3Vector RNode::toVector(void) const
{
    RR3Vector vector;
    vector.at(0) = this->getX();
    vector.at(1) = this->getY();
    vector.at(2) = this->getZ();
    return vector;
} /*RNode::toVector */


void RNode::toVector (RR3Vector &vector) const
{
    vector.at(0) = this->getX();
    vector.at(1) = this->getY();
    vector.at(2) = this->getZ();
} /*RNode::toVector */


void RNode::print(void) const
{
    this->toVector().print(true,true);
} /*RNode::print */
