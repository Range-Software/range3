/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_plane.cpp                                            *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   20-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Plane class definition                              *
 *********************************************************************/

#include <cmath>

#include "rbl_utils.h"
#include "rbl_plane.h"

void RPlane::_init(const RPlane *pPlane)
{
    if (pPlane)
    {
        this->position = pPlane->position;
        this->normal = pPlane->normal;
    }
}

RPlane::RPlane()
    : position(0.0,0.0,0.0)
    , normal(1.0,0.0,0.0)
{
    this->_init();
}

RPlane::RPlane(const RR3Vector &position, const RR3Vector &normal)
    : position(position)
    , normal(normal)
{
    this->_init();
}

RPlane::RPlane(const RR3Vector &node1, const RR3Vector &node2, const RR3Vector &node3)
{
    RR3Vector v1(node2[0]-node1[0],node2[1]-node1[1],node2[2]-node1[2]);
    RR3Vector v2(node3[0]-node1[0],node3[1]-node1[1],node3[2]-node1[2]);
    RR3Vector::cross(v1,v2,this->normal);
    this->position = node1;
}

RPlane::RPlane(const RPlane &rPlane)
{
    this->_init(&rPlane);
}

RPlane::~RPlane()
{
}

RPlane &RPlane::operator =(const RPlane &rPlane)
{
    this->_init(&rPlane);
    return (*this);
}

const RR3Vector &RPlane::getPosition(void) const
{
    return this->position;
}

void RPlane::setPosition(const RR3Vector &position)
{
    this->position = position;
}

const RR3Vector &RPlane::getNormal(void) const
{
    return this->normal;
}

void RPlane::setNormal(const RR3Vector &normal)
{
    this->normal = normal;
}

void RPlane::findRotationMatrix(RRMatrix &R) const
{
    this->normal.findRotationMatrix(R);
}

void RPlane::findPoints(RR3Vector &point1, RR3Vector &point2, RR3Vector &point3) const
{
    point1 = this->position;

    RR3Vector g(0.0,0.0,1.0);

    if (std::abs(RR3Vector::angle(this->normal,g)) < RConstants::pi/6.0)
    {
        g = RR3Vector(0.0,1.0,0.0);
    }
    if (std::abs(RR3Vector::angle(this->normal,g)) < RConstants::pi/6.0)
    {
        g = RR3Vector(1.0,0.0,0.0);
    }

    RR3Vector u;
    RR3Vector::cross(this->normal,g,u);
    u.normalize();

    RR3Vector v;
    RR3Vector::cross(u,this->normal,v);
    v.normalize();

    point3[0] = point1[0] + u[0];
    point3[1] = point1[1] + u[1];
    point3[2] = point1[2] + u[2];

    point2[0] = point1[0] + v[0];
    point2[1] = point1[1] + v[1];
    point2[2] = point1[2] + v[2];
}

bool RPlane::findLineIntersection(const RR3Vector &position, const RR3Vector &direction, RR3Vector &x, double &u) const
{
    double n = (this->position[0] - position[0]) * this->normal[0]
             + (this->position[1] - position[1]) * this->normal[1]
             + (this->position[2] - position[2]) * this->normal[2];
    double d = direction[0] * this->normal[0]
             + direction[1] * this->normal[1]
             + direction[2] * this->normal[2];

    if (std::abs(d) < RConstants::eps)
    {
        return false;
    }

    u = n / d;

    x[0] = u * direction[0] + position[0];
    x[1] = u * direction[1] + position[1];
    x[2] = u * direction[2] + position[2];

    return true;
}

bool RPlane::findSegmentIntersection(const RR3Vector &node1, const RR3Vector &node2, RR3Vector &x) const
{
    RR3Vector direction(node2[0]-node1[0],node2[1]-node1[1],node2[2]-node1[2]);
    double u;
    if (this->findLineIntersection(node1,direction,x,u))
    {
        if (u >= 0.0 && u <= 1.0)
        {
            return true;
        }
    }
    return false;
}

double RPlane::findPointDistance(const RR3Vector &point) const
{
    double d = 0.0;

    d = this->normal[0] * (this->position[0] - point[0])
      + this->normal[1] * (this->position[1] - point[1])
      + this->normal[2] * (this->position[2] - point[2]);

    return d;
}
