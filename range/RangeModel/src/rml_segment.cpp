/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_segment.cpp                                          *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   17-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Segment class definition                            *
 *********************************************************************/

#include <cmath>

#include "rml_segment.h"
#include "rml_triangle.h"

void RSegment::_init(const RSegment *pSegment)
{
    if (pSegment)
    {
        this->node1ID = pSegment->node1ID;
        this->node2ID = pSegment->node2ID;
        this->node1 = pSegment->node1;
        this->node2 = pSegment->node2;
    }
}

RSegment::RSegment(const RNode &node1, const RNode &node2)
    : node1ID(RConstants::eod)
    , node2ID(RConstants::eod)
    , node1(node1)
    , node2(node2)
{
    this->_init();
}

RSegment::RSegment(const RSegment &segment)
{
    this->_init(&segment);
}

RSegment::~RSegment()
{
}

RSegment &RSegment::operator =(const RSegment &segment)
{
    this->_init(&segment);
    return (*this);
}

bool RSegment::operator <(const RSegment &segment) const
{
    if (this->node1 < segment.node1 || this->node1 < segment.node2)
    {
        return true;
    }
    if (this->node2 < segment.node1 || this->node2 < segment.node2)
    {
        return true;
    }
    return false;
}

void RSegment::setNodeIDs(uint node1ID, uint node2ID)
{
    this->node1ID = node1ID;
    this->node2ID = node2ID;
}

bool RSegment::hasNodeID(uint nodeID) const
{
    if (nodeID == RConstants::eod)
    {
        return false;
    }
    return (this->node1ID == nodeID || this->node2ID == nodeID);
}

void RSegment::setNodes(const RNode &node1, const RNode &node2)
{
    this->node1 = node1;
    this->node2 = node2;
}

void RSegment::findRotationMatrix(RRMatrix &R) const
{
    RR3Vector vec;

    vec[0] = this->node2.getX() - this->node1.getX();
    vec[1] = this->node2.getY() - this->node1.getY();
    vec[2] = this->node2.getZ() - this->node1.getZ();

    vec.findRotationMatrix(R);
}

double RSegment::findLineDistance(const RR3Vector &position, const RR3Vector &direction) const
{
    RR3Vector segmentDirection(this->node2.getX()-this->node1.getX(),this->node2.getY()-this->node1.getY(),this->node2.getZ()-this->node1.getZ());

    RR3Vector vp;
    RR3Vector::cross(segmentDirection,direction,vp);

    double norm = RR3Vector::norm(vp);

    if (norm < RConstants::eps)
    {
        // Parallel lines
        return this->findPointDistance(position);
    }

    vp *= 1.0 / norm;

    RR3Vector startDistance;
    RR3Vector::subtract(position,this->node1.toVector(),startDistance);
    return std::fabs(RR3Vector::dot(vp,startDistance));
}

double RSegment::findSegmentDistance(const RR3Vector &position, const RR3Vector &direction) const
{
    RR3Vector c1 = this->findLineNearestPoint(position,direction,true);

    RSegment line(RNode(position),RNode(position[0]+direction[0],position[1]+direction[1],position[2]+direction[2]));

    if (this->isPointInside(c1))
    {
        return line.findPointDistance(c1);
    }
    else
    {
        double d1 = line.findPointDistance(this->node1.toVector());
        double d2 = line.findPointDistance(this->node2.toVector());

        return d1 < d2 ? d1 : d2;
    }
}

RR3Vector RSegment::findLineNearestPoint(const RR3Vector &position, const RR3Vector &direction, bool onSegment) const
{
    RR3Vector p1(this->node1.toVector());
    RR3Vector d1(this->node2.getX()-this->node1.getX(),this->node2.getY()-this->node1.getY(),this->node2.getZ()-this->node1.getZ());
    const RR3Vector &p2 = position;
    const RR3Vector &d2 = direction;
    RR3Vector n;

    RR3Vector::cross(d1,d2,n);

    if (RR3Vector::norm(n) < RConstants::eps)
    {
        // Parallel lines
        return position;
    }

    if (onSegment)
    {
        RR3Vector n2;
        RR3Vector::cross(d2,n,n2);

        // c1 = p1 + (((p2-p1).n2)/(d1.n2))d1

        // p21 = p2-p1
        RR3Vector p21;
        RR3Vector::subtract(p2,p1,p21);

        d1 *= RR3Vector::dot(p21,n2) / RR3Vector::dot(d1,n2);

        RR3Vector c1;

        RR3Vector::add(p1,d1,c1);

        return c1;
    }
    else
    {
        RR3Vector n1;
        RR3Vector::cross(d1,n,n1);

        // c2 = p2 + (((p1-p2).n1)/(d2.n1))d2

        // p12 = p1-p2
        RR3Vector p12;
        RR3Vector::subtract(p1,p2,p12);

        RR3Vector sd2(d2);
        sd2 *= RR3Vector::dot(p12,n1) / RR3Vector::dot(d2,n1);

        RR3Vector c2;

        RR3Vector::add(p2,sd2,c2);

        return c2;
    }
}

