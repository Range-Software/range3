/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_triangle.cpp                                         *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   13-th September 2012                                     *
 *                                                                   *
 *  DESCRIPTION: Triangle class definition                           *
 *********************************************************************/

#include <cmath>

#include "rml_triangle.h"


void RTriangle::_init(const RTriangle *pTriangle)
{
    if (pTriangle)
    {
        this->node1ID = pTriangle->node1ID;
        this->node2ID = pTriangle->node2ID;
        this->node3ID = pTriangle->node3ID;
        this->node1 = pTriangle->node1;
        this->node2 = pTriangle->node2;
        this->node3 = pTriangle->node3;
        this->normal = pTriangle->normal;
    }
}

RTriangle::RTriangle(const RNode &node1, const RNode &node2, const RNode &node3)
    : node1ID(RConstants::eod)
    , node2ID(RConstants::eod)
    , node3ID(RConstants::eod)
    , node1(node1)
    , node2(node2)
    , node3(node3)
{
    this->_init();
    this->computeNormal();
}

RTriangle::RTriangle(const RR3Vector &node1, const RR3Vector &node2, const RR3Vector &node3) :
    node1(RNode(node1)),
    node2(RNode(node2)),
    node3(RNode(node3))
{
    this->_init();
    this->computeNormal();
}

RTriangle::RTriangle(const RTriangle &triangle)
{
    this->_init(&triangle);
}

RTriangle::~RTriangle()
{
}

RTriangle &RTriangle::operator =(const RTriangle &triangle)
{
    this->_init(&triangle);
    return (*this);
}

void RTriangle::setNodeIDs(uint node1ID, uint node2ID, uint node3ID)
{
    this->node1ID = node1ID;
    this->node2ID = node2ID;
    this->node3ID = node3ID;
}

bool RTriangle::hasNodeID(uint nodeID) const
{
    if (nodeID == RConstants::eod)
    {
        return false;
    }
    return (this->node1ID == nodeID || this->node2ID == nodeID || this->node3ID == nodeID);
}

void RTriangle::setNodes(const RNode &node1, const RNode &node2, const RNode &node3)
{
    this->node1 = node1;
    this->node2 = node2;
    this->node3 = node3;
    this->computeNormal();
}

void RTriangle::findRotationMatrix(RRMatrix &R) const
{
    R.setIdentity(3);

    RR3Vector lx;
    RR3Vector ly;
    RR3Vector lz;

    lx[0] = this->node2.getX() - this->node1.getX();
    lx[1] = this->node2.getY() - this->node1.getY();
    lx[2] = this->node2.getZ() - this->node1.getZ();

    ly[0] = this->node3.getX() - this->node1.getX();
    ly[1] = this->node3.getY() - this->node1.getY();
    ly[2] = this->node3.getZ() - this->node1.getZ();

    RR3Vector::cross(lx,ly,lz);

    lx.normalize();
    lz.normalize();

    RR3Vector::cross(lz,lx,ly);

    ly.normalize();

    for (unsigned int i=0;i<3;i++)
    {
        R[i][0] = lx[i];
        R[i][1] = ly[i];
        R[i][2] = lz[i];
    }
}

bool RTriangle::findLineIntersection(const RR3Vector &position, const RR3Vector &direction, RR3Vector &x, double &u) const
{
    RPlane plane(this->node1.toVector(),this->node2.toVector(),this->node3.toVector());
    if (!plane.findLineIntersection(position,direction,x,u))
    {
        return false;
    }

    return this->isPointInside(x);
}

