/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_polygon.cpp                                          *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   24-th October 2014                                       *
 *                                                                   *
 *  DESCRIPTION: Polygon class definition                            *
 *********************************************************************/

#include "rml_polygon.h"
#include "rml_triangle.h"
#include "rml_element.h"

void RPolygon::_init(const RPolygon *pPolygon)
{
    if (pPolygon)
    {
        this->nodes = pPolygon->nodes;
        this->normal = pPolygon->normal;
    }
}

RPolygon::RPolygon(const std::vector<RNode> &nodes)
    : nodes(nodes)
{
    this->_init();
    RPolygon::sortNodes(this->nodes);
    this->computeNormal();
}

RPolygon::RPolygon(const std::vector<RR3Vector> &nodes)
{
    this->nodes.resize(nodes.size());
    for (uint i=0;i<nodes.size();i++)
    {
        this->nodes[i].setX(nodes[i][0]);
        this->nodes[i].setY(nodes[i][1]);
        this->nodes[i].setZ(nodes[i][2]);
    }

    this->_init();
    RPolygon::sortNodes(this->nodes);
    this->computeNormal();
}

RPolygon::RPolygon(const RPolygon &polygon)
{
    this->_init(&polygon);
}

RPolygon::~RPolygon()
{

}

RPolygon &RPolygon::operator =(const RPolygon &polygon)
{
    this->_init(&polygon);
    return (*this);
}

void RPolygon::setNodes(const std::vector<RNode> &nodes)
{
    this->nodes = nodes;
    RPolygon::sortNodes(this->nodes);
    this->computeNormal();
}

void RPolygon::removeNode(uint nodePosition)
{
    R_ERROR_ASSERT (nodePosition < this->nodes.size());
    this->nodes.erase(this->nodes.begin()+nodePosition);
    this->computeNormal();
}

double RPolygon::findArea(void) const
{
    double area = 0.0;

    return area;
}

uint RPolygon::getNNodes(void) const
{
    return this->nodes.size();
}

struct NodeAngleItem
{
    uint id;
    RNode node;
    double angle;
};

bool nodeAngleItemCompare(const NodeAngleItem &a, const NodeAngleItem &b)
{
    return a.angle < b.angle;
}

std::vector<uint> RPolygon::sortNodes(std::vector<RNode> &nodes)
{
    std::vector<uint> nodeIDs;
    nodeIDs.resize(nodes.size());
    for (uint i=0;i<nodeIDs.size();i++)
    {
        nodeIDs[i] = i;
    }

    if (nodes.size() < 4)
    {
        return nodeIDs;
    }

    std::vector<NodeAngleItem> nodeAngleList;

    RR3Vector v1(nodes[1].getX()-nodes[0].getX(),nodes[1].getY()-nodes[0].getY(),nodes[1].getZ()-nodes[0].getZ());

    RTriangle t(nodes[0],nodes[1],nodes[2]);
    RR3Vector firstNormal(t.getNormal());

    nodeAngleList.push_back(NodeAngleItem());
    nodeAngleList.back().id = nodeIDs[1];
    nodeAngleList.back().node = nodes[1];
    nodeAngleList.back().angle = 0.0;
    for (uint i=2;i<nodes.size();i++)
    {
        nodeAngleList.push_back(NodeAngleItem());
        nodeAngleList.back().id = nodeIDs[i];
        nodeAngleList.back().node = nodes[i];
        RR3Vector v2(nodes[i].getX()-nodes[0].getX(),nodes[i].getY()-nodes[0].getY(),nodes[i].getZ()-nodes[0].getZ());
        nodeAngleList.back().angle = RRVector::angle(v1,v2);

        if (i > 2)
        {
            RTriangle t(nodes[0],nodes[1],nodes[i]);
            if (RRVector::angle(firstNormal,t.getNormal()) > RConstants::pi/2.0)
            {
                nodeAngleList.back().angle *= -1.0;
            }
        }
    }

    std::sort(nodeAngleList.begin(), nodeAngleList.end(), nodeAngleItemCompare);

    for (uint i=0;i<nodeAngleList.size();i++)
    {
        nodeIDs[i+1] = nodeAngleList[i].id;
        nodes[i+1] = nodeAngleList[i].node;
    }

    return nodeIDs;
}