bool RSegment::findLineIntersection(const RR3Vector &position, const RR3Vector &direction, RR3Vector &x, double &u) const
{
    RR3Vector n1 = this->getNode1().toVector();
    RR3Vector n2 = this->getNode2().toVector();
    const RR3Vector &n3(position);
    RR3Vector n4(position[0]+direction[0],position[1]+direction[1],position[2]+direction[2]);

    double u2;
    if (!RSegment::findLineLineIntersection(n1,n2,n3,n4,x,u,u2))
    {
        return false;
    }

    return R_IS_IN_CLOSED_INTERVAL(0.0,1.0,u);
}

bool RSegment::findPointIntersection(const RNode &node, std::set<RR3Vector> &x) const
{
    bool intersectionFound = false;
    RR3Vector point(node.toVector());

    if (this->isPointInside(point))
    {
        intersectionFound = (RR3Vector::findDistance(this->getNode1().toVector(),point) > RConstants::eps &&
                             RR3Vector::findDistance(this->getNode2().toVector(),point) > RConstants::eps);
    }

    if (intersectionFound)
    {
        x.insert(point);
    }

    return intersectionFound;
}

bool RSegment::findSegmentIntersection(const RSegment &segment, std::set<RR3Vector> &x) const
{
    double tollerance = 1.0e-5;

    RPlane plane(this->getNode1().toVector(),
                 this->getNode2().toVector(),
                 segment.getNode1().toVector());

    double pointPlaneDistance = std::fabs(plane.findPointDistance(segment.getNode2().toVector()));
    if (pointPlaneDistance > tollerance)
    {
        return false;
    }

    std::set<RR3Vector> xTemp;

    if (RR3Vector::areParallel(this->findDirection(),segment.findDirection()))
    {
        if (this->isPointInside(segment.getNode1().toVector()))
        {
            xTemp.insert(segment.getNode1().toVector());
        }
        if (this->isPointInside(segment.getNode2().toVector()))
        {
            xTemp.insert(segment.getNode2().toVector());
        }
        if (segment.isPointInside(this->getNode1().toVector()))
        {
            xTemp.insert(this->getNode1().toVector());
        }
        if (segment.isPointInside(this->getNode2().toVector()))
        {
            xTemp.insert(this->getNode2().toVector());
        }
    }
    else
    {
        RR3Vector xVec;
        double u1 = 0.0;
        double u2 = 0.0;

        if (RSegment::findLineLineIntersection(this->getNode1().toVector(),
                                               this->getNode2().toVector(),
                                               segment.getNode1().toVector(),
                                               segment.getNode2().toVector(),
                                               xVec,u1,u2))
        {
            if (R_IS_IN_CLOSED_INTERVAL(0.0,1.0,u1) && R_IS_IN_CLOSED_INTERVAL(0.0,1.0,u2))
            {
                xTemp.insert(xVec);
            }
        }
    }

    bool intersectionFound = false;

    // Delete intersections at nodes.

    std::set<RR3Vector>::reverse_iterator it;
    for (it=xTemp.rbegin();it!=xTemp.rend();++it)
    {
        bool isNode1 = (RR3Vector::findDistance(this->getNode1().toVector(),*it) < RConstants::eps ||
                        RR3Vector::findDistance(this->getNode2().toVector(),*it) < RConstants::eps);
        bool isNode2 = (RR3Vector::findDistance(segment.getNode1().toVector(),*it) < RConstants::eps ||
                        RR3Vector::findDistance(segment.getNode2().toVector(),*it) < RConstants::eps);
        if (isNode1 && isNode2)
        {
            continue;
        }
        // Intersection is not in node.
        x.insert(*it);
        intersectionFound = true;
    }

    return intersectionFound;
}

bool RSegment::findSegmentIntersection(const RSegment &segment, RR3Vector &x, double &u) const
{
    RR3Vector position;
    RR3Vector direction;

    position[0] = segment.getNode1().getX();
    position[1] = segment.getNode1().getY();
    position[2] = segment.getNode1().getZ();

    direction[0] = segment.getNode2().getX() - segment.getNode1().getX();
    direction[1] = segment.getNode2().getY() - segment.getNode1().getY();
    direction[2] = segment.getNode2().getZ() - segment.getNode1().getZ();

    return this->findLineIntersection(position,direction,x,u);
}