bool RTriangle::findSegmentIntersection(const RSegment &segment, RR3Vector &x, double &u) const
{
    if (this->hasNodeID(segment.getNode1ID()))
    {
        u = 1.0;
        return this->findPointIntersection(segment.getNode2(),x);
    }
    if (this->hasNodeID(segment.getNode2ID()))
    {
        u = 0.0;
        return this->findPointIntersection(segment.getNode1(),x);
    }

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

bool RTriangle::findPointIntersection(const RNode &node, RR3Vector &x) const
{
    bool intersectionFound = false;
    x = node.toVector();

    if (this->isPointInside(x))
    {
        intersectionFound = (RR3Vector::findDistance(this->getNode1().toVector(),x) > RConstants::eps &&
                             RR3Vector::findDistance(this->getNode2().toVector(),x) > RConstants::eps &&
                             RR3Vector::findDistance(this->getNode3().toVector(),x) > RConstants::eps);
    }

    return intersectionFound;
}

bool RTriangle::findPointIntersection(const RNode &node, std::set<RR3Vector> &x) const
{
    bool intersectionFound = false;
    RR3Vector point(node.toVector());

    if (this->isPointInside(point))
    {
        intersectionFound = (RR3Vector::findDistance(this->getNode1().toVector(),point) > RConstants::eps &&
                             RR3Vector::findDistance(this->getNode2().toVector(),point) > RConstants::eps &&
                             RR3Vector::findDistance(this->getNode3().toVector(),point) > RConstants::eps);
    }

    if (intersectionFound)
    {
        x.insert(point);
    }

    return intersectionFound;
}

bool RTriangle::findSegmentIntersection(const RSegment &segment, std::set<RR3Vector> &x) const
{
    RPlane plane(this->getNode1().toVector(),this->getNode2().toVector(),this->getNode3().toVector());
    double d1 = plane.findPointDistance(segment.getNode1().toVector());
    double d2 = plane.findPointDistance(segment.getNode2().toVector());

    std::set<RR3Vector> xTemp;

    bool intersectionFound = false;
    if (std::fabs(d1-d2) < RConstants::eps)
    {
        // Segment is parallel to triangle.
        intersectionFound = this->findParallelSegmentIntersection(segment,xTemp);
    }
    else
    {
        // Segment is not parallel to triangle.
        intersectionFound = this->findSkewedSegmentIntersection(segment,xTemp);
    }

    if (intersectionFound)
    {
        intersectionFound = false;

        // Delete intersections at corner nodes.

        std::set<RR3Vector>::reverse_iterator it;
        for (it=xTemp.rbegin();it!=xTemp.rend();++it)
        {
            bool isNode1 = (RR3Vector::findDistance(this->getNode1().toVector(),*it) < RConstants::eps ||
                            RR3Vector::findDistance(this->getNode2().toVector(),*it) < RConstants::eps ||
                            RR3Vector::findDistance(this->getNode3().toVector(),*it) < RConstants::eps);
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
    }

    return intersectionFound;
}

bool RTriangle::findTriangleIntersection(const RTriangle &triangle, std::set<RR3Vector> &x) const
{
    double tolerance = 1.0e-5;

    bool intersectionFound = false;
    std::set<RR3Vector> xTemp;
    RR3Vector nv;
    RR3Vector::cross(this->getNormal(),triangle.getNormal(),nv);
    if (RR3Vector::norm(nv) < tolerance)
    {
        // Triangles are parallel.
        intersectionFound = this->findParallelTriangleIntersection(triangle,xTemp);
    }
    else
    {
        // Triangles are skewed.
        intersectionFound = this->findSkewedTriangleIntersection(triangle,xTemp);
    }

    tolerance = RConstants::eps * 100.0;

    if (intersectionFound)
    {
        intersectionFound = false;

        // Delete intersections at corner nodes.

        std::set<RR3Vector>::reverse_iterator it;
        for (it=xTemp.rbegin();it!=xTemp.rend();++it)
        {
            bool isNode1 = (RR3Vector::findDistance(this->getNode1().toVector(),*it) < tolerance ||
                            RR3Vector::findDistance(this->getNode2().toVector(),*it) < tolerance ||
                            RR3Vector::findDistance(this->getNode3().toVector(),*it) < tolerance);
            bool isNode2 = (RR3Vector::findDistance(triangle.getNode1().toVector(),*it) < tolerance ||
                            RR3Vector::findDistance(triangle.getNode2().toVector(),*it) < tolerance ||
                            RR3Vector::findDistance(triangle.getNode3().toVector(),*it) < tolerance);
            if (isNode1 && isNode2)
            {
                continue;
            }
            if (this->isPointInside(*it,false) || triangle.isPointInside(*it,false))
            {
                // Intersection is not in node.
                x.insert(*it);
                intersectionFound = true;
            }
        }
    }

    return intersectionFound;
}

bool RTriangle::isPointInside(const RR3Vector &point, bool includeNodes) const
{
    if (this->isPointAtNode(point) != RConstants::eod)
    {
        return includeNodes;
    }

    RNode pointNode(point);

    RTriangle t1(this->node1,this->node2,pointNode);
    RTriangle t2(this->node2,this->node3,pointNode);
    RTriangle t3(this->node3,this->node1,pointNode);

    double tArea = this->findArea();
    double pArea = t1.findArea() + t2.findArea() + t3.findArea();

    // EPS (tolerance) is multiplied by factor 100 due to cumulative error when calculating an area
    return !(std::fabs(pArea-tArea) > 100*RConstants::eps);
}

uint RTriangle::isPointAtNode(const RR3Vector &point) const
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
    if (this->getNode3().getDistance(pointNode) < RConstants::eps)
    {
        return 2;
    }

    return RConstants::eod;
}

double RTriangle::findLongestEdgeLength(void) const
{
    double l1 = this->node2.getDistance(this->node3);
    double l2 = this->node3.getDistance(this->node1);
    double l3 = this->node1.getDistance(this->node2);

    if (l1 < l2)
    {
        if (l1 < l3)
        {
            return l1;
        }
        else
        {
            return l3;
        }
    }
    else
    {
        if (l2 < l3)
        {
            return l2;
        }
        else
        {
            return l3;
        }
    }
}

double RTriangle::findArea(void) const
{
    return RTriangle::findArea(this->node1.getX(),this->node1.getY(),this->node1.getZ(),
                               this->node2.getX(),this->node2.getY(),this->node2.getZ(),
                               this->node3.getX(),this->node3.getY(),this->node3.getZ());
}

double RTriangle::findArea(const RNode &node1, const RNode &node2, const RNode &node3)
{
    return RTriangle::findArea(node1.getX(),node1.getY(),node1.getZ(),
                               node2.getX(),node2.getY(),node2.getZ(),
                               node3.getX(),node3.getY(),node3.getZ());
}

double RTriangle::findArea(double xi, double yi, double zi, double xj, double yj, double zj, double xk, double yk, double zk)
{
    double Lij = sqrt((xj-xi)*(xj-xi) + (yj-yi)*(yj-yi) + (zj-zi)*(zj-zi));
    double Ljk = sqrt((xk-xj)*(xk-xj) + (yk-yj)*(yk-yj) + (zk-zj)*(zk-zj));
    double Lki = sqrt((xi-xk)*(xi-xk) + (yi-yk)*(yi-yk) + (zi-zk)*(zi-zk));

    double s = (Lij + Ljk + Lki)/2.0;
    return std::sqrt(std::abs(s*(s-Lij)*(s-Ljk)*(s-Lki)));
}

void RTriangle::findCenter(RR3Vector &center) const
{
    center[0] = (this->getNode1().getX() + this->getNode2().getX() + this->getNode3().getX()) / 3.0;
    center[1] = (this->getNode1().getY() + this->getNode2().getY() + this->getNode3().getY()) / 3.0;
    center[2] = (this->getNode1().getZ() + this->getNode2().getZ() + this->getNode3().getZ()) / 3.0;
}

RR3Vector RTriangle::computeNormal(const RNode &node1, const RNode &node2, const RNode &node3, bool normalize)
{
    return RTriangle::computeNormal(node1.toVector(),node2.toVector(),node3.toVector(),normalize);
}

RR3Vector RTriangle::computeNormal(const RR3Vector &node1, const RR3Vector &node2, const RR3Vector &node3, bool normalize)
{
    RR3Vector v1;
    RR3Vector v2;
    RR3Vector::subtract(node2,node1,v1);
    RR3Vector::subtract(node3,node1,v2);

    RR3Vector n;
    RR3Vector::cross(v1,v2,n);

    if (normalize)
    {
        n.normalize();
    }
    return n;
}

void RTriangle::computeNormal(void)
{
    this->normal = RTriangle::computeNormal(this->node1,this->node2,this->node3);
}

bool RTriangle::findParallelSegmentIntersection(const RSegment &segment, std::set<RR3Vector> &x) const
{
    bool intersectionFound = false;
    uint nNFound = 0;

    // Check if segment's points are inside this triangle.
    if (this->isPointInside(segment.getNode1().toVector()))
    {
        x.insert(segment.getNode1().toVector());
        intersectionFound = true;
        nNFound++;
    }
    if (this->isPointInside(segment.getNode2().toVector()))
    {
        x.insert(segment.getNode2().toVector());
        intersectionFound = true;
        nNFound++;
    }

    if (nNFound == 2)
    {
        return true;
    }

    RSegment s12(this->getNode1(),this->getNode2());
    RSegment s23(this->getNode2(),this->getNode3());
    RSegment s31(this->getNode3(),this->getNode1());

    if (s12.findSegmentIntersection(segment,x))
    {
        intersectionFound = true;
    }
    if (s23.findSegmentIntersection(segment,x))
    {
        intersectionFound = true;
    }
    if (s31.findSegmentIntersection(segment,x))
    {
        intersectionFound = true;
    }

    return intersectionFound;
}

bool RTriangle::findSkewedSegmentIntersection(const RSegment &segment, std::set<RR3Vector> &x) const
{
    RR3Vector xVec;
    double u = 0.0;
    if (this->findSegmentIntersection(segment,xVec,u))
    {
        if (R_IS_IN_CLOSED_INTERVAL(0.0,1.0,u))
        {
            x.insert(xVec);
            return true;
        }
    }
    return false;
}

bool RTriangle::findParallelTriangleIntersection(const RTriangle &triangle, std::set<RR3Vector> &x) const
{
    bool intersectionFound = false;
    uint nNFound = 0;

    bool hasNode11 = this->hasNodeID(triangle.getNode1ID());
    bool hasNode12 = this->hasNodeID(triangle.getNode2ID());
    bool hasNode13 = this->hasNodeID(triangle.getNode3ID());

    // Check if triangle's points are inside this triangle.
    if (hasNode11 || this->isPointInside(triangle.getNode1().toVector()))
    {
        if (!hasNode11)
        {
            x.insert(triangle.getNode1().toVector());
            intersectionFound = true;
        }
        nNFound++;
    }
    if (hasNode12 || this->isPointInside(triangle.getNode2().toVector()))
    {
        if (!hasNode12)
        {
            x.insert(triangle.getNode2().toVector());
            intersectionFound = true;
        }
        nNFound++;
    }
    if (hasNode13 || this->isPointInside(triangle.getNode3().toVector()))
    {
        if (!hasNode13)
        {
            x.insert(triangle.getNode3().toVector());
            intersectionFound = true;
        }
        nNFound++;
    }
    if (nNFound == 3)
    {
        return intersectionFound;
    }

    bool hasNode21 = triangle.hasNodeID(this->getNode1ID());
    bool hasNode22 = triangle.hasNodeID(this->getNode2ID());
    bool hasNode23 = triangle.hasNodeID(this->getNode3ID());

    // Check if this triangle's points are inside triangle.
    nNFound = 0;
    if (hasNode21 || triangle.isPointInside(this->getNode1().toVector()))
    {
        if (!hasNode21)
        {
            x.insert(this->getNode1().toVector());
            intersectionFound = true;
        }
        nNFound++;
    }
    if (hasNode22 || triangle.isPointInside(this->getNode2().toVector()))
    {
        if (!hasNode22)
        {
            x.insert(this->getNode2().toVector());
            intersectionFound = true;
        }
        nNFound++;
    }
    if (hasNode23 || triangle.isPointInside(this->getNode3().toVector()))
    {
        if (!hasNode23)
        {
            x.insert(this->getNode3().toVector());
            intersectionFound = true;
            nNFound++;
        }
    }
    if (nNFound == 3)
    {
        return intersectionFound;
    }

    // Check edge intersections.
    RSegment s11(this->getNode1(),this->getNode2());
    RSegment s12(this->getNode2(),this->getNode3());
    RSegment s13(this->getNode3(),this->getNode1());

    RSegment s21(triangle.getNode1(),triangle.getNode2());
    RSegment s22(triangle.getNode2(),triangle.getNode3());
    RSegment s23(triangle.getNode3(),triangle.getNode1());

    if (!hasNode11 && s11.findSegmentIntersection(s21,x))
    {
        intersectionFound = true;
    }
    if (!hasNode12 && s11.findSegmentIntersection(s22,x))
    {
        intersectionFound = true;
    }
    if (!hasNode13 && s11.findSegmentIntersection(s23,x))
    {
        intersectionFound = true;
    }

    if (!hasNode11 && s12.findSegmentIntersection(s21,x))
    {
        intersectionFound = true;
    }
    if (!hasNode12 && s12.findSegmentIntersection(s22,x))
    {
        intersectionFound = true;
    }
    if (!hasNode13 && s12.findSegmentIntersection(s23,x))
    {
        intersectionFound = true;
    }

    if (!hasNode11 && s13.findSegmentIntersection(s21,x))
    {
        intersectionFound = true;
    }
    if (!hasNode12 && s13.findSegmentIntersection(s22,x))
    {
        intersectionFound = true;
    }
    if (!hasNode13 && s13.findSegmentIntersection(s23,x))
    {
        intersectionFound = true;
    }

    return intersectionFound;
}


bool RTriangle::findSkewedTriangleIntersection(const RTriangle &triangle, std::set<RR3Vector> &x) const
{
    bool intersectionFound = false;

    RR3Vector vx;
    double u = 0.0;

    // Check if triangle's edges are intersecting this triangle.
    RSegment s11(triangle.getNode1(),triangle.getNode2());
    s11.setNodeIDs(triangle.getNode1ID(),triangle.getNode2ID());
    RSegment s12(triangle.getNode2(),triangle.getNode3());
    s12.setNodeIDs(triangle.getNode2ID(),triangle.getNode3ID());
    RSegment s13(triangle.getNode3(),triangle.getNode1());
    s13.setNodeIDs(triangle.getNode3ID(),triangle.getNode1ID());

    if (this->findSegmentIntersection(s11,vx,u))
    {
        if (R_IS_IN_CLOSED_INTERVAL(0.0,1.0,u))
        {
            x.insert(vx);
            intersectionFound = true;
        }
    }
    if (this->findSegmentIntersection(s12,vx,u))
    {
        if (R_IS_IN_CLOSED_INTERVAL(0.0,1.0,u))
        {
            x.insert(vx);
            intersectionFound = true;
        }
    }
    if (this->findSegmentIntersection(s13,vx,u))
    {
        if (R_IS_IN_CLOSED_INTERVAL(0.0,1.0,u))
        {
            x.insert(vx);
            intersectionFound = true;
        }
    }

    // Check if this triangle's edges are intersecting triangle.
    RSegment s21(this->getNode1(),this->getNode2());
    s21.setNodeIDs(this->getNode1ID(),this->getNode2ID());
    RSegment s22(this->getNode2(),this->getNode3());
    s22.setNodeIDs(this->getNode2ID(),this->getNode3ID());
    RSegment s23(this->getNode3(),this->getNode1());
    s23.setNodeIDs(this->getNode3ID(),this->getNode1ID());

    if (triangle.findSegmentIntersection(s21,vx,u))
    {
        if (R_IS_IN_CLOSED_INTERVAL(0.0,1.0,u))
        {
            x.insert(vx);
            intersectionFound = true;
        }
    }
    if (triangle.findSegmentIntersection(s22,vx,u))
    {
        if (R_IS_IN_CLOSED_INTERVAL(0.0,1.0,u))
        {
            x.insert(vx);
            intersectionFound = true;
        }
    }
    if (triangle.findSegmentIntersection(s23,vx,u))
    {
        if (R_IS_IN_CLOSED_INTERVAL(0.0,1.0,u))
        {
            x.insert(vx);
            intersectionFound = true;
        }
    }

    return intersectionFound;
}