std::vector<RElement> RPolygon::triangulate(const std::vector<RNode> &nodes, bool nodesSorted)
{
    if (nodes.size() < 3)
    {
        return std::vector<RElement>();
    }
    else if (nodes.size() == 3)
    {
        RElement element;
        element.setType(R_ELEMENT_TRI1);
        element.setNodeId(0,0);
        element.setNodeId(1,1);
        element.setNodeId(2,2);
        std::vector<RElement> elements;
        elements.push_back(element);
        return elements;
    }

    std::vector<RNode> sortedNodes(nodes);
    std::vector<uint> nodeIDs;

    nodeIDs.resize(nodes.size());
    for (uint i=0;i<nodeIDs.size();i++)
    {
        nodeIDs[i] = i;
    }

    if (!nodesSorted)
    {
        nodeIDs = RPolygon::sortNodes(sortedNodes);
    }

    RPolygon p(sortedNodes);
    RUVector book(p.getNNodes());

    for (uint i=0;i<book.size();i++)
    {
        book[i] = i;
    }

    std::vector<RElement> elements;

    while (true)
    {
        uint n2 = p.findEarNode();
        if (n2 == RConstants::eod)
        {
            break;
        }
        uint n1 = (n2 > 0) ? n2 - 1 : p.getNNodes() - 1;
        uint n3 = (n2 < p.getNNodes() - 1) ? n2 + 1 : 0;

        RElement element(R_ELEMENT_TRI1);
        element.setNodeId(0,nodeIDs[book[n1]]);
        element.setNodeId(1,nodeIDs[book[n2]]);
        element.setNodeId(2,nodeIDs[book[n3]]);

        elements.push_back(element);

        p.removeNode(n2);
        book.erase(book.begin()+n2);
    }

    return elements;
}

void RPolygon::computeNormal(void)
{
    this->normal[0] = this->normal[1] = this->normal[2] = 0.0;

    if (this->nodes.size() < 3)
    {
        return;
    }

    for (uint i=2;i<this->nodes.size();i++)
    {
        RTriangle t(this->nodes[0],this->nodes[1],this->nodes[i]);
        RR3Vector n(t.getNormal());
        this->normal[0] += n[0];
        this->normal[1] += n[1];
        this->normal[2] += n[2];
    }

    this->normal[0] /= double(this->nodes.size() - 2);
    this->normal[1] /= double(this->nodes.size() - 2);
    this->normal[2] /= double(this->nodes.size() - 2);
}

uint RPolygon::findEarNode(void) const
{
    if (this->getNNodes() < 3)
    {
        return RConstants::eod;
    }

    uint earNode = RConstants::eod;
    double earAngle = 0.0;

    for (uint i=2;i<this->getNNodes();i++)
    {
        uint n1 = i-2;
        uint n2 = i-1;
        uint n3 = i;

        RR3Vector v12(this->nodes[n2].getX()-this->nodes[n1].getX(),
                      this->nodes[n2].getY()-this->nodes[n1].getY(),
                      this->nodes[n2].getZ()-this->nodes[n1].getZ());

        RR3Vector v23(this->nodes[n3].getX()-this->nodes[n2].getX(),
                      this->nodes[n3].getY()-this->nodes[n2].getY(),
                      this->nodes[n3].getZ()-this->nodes[n2].getZ());

        RR3Vector v31(this->nodes[n1].getX()-this->nodes[n3].getX(),
                      this->nodes[n1].getY()-this->nodes[n3].getY(),
                      this->nodes[n1].getZ()-this->nodes[n3].getZ());

        double angle = 0.0;

        angle = RRVector::angle(v12,v23);
        if (angle > earAngle)
        {
            angle = earAngle;
            earNode = n2;
        }

        angle = RRVector::angle(v23,v31);
        if (angle > earAngle)
        {
            angle = earAngle;
            earNode = n3;
        }

        angle = RRVector::angle(v31,v12);
        if (angle > earAngle)
        {
            angle = earAngle;
            earNode = n1;
        }
    }

    return earNode;
}