double RSegment::findPointDistance(const RR3Vector &point) const
{
    const RR3Vector &p = point;
    RR3Vector a(this->getNode1().toVector());
    RR3Vector n(this->getNode2().getX()-this->getNode1().getX(),
                this->getNode2().getY()-this->getNode1().getY(),
                this->getNode2().getZ()-this->getNode1().getZ());
    RR3Vector ap;
    RR3Vector::subtract(a,p,ap);

    RR3Vector apn(n);
    apn *= RR3Vector::dot(ap,n);

    RR3Vector d;
    RR3Vector::subtract(ap,apn,d);
    return d.length();
}

bool RSegment::isPointInside(const RR3Vector &point, bool includeNodes) const
{
    if (this->isPointAtNode(point) != RConstants::eod)
    {
        return includeNodes;
    }

    RNode np(point);

    return (std::fabs(np.getDistance(this->node1) + np.getDistance(this->node2) - this->findLength()) < RConstants::eps);
}

uint RSegment::isPointAtNode(const RR3Vector &point) const
{
    RNode pointNode(point);

    if (this->getNode1().getDistance(pointNode) < RConstants::eps)
    {
        return 0;
    }
    if (this->getNode2().getDistance(pointNode) < RConstants::eps)
    {
        return 1;
    }

    return RConstants::eod;
}

RR3Vector RSegment::findDirection() const
{
    RR3Vector direction;
    this->findDirection(direction);
    return direction;
}

void RSegment::findDirection(RR3Vector &direction) const
{
    direction[0] = this->node2.getX() - this->node1.getX();
    direction[1] = this->node2.getY() - this->node1.getY();
    direction[2] = this->node2.getZ() - this->node1.getZ();
}

double RSegment::findLength() const
{
    return this->node1.getDistance(this->node2);
}

double RSegment::findLength(const RNode &node1, const RNode &node2)
{
    return node1.getDistance(node2);
}

void RSegment::findPerpendicularVectors(RR3Vector &d1, RR3Vector &d2, RR3Vector &d3) const
{
    d1 = this->findDirection();
    d1.normalize();

    if (std::abs(d1[0]-1.0) < 0.5)
    {
        d3[0]=0.0; d3[1]=1.0; d3[2]=0.0;
    }
    else
    {
        d3[0]=1.0; d3[1]=0.0; d3[2]=0.0;
    }

    RR3Vector::cross(d1,d3,d2);
    RR3Vector::cross(d1,d2,d3);
}

void RSegment::print() const
{
    RLogger::info("Segment:\n");
    this->node1.print();
    this->node2.print();
}

bool RSegment::findLineLineIntersection(const RR3Vector &n1, const RR3Vector &n2, const RR3Vector &n3, const RR3Vector &n4, RR3Vector &nx, double &u1, double &u2)
{
    // http://mathforum.org/library/drmath/view/62814.html
    double tolerance = 1.0e-5;

    RR3Vector v1;
    RR3Vector v2;
    const RR3Vector &p1 = n1;
    const RR3Vector &p2 = n3;

    RR3Vector::subtract(n2,n1,v1);
    RR3Vector::subtract(n4,n3,v2);

    RR3Vector v1v2;

    RR3Vector::cross(v1,v2,v1v2);

    double dv1v2 = RR3Vector::norm(v1v2);
    if (dv1v2 < tolerance)
    {
        return false;
    }

    RR3Vector p21;
    RR3Vector p21v2;

    RR3Vector::subtract(p2,p1,p21);
    RR3Vector::cross(p21,v2,p21v2);

    RR3Vector vx;
    RR3Vector::cross(v1v2,p21v2,vx);

    if (RR3Vector::norm(vx) > tolerance)
    {
        return false;
    }

    u1 = RR3Vector::norm(p21v2) / dv1v2;

    if (RR3Vector::dot(v1v2,p21v2) < 0.0)
    {
        u1 *= -1.0;
    }

    nx[0] = p1[0] + u1*v1[0];
    nx[1] = p1[1] + u1*v1[1];
    nx[2] = p1[2] + u1*v1[2];

    RR3Vector::subtract(nx,p2,vx);
    u2 = RR3Vector::norm(vx) / RR3Vector::norm(v2);

    if (RR3Vector::dot(vx,v2) < 0.0)
    {
        u2 *= -1.0;
    }
    return true;
}
