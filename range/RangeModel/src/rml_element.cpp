/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_element.cpp                                          *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   8-th December 2011                                       *
 *                                                                   *
 *  DESCRIPTION: Element class definition                            *
 *********************************************************************/

#include <vector>
#include <string>
#include <cmath>

#include "rml_element.h"
#include "rml_element_shape_function.h"
#include "rml_interpolated_element.h"
#include "rml_triangle.h"
#include "rml_tetrahedron.h"
#include "rml_triangulate.h"

typedef struct _RElementDesc
{
    QString                            id;
    QString                            name;
    unsigned int                       nNodes;
    unsigned int                       nNeighbors;
    std::vector<RElementShapeFunction> shapeFunctions;
    RRMatrix                           M;
    RRVector                           m;
    double                             wt;
    bool                               constantDerivative;
} RElementDesc;

static const RElementDesc elementDesc [] =
{
    {
        "elem-none",
        "None",
        0,
        0,
        RElement::generateShapeFunctionValues(R_ELEMENT_NONE),
        RElement::generateMassMatrix(R_ELEMENT_NONE),
        RElement::generateMassVector(R_ELEMENT_NONE),
        RElement::generateTotalWeightFactor(R_ELEMENT_NONE),
        false
    },
    {
        "elem-point",
        "Point",
        1,
        0,
        RElement::generateShapeFunctionValues(R_ELEMENT_POINT),
        RElement::generateMassMatrix(R_ELEMENT_POINT),
        RElement::generateMassVector(R_ELEMENT_POINT),
        RElement::generateTotalWeightFactor(R_ELEMENT_POINT),
        false
    },
    {
        "elem-truss1",
        "Truss1",
        2,
        2,
        RElement::generateShapeFunctionValues(R_ELEMENT_TRUSS1),
        RElement::generateMassMatrix(R_ELEMENT_TRUSS1),
        RElement::generateMassVector(R_ELEMENT_TRUSS1),
        RElement::generateTotalWeightFactor(R_ELEMENT_TRUSS1),
        true
    },
    {
        "elem-truss2",
        "Truss2",
        3,
        2,
        RElement::generateShapeFunctionValues(R_ELEMENT_TRUSS2),
        RElement::generateMassMatrix(R_ELEMENT_TRUSS2),
        RElement::generateMassVector(R_ELEMENT_TRUSS2),
        RElement::generateTotalWeightFactor(R_ELEMENT_TRUSS2),
        false
    },
    {
        "elem-beam1",
        "Beam1",
        2,
        2,
        RElement::generateShapeFunctionValues(R_ELEMENT_BEAM1),
        RElement::generateMassMatrix(R_ELEMENT_BEAM1),
        RElement::generateMassVector(R_ELEMENT_BEAM1),
        RElement::generateTotalWeightFactor(R_ELEMENT_BEAM1),
        false
    },
    {
        "elem-beam2",
        "Beam2",
        3,
        2,
        RElement::generateShapeFunctionValues(R_ELEMENT_BEAM2),
        RElement::generateMassMatrix(R_ELEMENT_BEAM2),
        RElement::generateMassVector(R_ELEMENT_BEAM2),
        RElement::generateTotalWeightFactor(R_ELEMENT_BEAM2),
        false
    },
    {
        "elem-triangle1",
        "Triangle1",
        3,
        3,
        RElement::generateShapeFunctionValues(R_ELEMENT_TRI1),
        RElement::generateMassMatrix(R_ELEMENT_TRI1),
        RElement::generateMassVector(R_ELEMENT_TRI1),
        RElement::generateTotalWeightFactor(R_ELEMENT_TRI1),
        true
    },
    {
        "elem-triangle2",
        "Triangle2",
        6,
        3,
        RElement::generateShapeFunctionValues(R_ELEMENT_TRI2),
        RElement::generateMassMatrix(R_ELEMENT_TRI2),
        RElement::generateMassVector(R_ELEMENT_TRI2),
        RElement::generateTotalWeightFactor(R_ELEMENT_TRI2),
        false
    },
    {
        "elem-1uadrilateral1",
        "Quadrilateral1",
        4,
        4,
        RElement::generateShapeFunctionValues(R_ELEMENT_QUAD1),
        RElement::generateMassMatrix(R_ELEMENT_QUAD1),
        RElement::generateMassVector(R_ELEMENT_QUAD1),
        RElement::generateTotalWeightFactor(R_ELEMENT_QUAD1),
        false
    },
    {
        "elem-quadrilateral2",
        "Quadrilateral2",
        9,
        4,
        RElement::generateShapeFunctionValues(R_ELEMENT_QUAD2),
        RElement::generateMassMatrix(R_ELEMENT_QUAD2),
        RElement::generateMassVector(R_ELEMENT_QUAD2),
        RElement::generateTotalWeightFactor(R_ELEMENT_QUAD2),
        false
    },
    {
        "elem-tetrahedron1",
        "Tetrahedron1",
        4,
        4,
        RElement::generateShapeFunctionValues(R_ELEMENT_TETRA1),
        RElement::generateMassMatrix(R_ELEMENT_TETRA1),
        RElement::generateMassVector(R_ELEMENT_TETRA1),
        RElement::generateTotalWeightFactor(R_ELEMENT_TETRA1),
        true
    },
    {
        "elem-wedge1",
        "Wedge1",
        6,
        5,
        RElement::generateShapeFunctionValues(R_ELEMENT_WEDGE1),
        RElement::generateMassMatrix(R_ELEMENT_WEDGE1),
        RElement::generateMassVector(R_ELEMENT_WEDGE1),
        RElement::generateTotalWeightFactor(R_ELEMENT_WEDGE1),
        false
    },
    {
        "elem-hexahedron1",
        "Hexahedron1",
        8,
        6,
        RElement::generateShapeFunctionValues(R_ELEMENT_HEXA1),
        RElement::generateMassMatrix(R_ELEMENT_HEXA1),
        RElement::generateMassVector(R_ELEMENT_HEXA1),
        RElement::generateTotalWeightFactor(R_ELEMENT_HEXA1),
        false
    }
};


RElement::RElement (RElementType type)
{
    this->setType (type);
    this->_init ();
} /* RElement::RElement */


RElement::RElement (const RElement &element)
{
    this->_init (&element);
} /* RElement::RElement */


RElement::~RElement ()
{
} /* RElement::~RElement */


void RElement::_init (const RElement *element)
{
    if (element)
    {
        this->setType (element->getType());
        for (unsigned int i=0;i<this->size ();i++)
        {
            this->setNodeId (i, element->getNodeId(i));
        }
    }
} /* RElement::_init */


RElement & RElement::operator = (const RElement &element)
{
    this->_init (&element);
    return (*this);
} /* RElement::operator = */


bool RElement::operator ==(const RElement &element) const
{
    if (this->type != element.type)
    {
        return false;
    }
    for (uint i=0;i<this->nodeIDs.size();i++)
    {
        bool nodeFound = false;
        for (uint j=0;j<element.nodeIDs.size();j++)
        {
            if (this->nodeIDs[i] == element.nodeIDs[j])
            {
                nodeFound = true;
                break;
            }
        }
        if (!nodeFound)
        {
            return false;
        }
    }
    return true;
} /* RElement::operator == */

bool RElement::operator <(const RElement &element) const
{
    if (this->type < element.type)
    {
        return true;
    }
    else if (this->type > element.type)
    {
        return false;
    }

    std::vector<uint> n1Nodes(this->nodeIDs);
    std::vector<uint> n2Nodes(element.nodeIDs);

    std::sort(n1Nodes.begin(),n1Nodes.end());
    std::sort(n2Nodes.begin(),n2Nodes.end());

    for (uint i=0;i<n1Nodes.size();i++)
    {
        if (n1Nodes[i] < n2Nodes[i])
        {
            return true;
        }
        else if (n1Nodes[i] > n2Nodes[i])
        {
            return false;
        }
    }

    return false;
} /* RElement::operator < */


RElementType RElement::getType (void) const
{
    return this->type;
} /* RElement::getType */


void RElement::setType (RElementType type)
{
    R_ERROR_ASSERT (R_ELEMENT_TYPE_IS_VALID (type));

    this->type = type;
    this->nodeIDs.resize (RElement::getNNodes(type));
} /* RElement::setType */


unsigned int RElement::size (void) const
{
    return static_cast<unsigned int>(this->nodeIDs.size());
} /* RElement::size */


unsigned int RElement::getNodeId (unsigned int position) const
{
    R_ERROR_ASSERT (position < this->nodeIDs.size());

    return this->nodeIDs[position];
} /* RElement::getNodeId */


void RElement::setNodeId (unsigned int position,
                            unsigned int nodeID)
{
    R_ERROR_ASSERT (position < this->nodeIDs.size());
    this->nodeIDs[position] = nodeID;
} /* RElement::setNodeId */


void RElement::swapNodeIds(unsigned int position1, unsigned int position2)
{
    R_ERROR_ASSERT (position1 < this->nodeIDs.size());
    R_ERROR_ASSERT (position2 < this->nodeIDs.size());
    std::swap(this->nodeIDs[position1],this->nodeIDs[position2]);
} /* RElement::swapNodeIds */


bool RElement::hasNodeId (unsigned int nodeID) const
{
    for (unsigned int i=0;i<this->size();i++)
    {
        if (nodeID == this->getNodeId(i))
        {
           return true;
        }
    }
    return false;
} /* RElement::hasNodeId */


bool RElement::hasDuplicateNodes(void) const
{
    for (uint i=0;i<this->size();i++)
    {
        for (uint j=i+1;j<this->size();j++)
        {
            if (this->getNodeId(i) == this->getNodeId(j))
            {
                return true;
            }
        }
    }

    return false;
} /* RElement::hasDuplicateNodes */


bool RElement::swapNormal(void)
{
    if (!R_ELEMENT_TYPE_IS_SURFACE(this->getType()))
    {
        return false;
    }

    if (this->getType() == R_ELEMENT_TRI1 ||
        this->getType() == R_ELEMENT_TRI2)
    {
        std::swap(this->nodeIDs[0],this->nodeIDs[1]);
    }
    else if (this->getType() == R_ELEMENT_QUAD1 ||
             this->getType() == R_ELEMENT_QUAD2)
    {
        std::swap(this->nodeIDs[1],this->nodeIDs[3]);
    }

    return true;
} /* RElement::swapNormal */


bool RElement::findNormal(const std::vector<RNode> &nodes, double &nx, double &ny, double &nz) const
{
    if (!R_ELEMENT_TYPE_IS_SURFACE(this->getType()))
    {
        return false;
    }

    if (this->getType() == R_ELEMENT_TRI1 ||
        this->getType() == R_ELEMENT_TRI2)
    {
        RTriangle triangle(nodes[this->getNodeId(0)],
                           nodes[this->getNodeId(1)],
                           nodes[this->getNodeId(2)]);
        nx = triangle.getNormal()[0];
        ny = triangle.getNormal()[1];
        nz = triangle.getNormal()[2];
    }
    if (this->getType() == R_ELEMENT_QUAD1 ||
        this->getType() == R_ELEMENT_QUAD2)
    {
        RTriangle triangle1(nodes[this->getNodeId(0)],
                            nodes[this->getNodeId(1)],
                            nodes[this->getNodeId(2)]);
        RTriangle triangle2(nodes[this->getNodeId(2)],
                            nodes[this->getNodeId(3)],
                            nodes[this->getNodeId(0)]);
        nx = (triangle1.getNormal()[0] + triangle2.getNormal()[0])/2.0;
        ny = (triangle1.getNormal()[1] + triangle2.getNormal()[1])/2.0;
        nz = (triangle1.getNormal()[2] + triangle2.getNormal()[2])/2.0;
    }

    return true;
} /* RElement::findNormal */


void RElement::findCenter(const std::vector<RNode> &nodes, double &cx, double &cy, double &cz) const
{
    cx = cy = cz = 0.0;

    for (unsigned int i=0;i<this->size();i++)
    {
        cx += nodes[this->getNodeId(i)].getX();
        cy += nodes[this->getNodeId(i)].getY();
        cz += nodes[this->getNodeId(i)].getZ();
    }

    if (this->size() > 0)
    {
        cx /= double(this->size());
        cy /= double(this->size());
        cz /= double(this->size());
    }
} /* RElement::findCenter */


bool RElement::findShortestEdge(const std::vector<RNode> &nodes, double &length, uint &node1, uint &node2) const
{
    if (!R_ELEMENT_TYPE_IS_SURFACE(this->getType()))
    {
        return false;
    }

    switch (this->getType())
    {
        case R_ELEMENT_TRI1:
        case R_ELEMENT_TRI2:
        {
            const RNode &n1 = nodes[this->getNodeId(0)];
            const RNode &n2 = nodes[this->getNodeId(1)];
            const RNode &n3 = nodes[this->getNodeId(2)];

            double l1 = n1.getDistance(n2);
            double l2 = n2.getDistance(n3);
            double l3 = n3.getDistance(n1);

            length = l1;
            node1 = 0;
            node2 = 1;

            if (l2 < length)
            {
                length = l2;
                node1 = 1;
                node2 = 2;
            }
            if (l3 < length)
            {
                length = l3;
                node1 = 2;
                node2 = 0;
            }

            break;
        }
        case R_ELEMENT_QUAD1:
        case R_ELEMENT_QUAD2:
        {
            const RNode &n1 = nodes[this->getNodeId(0)];
            const RNode &n2 = nodes[this->getNodeId(1)];
            const RNode &n3 = nodes[this->getNodeId(2)];
            const RNode &n4 = nodes[this->getNodeId(3)];

            double l1 = n1.getDistance(n2);
            double l2 = n2.getDistance(n3);
            double l3 = n3.getDistance(n4);
            double l4 = n4.getDistance(n1);

            length = l1;
            node1 = 0;
            node2 = 1;

            if (l2 < length)
            {
                length = l2;
                node1 = 1;
                node2 = 2;
            }
            if (l3 < length)
            {
                length = l3;
                node1 = 2;
                node2 = 3;
            }
            if (l4 < length)
            {
                length = l4;
                node1 = 3;
                node2 = 0;
            }

            break;
        }
        default:
        {
            return false;
        }
    }

    return true;
} /* RElement::findShortestEdge */


bool RElement::findLength(const std::vector<RNode> &nodes, double &length) const
{
    if (!R_ELEMENT_TYPE_IS_LINE(this->getType()))
    {
        return false;
    }

    length = nodes[this->getNodeId(0)].getDistance(nodes[this->getNodeId(1)]);

    return true;
} /* RElement::findLength */


bool RElement::findArea(const std::vector<RNode> &nodes, double &area) const
{
    if (!R_ELEMENT_TYPE_IS_SURFACE(this->getType()))
    {
        return false;
    }

    if (this->getType() == R_ELEMENT_TRI1 ||
        this->getType() == R_ELEMENT_TRI2)
    {
        RTriangle triangle(nodes[this->getNodeId(0)],
                           nodes[this->getNodeId(1)],
                           nodes[this->getNodeId(2)]);
        area = triangle.findArea();
    }
    if (this->getType() == R_ELEMENT_QUAD1 ||
        this->getType() == R_ELEMENT_QUAD2)
    {
        RTriangle triangle1(nodes[this->getNodeId(0)],
                            nodes[this->getNodeId(1)],
                            nodes[this->getNodeId(2)]);
        RTriangle triangle2(nodes[this->getNodeId(2)],
                            nodes[this->getNodeId(3)],
                            nodes[this->getNodeId(0)]);
        area = triangle1.findArea() + triangle2.findArea();
    }

    return true;
} /* RElement::findArea */


bool RElement::findVolume(const std::vector<RNode> &nodes, double &volume) const
{
    if (!R_ELEMENT_TYPE_IS_VOLUME(this->getType()))
    {
        return false;
    }

    if (this->getType() == R_ELEMENT_TETRA1)
    {
        const RNode &n1 = nodes[this->getNodeId(0)];
        const RNode &n2 = nodes[this->getNodeId(1)];
        const RNode &n3 = nodes[this->getNodeId(2)];
        const RNode &n4 = nodes[this->getNodeId(3)];
        volume = RTetrahedron::findVolume(n1,n2,n3,n4);
    }

    return true;
} /* RElement::findVolume */


double RElement::findSize(const std::vector<RNode> &nodes) const
{
    double elementSize = 0.0;

    if (R_ELEMENT_TYPE_IS_POINT(this->type))
    {
        elementSize = 0.0;
    }
    else if (R_ELEMENT_TYPE_IS_LINE(this->type))
    {
        double length = 0.0;
        if (this->findLength(nodes,length))
        {
            elementSize = length;
        }
    }
    else if (R_ELEMENT_TYPE_IS_SURFACE(this->type))
    {
        double area = 0.0;
        if (this->findArea(nodes,area))
        {
            if (this->type == R_ELEMENT_TRI1 || this->type == R_ELEMENT_TRI2)
            {
                elementSize = std::sqrt(2.0*area / RConstants::pi);
            }
            else if (this->type == R_ELEMENT_QUAD1 || this->type == R_ELEMENT_QUAD2)
            {
                elementSize = std::sqrt(area);
            }
        }
    }
    else if (R_ELEMENT_TYPE_IS_VOLUME(this->type))
    {
        double volume = 0.0;
        if (this->findVolume(nodes,volume))
        {
            if (this->type == R_ELEMENT_TETRA1)
            {
                elementSize = std::pow(6.0 * volume / RConstants::pi, 1.0/3.0);
            }
            else if (this->type == R_ELEMENT_HEXA1)
            {
                elementSize = std::pow(volume, 1.0/3.0);
            }
        }
    }
    return elementSize;
} /* RElement::findSize */

bool RElement::findPickDistance(const std::vector<RNode> &nodes,
                                const RR3Vector &position,
                                const RR3Vector &direction,
                                double tolerance,
                                double &distance) const
{
    if (R_ELEMENT_TYPE_IS_POINT(this->type))
    {
        const RNode &node = nodes[this->getNodeId(0)];
        RSegment s(RNode(position),RNode(position[0]+direction[0],position[1]+direction[1],position[2]+direction[2]));
        double u = s.findPointDistance(node.toVector());
        if (u <= tolerance)
        {
            double lp = node.getDistance(RNode(position));
            distance = std::sqrt(lp*lp-u*u);
            return true;
        }
    }
    else if (R_ELEMENT_TYPE_IS_LINE(this->type))
    {
        std::vector<RSegment> segments = this->segmentize(nodes);
        double d;
        bool found = false;
        for (uint i=0;i<segments.size();i++)
        {
            d = segments[i].findSegmentDistance(position,direction);
            if (d <= tolerance)
            {
                if (!found || d < distance)
                {
                    distance = d;
                    found = true;
                }
            }
        }
        return found;
    }
    else if (R_ELEMENT_TYPE_IS_SURFACE(this->type))
    {
        std::vector<RTriangle> triangles = this->triangulate(nodes);
        for (uint i=0;i<triangles.size();i++)
        {
            RR3Vector x;
            if (triangles[i].findLineIntersection(position,direction,x,distance))
            {
                return true;
            }
        }
    }
    else if (R_ELEMENT_TYPE_IS_VOLUME(this->type))
    {
        std::vector<RElement> edges = this->generateEdgeElements();

        double u;
        bool found = false;
        for (uint i=0;i<edges.size();i++)
        {
            if (edges[i].findPickDistance(nodes,position,direction,tolerance,u))
            {
                if (!found || distance > u)
                {
                    found = true;
                    distance = u;
                }
            }
        }
        return found;
    }
    return false;
} /* RElement::findPickDistance */


RInterpolatedElement RElement::createInterpolatedElement(const RPlane &plane, const std::vector<RNode> &nodes, unsigned int elementID) const
{
    RInterpolatedElement iElement;

    switch (this->getType())
    {
        case R_ELEMENT_TRUSS1:
        {
            const RNode &node1 = nodes[this->getNodeId(0)];
            const RNode &node2 = nodes[this->getNodeId(1)];

            RR3Vector x;

            if (this->testPlaneIntersect(plane,nodes))
            {
                if (plane.findSegmentIntersection(node1.toVector(),node2.toVector(),x))
                {
                    iElement.push_back(RInterpolatedNode(elementID,x[0],x[1],x[2]));
                }
            }
            break;
        }
        case R_ELEMENT_TRI1:
        {
            const RNode &node1 = nodes[this->getNodeId(0)];
            const RNode &node2 = nodes[this->getNodeId(1)];
            const RNode &node3 = nodes[this->getNodeId(2)];

            RR3Vector x;

            if (this->testPlaneIntersect(plane,nodes))
            {
                if (plane.findSegmentIntersection(node1.toVector(),node2.toVector(),x))
                {
                    iElement.push_back(RInterpolatedNode(elementID,x[0],x[1],x[2]));
                }

                if (plane.findSegmentIntersection(node2.toVector(),node3.toVector(),x))
                {
                    iElement.push_back(RInterpolatedNode(elementID,x[0],x[1],x[2]));
                }

                if (plane.findSegmentIntersection(node3.toVector(),node1.toVector(),x))
                {
                    iElement.push_back(RInterpolatedNode(elementID,x[0],x[1],x[2]));
                }
            }
            break;
        }
        case R_ELEMENT_TETRA1:
        {
            const RNode &node1 = nodes[this->getNodeId(0)];
            const RNode &node2 = nodes[this->getNodeId(1)];
            const RNode &node3 = nodes[this->getNodeId(2)];
            const RNode &node4 = nodes[this->getNodeId(3)];

            RR3Vector x;

            if (this->testPlaneIntersect(plane,nodes))
            {
                if (plane.findSegmentIntersection(node1.toVector(),node2.toVector(),x))
                {
                    iElement.push_back(RInterpolatedNode(elementID,x[0],x[1],x[2]));
                }

                if (plane.findSegmentIntersection(node1.toVector(),node3.toVector(),x))
                {
                    iElement.push_back(RInterpolatedNode(elementID,x[0],x[1],x[2]));
                }

                if (plane.findSegmentIntersection(node1.toVector(),node4.toVector(),x))
                {
                    iElement.push_back(RInterpolatedNode(elementID,x[0],x[1],x[2]));
                }

                if (plane.findSegmentIntersection(node2.toVector(),node3.toVector(),x))
                {
                    iElement.push_back(RInterpolatedNode(elementID,x[0],x[1],x[2]));
                }

                if (plane.findSegmentIntersection(node3.toVector(),node4.toVector(),x))
                {
                    iElement.push_back(RInterpolatedNode(elementID,x[0],x[1],x[2]));
                }

                if (plane.findSegmentIntersection(node4.toVector(),node2.toVector(),x))
                {
                    iElement.push_back(RInterpolatedNode(elementID,x[0],x[1],x[2]));
                }
            }
            iElement.sortNodes();
            break;
        }
        default:
        {
            break;
        }
    }

    return iElement;
} /* RElement::createInterpolatedElement */


RInterpolatedElement RElement::createInterpolatedElement(double value, const std::vector<double> &nodeValues, const std::vector<RNode> &nodes, unsigned int elementID) const
{
    RInterpolatedElement iElement;

    bool isBigger = false;
    bool isSmaller = false;
    for (unsigned int i=0;i<nodeValues.size();i++)
    {
        if (value <= nodeValues[i])
        {
            isSmaller = true;
        }
        if (value >= nodeValues[i])
        {
            isBigger = true;
        }
    }
    if (!isSmaller || !isBigger)
    {
        return iElement;
    }

    switch (this->getType())
    {
        case R_ELEMENT_TRUSS1:
        {
            const RNode &node1 = nodes[this->getNodeId(0)];
            const RNode &node2 = nodes[this->getNodeId(1)];

            double dist = nodeValues[1] - nodeValues[0];
            double ratio = (dist != 0.0) ? (value - nodeValues[0]) / dist : 0.0;
            if (ratio >= 0.0 && ratio <= 1.0)
            {
                iElement.push_back(RInterpolatedNode(elementID,
                                                     ratio * (node2.getX() - node1.getX()) + node1.getX(),
                                                     ratio * (node2.getY() - node1.getY()) + node1.getY(),
                                                     ratio * (node2.getZ() - node1.getZ()) + node1.getZ()));
            }
            break;
        }
        case R_ELEMENT_TRI1:
        {
            const RNode &node1 = nodes[this->getNodeId(0)];
            const RNode &node2 = nodes[this->getNodeId(1)];
            const RNode &node3 = nodes[this->getNodeId(2)];

            double dist = 0.0;
            double ratio = 0.0;

            dist = nodeValues[1] - nodeValues[0];
            ratio = (dist != 0.0) ? (value - nodeValues[0]) / dist : 0.0;
            if (ratio > 0.0 && ratio <= 1.0)
            {
                iElement.push_back(RInterpolatedNode(elementID,
                                                     ratio * (node2.getX() - node1.getX()) + node1.getX(),
                                                     ratio * (node2.getY() - node1.getY()) + node1.getY(),
                                                     ratio * (node2.getZ() - node1.getZ()) + node1.getZ()));
            }

            dist = nodeValues[2] - nodeValues[1];
            ratio = (dist != 0.0) ? (value - nodeValues[1]) / dist : 0.0;
            if (ratio > 0.0 && ratio <= 1.0)
            {
                iElement.push_back(RInterpolatedNode(elementID,
                                                     ratio * (node3.getX() - node2.getX()) + node2.getX(),
                                                     ratio * (node3.getY() - node2.getY()) + node2.getY(),
                                                     ratio * (node3.getZ() - node2.getZ()) + node2.getZ()));
            }

            dist = nodeValues[0] - nodeValues[2];
            ratio = (dist != 0.0) ? (value - nodeValues[2]) / dist : 0.0;
            if (ratio > 0.0 && ratio <= 1.0)
            {
                iElement.push_back(RInterpolatedNode(elementID,
                                                     ratio * (node1.getX() - node3.getX()) + node3.getX(),
                                                     ratio * (node1.getY() - node3.getY()) + node3.getY(),
                                                     ratio * (node1.getZ() - node3.getZ()) + node3.getZ()));
            }
            break;
        }
        case R_ELEMENT_TETRA1:
        {
            const RNode &node1 = nodes[this->getNodeId(0)];
            const RNode &node2 = nodes[this->getNodeId(1)];
            const RNode &node3 = nodes[this->getNodeId(2)];
            const RNode &node4 = nodes[this->getNodeId(3)];

            double dist = 0.0;
            double ratio = 0.0;

            // 1 - 2
            dist = nodeValues[1] - nodeValues[0];
            ratio = (dist != 0.0) ? (value - nodeValues[0]) / dist : 0.0;
            if (ratio > 0.0 && ratio <= 1.0)
            {
                iElement.push_back(RInterpolatedNode(elementID,
                                                     ratio * (node2.getX() - node1.getX()) + node1.getX(),
                                                     ratio * (node2.getY() - node1.getY()) + node1.getY(),
                                                     ratio * (node2.getZ() - node1.getZ()) + node1.getZ()));
            }

            // 1 - 3
            dist = nodeValues[2] - nodeValues[0];
            ratio = (dist != 0.0) ? (value - nodeValues[0]) / dist : 0.0;
            if (ratio > 0.0 && ratio <= 1.0)
            {
                iElement.push_back(RInterpolatedNode(elementID,
                                                     ratio * (node3.getX() - node1.getX()) + node1.getX(),
                                                     ratio * (node3.getY() - node1.getY()) + node1.getY(),
                                                     ratio * (node3.getZ() - node1.getZ()) + node1.getZ()));
            }

            // 1 - 4
            dist = nodeValues[3] - nodeValues[0];
            ratio = (dist != 0.0) ? (value - nodeValues[0]) / dist : 0.0;
            if (ratio > 0.0 && ratio <= 1.0)
            {
                iElement.push_back(RInterpolatedNode(elementID,
                                                     ratio * (node4.getX() - node1.getX()) + node1.getX(),
                                                     ratio * (node4.getY() - node1.getY()) + node1.getY(),
                                                     ratio * (node4.getZ() - node1.getZ()) + node1.getZ()));
            }

            // 2 - 3
            dist = nodeValues[2] - nodeValues[1];
            ratio = (dist != 0.0) ? (value - nodeValues[1]) / dist : 0.0;
            if (ratio > 0.0 && ratio <= 1.0)
            {
                iElement.push_back(RInterpolatedNode(elementID,
                                                     ratio * (node3.getX() - node2.getX()) + node2.getX(),
                                                     ratio * (node3.getY() - node2.getY()) + node2.getY(),
                                                     ratio * (node3.getZ() - node2.getZ()) + node2.getZ()));
            }

            // 2 - 4
            ratio = (dist != 0.0) ? (value - nodeValues[1]) / dist : 0.0;
            if (ratio > 0.0 && ratio <= 1.0)
            {
                iElement.push_back(RInterpolatedNode(elementID,
                                                     ratio * (node4.getX() - node2.getX()) + node2.getX(),
                                                     ratio * (node4.getY() - node2.getY()) + node2.getY(),
                                                     ratio * (node4.getZ() - node2.getZ()) + node2.getZ()));
            }

            // 3 - 4
            dist = nodeValues[3] - nodeValues[2];
            ratio = (dist != 0.0) ? (value - nodeValues[2]) / dist : 0.0;
            if (ratio > 0.0 && ratio <= 1.0)
            {
                iElement.push_back(RInterpolatedNode(elementID,
                                                     ratio * (node4.getX() - node3.getX()) + node3.getX(),
                                                     ratio * (node4.getY() - node3.getY()) + node3.getY(),
                                                     ratio * (node4.getZ() - node3.getZ()) + node3.getZ()));
            }

            iElement.sortNodes();
            break;
        }
        default:
        {
            break;
        }
    }

    return iElement;
} /* RElement::createInterpolatedElement */


bool RElement::isInside(const std::vector<RNode> &nodes, const RNode &node) const
{
    RRVector volumes;
    return this->isInside(nodes,node,volumes);
} /* RElement::isInside */


bool RElement::isInside(const std::vector<RNode> &nodes, const RNode &node, RRVector &volumes) const
{
    switch (this->getType())
    {
        case R_ELEMENT_POINT:
        {
            volumes.resize(0);
            return (std::abs(nodes[this->getNodeId(0)].getDistance(node)) < RConstants::eps);
        }
        case R_ELEMENT_TRUSS1:
        {
            if (!this->isInsideBoundingBox(nodes,node))
            {
                return false;
            }
            const RNode &n1 = nodes[this->getNodeId(0)];
            const RNode &n2 = nodes[this->getNodeId(1)];
            volumes.resize(2);
            volumes[0] = n1.getDistance(node);
            volumes[1] = n2.getDistance(node);
            double bt = volumes[0] + volumes[1];

            return (std::abs(n1.getDistance(n2) - bt) < RConstants::eps);
        }
        case R_ELEMENT_TRI1:
        {
            if (!this->isInsideBoundingBox(nodes,node))
            {
                return false;
            }
            const RNode &n1 = nodes[this->getNodeId(0)];
            const RNode &n2 = nodes[this->getNodeId(1)];
            const RNode &n3 = nodes[this->getNodeId(2)];

            volumes.resize(3);
            volumes[0] = RTriangle::findArea(node,n2,n3);
            volumes[1] = RTriangle::findArea(node,n3,n1);
            volumes[2] = RTriangle::findArea(node,n1,n2);
            double bt = volumes[0] + volumes[1] + volumes[2];

            double area = 0.0;

            if (!this->findArea(nodes,area))
            {
                return false;
            }

            return (std::abs(area - bt) < RConstants::eps);
        }
        case R_ELEMENT_TETRA1:
        {
            if (!this->isInsideBoundingBox(nodes,node))
            {
                return false;
            }
            double volume = 0.0;

            if (!this->findVolume(nodes,volume))
            {
                return false;
            }

            const RNode &n1 = nodes[this->getNodeId(0)];
            const RNode &n2 = nodes[this->getNodeId(1)];
            const RNode &n3 = nodes[this->getNodeId(2)];
            const RNode &n4 = nodes[this->getNodeId(3)];

            double b1 = RTetrahedron::findVolume(node,n2,n4,n3);
            if (!R_SIGNS_EQUAL(b1,volume) && b1 != 0.0)
            {
                return false;
            }
            double b2 = RTetrahedron::findVolume(node,n1,n3,n4);
            if (!R_SIGNS_EQUAL(b2,volume) && b2 != 0.0)
            {
                return false;
            }
            double b3 = RTetrahedron::findVolume(node,n1,n4,n2);
            if (!R_SIGNS_EQUAL(b3,volume) && b3 != 0.0)
            {
                return false;
            }
            double b4 = RTetrahedron::findVolume(node,n1,n2,n3);
            if (!R_SIGNS_EQUAL(b4,volume) && b4 != 0.0)
            {
                return false;
            }
            volumes.resize(4);
            volumes[0] = b1;
            volumes[1] = b2;
            volumes[2] = b3;
            volumes[3] = b4;
            double bt = b1 + b2 + b3 + b4;

            return (std::abs(volume - bt) < RConstants::eps);
        }
        default:
        {
            break;
        }
    }

    return false;
} /* RElement::isInside */


double RElement::interpolate(const std::vector<RNode> &nodes, const RNode &interpolatedNode, const RRVector &nodeValues) const
{
    RRVector volumes;
    return this->interpolate(nodes,interpolatedNode,nodeValues,volumes);
} /* RElement::interpolate */


double RElement::interpolate(const std::vector<RNode> &nodes,
                             const RNode &interpolatedNode,
                             const RRVector &nodeValues,
                             const RRVector &volumes) const
{
    RRVector ratios;

    switch (this->getType())
    {
        case R_ELEMENT_POINT:
        {
            ratios.resize(1,1.0);
            break;
        }
        case R_ELEMENT_TRUSS1:
        {
            ratios.resize(2,1.0);
            const RNode &n1 = nodes[this->getNodeId(0)];
            const RNode &n2 = nodes[this->getNodeId(1)];
            double b1 = volumes.size() == 2 ? volumes[0] : n1.getDistance(interpolatedNode);
            double b2 = volumes.size() == 2 ? volumes[1] : n2.getDistance(interpolatedNode);
            double bt = b1 + b2;
            if (bt != 0.0)
            {
                ratios[0] = b1/bt;
                ratios[1] = b2/bt;
            }
            break;
        }
        case R_ELEMENT_TRI1:
        {
            ratios.resize(3,1.0);
            const RNode &n1 = nodes[this->getNodeId(0)];
            const RNode &n2 = nodes[this->getNodeId(1)];
            const RNode &n3 = nodes[this->getNodeId(2)];

//            double b1 = RTriangle::findArea(interpolatedNode,n2,n3);
//            double b2 = RTriangle::findArea(interpolatedNode,n3,n1);
//            double b3 = RTriangle::findArea(interpolatedNode,n1,n2);

            double b1 = volumes.size() == 3 ? volumes[0] : RTriangle::findArea(interpolatedNode,n2,n3);
            double b2 = volumes.size() == 3 ? volumes[1] : RTriangle::findArea(interpolatedNode,n3,n1);
            double b3 = volumes.size() == 3 ? volumes[2] : RTriangle::findArea(interpolatedNode,n1,n2);
            double bt = b1 + b2 + b3;
            if (bt != 0.0)
            {
                ratios[0] = b1/bt;
                ratios[1] = b2/bt;
                ratios[2] = b3/bt;
            }
            break;
        }
        case R_ELEMENT_TETRA1:
        {
            ratios.resize(4,1.0);
            const RNode &n1 = nodes[this->getNodeId(0)];
            const RNode &n2 = nodes[this->getNodeId(1)];
            const RNode &n3 = nodes[this->getNodeId(2)];
            const RNode &n4 = nodes[this->getNodeId(3)];

            double b1 = volumes.size() == 4 ? volumes[0] : RTetrahedron::findVolume(interpolatedNode,n2,n4,n3);
            double b2 = volumes.size() == 4 ? volumes[1] : RTetrahedron::findVolume(interpolatedNode,n1,n3,n4);
            double b3 = volumes.size() == 4 ? volumes[2] : RTetrahedron::findVolume(interpolatedNode,n1,n4,n2);
            double b4 = volumes.size() == 4 ? volumes[3] : RTetrahedron::findVolume(interpolatedNode,n1,n2,n3);
            double bt = b1 + b2 + b3 + b4;
            if (bt != 0.0)
            {
                ratios[0] = b1/bt;
                ratios[1] = b2/bt;
                ratios[2] = b3/bt;
                ratios[3] = b4/bt;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    double value = 0.0;

    for (unsigned int i=0;i<ratios.size();i++)
    {
        value += ratios[i] * nodeValues[i];
    }

    return value;
} /* RElement::interpolate */


RRMatrix RElement::findTransformationMatrix(const std::vector<RNode> &nodes, RRMatrix &R, RRVector &t) const
{
    RRMatrix lNodes;
    R.setIdentity(3);
    t.resize(3);
    t.fill(0.0);

    unsigned int nn = this->size();

    if (R_ELEMENT_TYPE_IS_POINT(this->getType()) || R_ELEMENT_TYPE_IS_VOLUME(this->getType()))
    {
        lNodes.resize(nn,3);

        for (unsigned int i=0;i<nn;i++)
        {
            lNodes[i][0] = nodes[this->getNodeId(i)].getX();
            lNodes[i][1] = nodes[this->getNodeId(i)].getY();
            lNodes[i][2] = nodes[this->getNodeId(i)].getZ();
        }
    }
    else if (R_ELEMENT_TYPE_IS_LINE(this->getType()))
    {
        t[0] = nodes[this->getNodeId(0)].getX();
        t[1] = nodes[this->getNodeId(0)].getY();
        t[2] = nodes[this->getNodeId(0)].getZ();

        lNodes.resize(nn,1);

        RSegment segment(nodes[this->getNodeId(0)],
                         nodes[this->getNodeId(1)]);
        segment.findRotationMatrix(R);

        RRMatrix Ri(R);
        Ri.invert();

        for (unsigned int i=0;i<nn;i++)
        {
            double x = nodes[this->getNodeId(i)].getX();
            double y = nodes[this->getNodeId(i)].getY();
            double z = nodes[this->getNodeId(i)].getZ();
            x -= t[0];
            y -= t[1];
            z -= t[2];
            lNodes[i][0] = Ri[0][0]*x + Ri[0][1]*y + Ri[0][2]*z;
        }
    }
    else if (R_ELEMENT_TYPE_IS_SURFACE(this->getType()))
    {
        lNodes.resize(nn,2);

        RTriangle triangle(nodes[this->getNodeId(0)],
                           nodes[this->getNodeId(1)],
                           nodes[this->getNodeId(2)]);
        triangle.findRotationMatrix(R);

        RRMatrix Ri(R);
        Ri.invert();

        for (unsigned int i=0;i<nn;i++)
        {
            double x = nodes[this->getNodeId(i)].getX();
            double y = nodes[this->getNodeId(i)].getY();
            double z = nodes[this->getNodeId(i)].getZ();
            x -= t[0];
            y -= t[1];
            z -= t[2];
            lNodes[i][0] = Ri[0][0]*x + Ri[0][1]*y + Ri[0][2]*z;
            lNodes[i][1] = Ri[1][0]*x + Ri[1][1]*y + Ri[1][2]*z;
        }
    }
    else
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Element type (%d) has no group type.",this->getType());
    }


    return lNodes;
} /* RElement::findRotationMatrix */


double RElement::findJacobian(const std::vector<RNode> &nodes, unsigned int iPoint, RRMatrix &J, RRMatrix &Rt) const
{
    RRMatrix lNodes;
    RRMatrix R;
    RRVector t;

    const RElementShapeFunction &shapeFunction = RElement::getShapeFunction(this->getType(),iPoint);
    const RRMatrix &dN = shapeFunction.getDN();

    J.resize(dN.getNColumns(),dN.getNColumns());
    J.fill(0.0);
    lNodes.resize(dN.getNRows(),dN.getNColumns());

    lNodes = this->findTransformationMatrix(nodes,R,t);

    for (unsigned int i=0;i<dN.getNColumns();i++)
    {
        for (unsigned int j=0;j<dN.getNColumns();j++)
        {
            for (unsigned int k=0;k<dN.getNRows();k++)
            {
                J[i][j] += dN[k][i]*lNodes[k][j];
            }
        }
    }
    double detJ = J.getDeterminant();
    if (J.getNRows() == 1 && J.getNColumns() == 1)
    {
        if (std::abs(J[0][0]) > RConstants::eps)
        {
            J[0][0] = 1.0 / J[0][0];
        }
    }
    else
    {
        J.invert();
    }

    if (R_ELEMENT_TYPE_IS_LINE(this->getType()))
    {
        Rt.resize(this->size()*3,this->size()*1);

        for (uint i=0;i<this->size();i++)
        {
            for (uint j=0;j<this->size();j++)
            {
                Rt[3*i+0][j] = R[0][0];
                Rt[3*i+1][j] = R[1][0];
                Rt[3*i+2][j] = R[2][0];
            }
        }
    }
    else if (R_ELEMENT_TYPE_IS_SURFACE(this->getType()))
    {
        Rt.resize(this->size()*3,this->size()*2);

        for (uint i=0;i<this->size();i++)
        {
            for (uint j=0;j<this->size();j++)
            {
                Rt[3*i+0][2*j+0] = R[0][0];
                Rt[3*i+1][2*j+0] = R[1][0];
                Rt[3*i+2][2*j+0] = R[2][0];

                Rt[3*i+0][2*j+1] = R[0][1];
                Rt[3*i+1][2*j+1] = R[1][1];
                Rt[3*i+2][2*j+1] = R[2][1];
            }
        }
    }
    else if (R_ELEMENT_TYPE_IS_VOLUME(this->getType()) || R_ELEMENT_TYPE_IS_POINT(this->getType()))
    {
        Rt.setIdentity(this->size()*3);
    }

    return detJ;
} /* RElement::findJacobian */


unsigned int RElement::findIntersectedSide(const std::vector<RNode> &nodes,
                                           const RR3Vector &position,
                                           const RR3Vector &direction,
                                           RR3Vector &intersection) const
{
    switch (this->getType())
    {
        case R_ELEMENT_TETRA1:
        {
            const RNode &n1 = nodes[this->getNodeId(0)];
            const RNode &n2 = nodes[this->getNodeId(1)];
            const RNode &n3 = nodes[this->getNodeId(2)];
            const RNode &n4 = nodes[this->getNodeId(3)];

            double u = 0.0;

            RTriangle t1(n2,n3,n4);
            if (t1.findLineIntersection(position,direction,intersection,u))
            {
                if (u > RConstants::eps)
                {
                    return 0;
                }
            }

            RTriangle t2(n1,n3,n4);
            if (t2.findLineIntersection(position,direction,intersection,u))
            {
                if (u > RConstants::eps)
                {
                    return 1;
                }
            }

            RTriangle t3(n1,n2,n4);
            if (t3.findLineIntersection(position,direction,intersection,u))
            {
                if (u > RConstants::eps)
                {
                    return 2;
                }
            }

            RTriangle t4(n1,n2,n3);
            if (t4.findLineIntersection(position,direction,intersection,u))
            {
                if (u > RConstants::eps)
                {
                    return 3;
                }
            }

            break;
        }
        default:
        {
            RLogger::warning("Unsupported element type.\n");
            break;
        }
    }

    return RConstants::eod;
} /* RElement::findIntersectedSide */

std::vector<RSegment> RElement::segmentize(const std::vector<RNode> &nodes) const
{
    std::vector<RSegment> segments;

    switch (this->type)
    {
        case R_ELEMENT_BEAM1:
        case R_ELEMENT_TRUSS1:
            segments.push_back(RSegment(nodes[this->nodeIDs[0]],nodes[this->nodeIDs[1]]));
            break;
        case R_ELEMENT_BEAM2:
        case R_ELEMENT_TRUSS2:
            segments.push_back(RSegment(nodes[this->nodeIDs[0]],nodes[this->nodeIDs[2]]));
            segments.push_back(RSegment(nodes[this->nodeIDs[2]],nodes[this->nodeIDs[1]]));
            break;
        default:
            break;
    }

    return segments;
} /* RElement::segmentize */

std::vector<RTriangle> RElement::triangulate(const std::vector<RNode> &nodes) const
{
    std::vector<RTriangle> triangles;

    switch (this->type)
    {
        case R_ELEMENT_TRI1:
        case R_ELEMENT_TRI2:
        {
            RTriangle t1(RTriangle(nodes[this->nodeIDs[0]],nodes[this->nodeIDs[1]],nodes[this->nodeIDs[2]]));
            t1.setNodeIDs(this->nodeIDs[0],this->nodeIDs[1],this->nodeIDs[2]);
            triangles.push_back(t1);
            break;
        }
        case R_ELEMENT_QUAD1:
        case R_ELEMENT_QUAD2:
        {
            RTriangle t1(RTriangle(nodes[this->nodeIDs[0]],nodes[this->nodeIDs[1]],nodes[this->nodeIDs[2]]));
            RTriangle t2(RTriangle(nodes[this->nodeIDs[0]],nodes[this->nodeIDs[2]],nodes[this->nodeIDs[3]]));
            t1.setNodeIDs(this->nodeIDs[0],this->nodeIDs[1],this->nodeIDs[2]);
            t2.setNodeIDs(this->nodeIDs[0],this->nodeIDs[2],this->nodeIDs[3]);
            triangles.reserve(2);
            triangles.push_back(t1);
            triangles.push_back(t2);
            break;
        }
        default:
        {
            break;
        }
    }

    return triangles;
} /* RElement::triangulate */


uint RElement::getNEdgeElements(void) const
{
    switch (this->type)
    {
        case R_ELEMENT_TRUSS1:
        {
            return 2;
        }
        case R_ELEMENT_TRI1:
        {
            return 3;
        }
        case R_ELEMENT_QUAD1:
        {
            return 4;
        }
        case R_ELEMENT_TETRA1:
        {
            return 4;
        }
        default:
        {
            return 0;
        }
    }
} /* RElement::getNEdgeElements */


std::vector<RElement> RElement::generateEdgeElements(void) const
{
    std::vector<RElement> edgeElements;

    switch (this->type)
    {
        case R_ELEMENT_TRUSS1:
        {
            RElement e1(R_ELEMENT_POINT);
            e1.setNodeId(0,this->getNodeId(0));
            edgeElements.push_back(e1);

            RElement e2(R_ELEMENT_POINT);
            e2.setNodeId(0,this->getNodeId(1));
            edgeElements.push_back(e2);

            break;
        }
        case R_ELEMENT_TRI1:
        {
            RElement e1(R_ELEMENT_TRUSS1);
            e1.setNodeId(0,this->getNodeId(0));
            e1.setNodeId(1,this->getNodeId(1));
            edgeElements.push_back(e1);

            RElement e2(R_ELEMENT_TRUSS1);
            e2.setNodeId(0,this->getNodeId(1));
            e2.setNodeId(1,this->getNodeId(2));
            edgeElements.push_back(e2);

            RElement e3(R_ELEMENT_TRUSS1);
            e3.setNodeId(0,this->getNodeId(2));
            e3.setNodeId(1,this->getNodeId(0));
            edgeElements.push_back(e3);

            break;
        }
        case R_ELEMENT_QUAD1:
        {
            RElement e1(R_ELEMENT_TRUSS1);
            e1.setNodeId(0,this->getNodeId(0));
            e1.setNodeId(1,this->getNodeId(1));
            edgeElements.push_back(e1);

            RElement e2(R_ELEMENT_TRUSS1);
            e2.setNodeId(0,this->getNodeId(1));
            e2.setNodeId(1,this->getNodeId(2));
            edgeElements.push_back(e2);

            RElement e3(R_ELEMENT_TRUSS1);
            e3.setNodeId(0,this->getNodeId(2));
            e3.setNodeId(1,this->getNodeId(3));
            edgeElements.push_back(e3);

            RElement e4(R_ELEMENT_TRUSS1);
            e4.setNodeId(0,this->getNodeId(3));
            e4.setNodeId(1,this->getNodeId(0));
            edgeElements.push_back(e4);

            break;
        }
        case R_ELEMENT_TETRA1:
        {
            RElement e1(R_ELEMENT_TRI1);
            e1.setNodeId(0,this->getNodeId(1));
            e1.setNodeId(1,this->getNodeId(3));
            e1.setNodeId(2,this->getNodeId(2));
            edgeElements.push_back(e1);

            RElement e2(R_ELEMENT_TRI1);
            e2.setNodeId(0,this->getNodeId(0));
            e2.setNodeId(1,this->getNodeId(2));
            e2.setNodeId(2,this->getNodeId(3));
            edgeElements.push_back(e2);

            RElement e3(R_ELEMENT_TRI1);
            e3.setNodeId(0,this->getNodeId(0));
            e3.setNodeId(1,this->getNodeId(3));
            e3.setNodeId(2,this->getNodeId(1));
            edgeElements.push_back(e3);

            RElement e4(R_ELEMENT_TRI1);
            e4.setNodeId(0,this->getNodeId(0));
            e4.setNodeId(1,this->getNodeId(1));
            e4.setNodeId(2,this->getNodeId(2));
            edgeElements.push_back(e4);

            break;
        }
        default:
        {
            break;
        }
    }

    return edgeElements;
} /* RElement::generateEdgeElements */


bool RElement::isNeighbor(const RElement &rElement) const
{
    if (!R_ELEMENT_GROUP_TYPE_EQUALS(this->getType(),rElement.getType()))
    {
        return RConstants::eod;
    }

    uint nNodesPerSide = 0;

    if (R_ELEMENT_TYPE_IS_LINE(this->getType()))
    {
        nNodesPerSide = 1;
    }
    else if (R_ELEMENT_TYPE_IS_SURFACE(this->getType()))
    {
        nNodesPerSide = 2;
    }
    else if (R_ELEMENT_TYPE_IS_VOLUME(this->getType()))
    {
        if (this->getType() == R_ELEMENT_TETRA1)
        {
            nNodesPerSide = 3;
        }
        if (this->getType() == R_ELEMENT_HEXA1)
        {
            nNodesPerSide = 4;
        }
    }
    if (nNodesPerSide == 0)
    {
        return false;
    }

    uint nFound = 0;
    for (uint i=0;i<rElement.size();i++)
    {
        if (this->hasNodeId(rElement.getNodeId(i)))
        {
            nFound++;
        }
    }
    if (nFound == nNodesPerSide)
    {
        return true;
    }
    return false;
} /* RElement::isNeighbor */


unsigned int RElement::findEdgePositionForNeighborElement(const RElement &rElement) const
{
    if (!R_ELEMENT_GROUP_TYPE_EQUALS(this->getType(),rElement.getType()))
    {
        return RConstants::eod;
    }
    std::vector<RElement> edgeElements = this->generateEdgeElements();
    for (uint i=0;i<edgeElements.size();i++)
    {
        uint nNodesFound = 0;
        for (uint j=0;j<edgeElements[i].size();j++)
        {
            for (uint k=0;k<rElement.size();k++)
            {
                if (edgeElements[i].getNodeId(j) == rElement.getNodeId(k))
                {
                    nNodesFound++;
                    break;
                }
            }
        }
        if (nNodesFound == edgeElements[i].size())
        {
            return i;
        }
    }
    return RConstants::eod;
} /* RElement::findEdgePositionForNeighborElement */


bool RElement::isNeighborNormalSync(const RElement &rElement) const
{
    for (uint i=0;i<this->size();i++)
    {
        uint n11 = this->getNodeId(i);
        uint n12 = (i+1<this->size()) ? this->getNodeId(i+1) : this->getNodeId(0);
        for (uint j=0;j<rElement.size();j++)
        {
            uint n21 = rElement.getNodeId(j);
            uint n22 = (j+1<rElement.size()) ? rElement.getNodeId(j+1) : rElement.getNodeId(0);

            if (n11 == n22 && n12 == n21)
            {
                return true;
            }
            else if (n11 == n21 && n12 == n22)
            {
                return false;
            }
        }
    }
    return true;
} /* RElement::isNeighborNormalSync */


bool RElement::nodeIsOnEdge(uint nodePosition, uint edgePosition) const
{
    switch (this->type)
    {
        case R_ELEMENT_TRUSS1:
        {
            return ((nodePosition == 0 && edgePosition == 0)
                    ||
                    (nodePosition == 1 && edgePosition == 1));
        }
        case R_ELEMENT_TRI1:
        {
            return (((nodePosition == 0 || nodePosition == 1) && edgePosition == 0)
                    ||
                    ((nodePosition == 1 || nodePosition == 2) && edgePosition == 1)
                    ||
                    ((nodePosition == 2 || nodePosition == 0) && edgePosition == 2));
        }
        case R_ELEMENT_QUAD1:
        {
            return (((nodePosition == 0 || nodePosition == 1) && edgePosition == 0)
                    ||
                    ((nodePosition == 1 || nodePosition == 2) && edgePosition == 1)
                    ||
                    ((nodePosition == 2 || nodePosition == 3) && edgePosition == 2)
                    ||
                    ((nodePosition == 3 || nodePosition == 0) && edgePosition == 3));
        }
        case R_ELEMENT_TETRA1:
        {
            return (((nodePosition == 1 || nodePosition == 3 || nodePosition == 2) && edgePosition == 0)
                    ||
                    ((nodePosition == 0 || nodePosition == 2 || nodePosition == 3) && edgePosition == 1)
                    ||
                    ((nodePosition == 0 || nodePosition == 3 || nodePosition == 1) && edgePosition == 2)
                    ||
                    ((nodePosition == 0 || nodePosition == 1 || nodePosition == 2) && edgePosition == 3));
        }
        default:
        {
            return false;
        }
    }
} /* RElement::nodeIsOnEdge */


bool RElement::mergeNodes(uint nodeId1, uint nodeId2, bool allowDowngrade)
{
    bool nodeMerged = false;

    for (int i=int(this->size())-1;i>=0;i--)
    {
        if (this->getNodeId(uint(i)) == nodeId2)
        {
            this->setNodeId(uint(i),nodeId1);
            nodeMerged = true;
        }
    }
    if (!allowDowngrade || !nodeMerged)
    {
        return false;
    }

    std::vector<uint> nids;
    for (uint i=0;i<this->size();i++)
    {
        if (std::find(nids.begin(),nids.end(),this->getNodeId(i)) == nids.end())
        {
            nids.push_back(this->getNodeId(i));
        }
    }

    if (nids.size() == this->size())
    {
        return false;
    }

    switch (this->type)
    {
        case R_ELEMENT_BEAM1:
        case R_ELEMENT_TRUSS1:
        {
            this->setType(R_ELEMENT_POINT);
            this->nodeIDs.resize(1);
            this->setNodeId(0,nids[0]);
            return true;
        }
        case R_ELEMENT_TRI1:
        {
            this->setType(R_ELEMENT_TRUSS1);
            this->nodeIDs.resize(2);
            this->setNodeId(0,nids[0]);
            this->setNodeId(1,nids[1]);
            return true;
        }
        case R_ELEMENT_QUAD1:
        {
            this->setType(R_ELEMENT_TRI1);
            this->nodeIDs.resize(3);
            this->setNodeId(0,nids[0]);
            this->setNodeId(1,nids[1]);
            this->setNodeId(2,nids[2]);
            return true;
        }
        case R_ELEMENT_TETRA1:
        {
            this->setType(R_ELEMENT_TRI1);
            this->nodeIDs.resize(3);
            this->setNodeId(0,nids[0]);
            this->setNodeId(1,nids[1]);
            this->setNodeId(2,nids[2]);
            return true;
        }
        default:
        {
            return false;
        }
    }
} /* RElement::mergeNodes */


void RElement::breakWithNodes(const std::vector<RNode> &nodes, const std::vector<uint> breakNodeIDs, std::vector<RElement> &newElements)
{
    if (R_ELEMENT_TYPE_IS_SURFACE(this->getType()))
    {
        RRMatrix R;
        RRVector t;
        RRMatrix rotatedNodes;
        try
        {
            rotatedNodes = this->findTransformationMatrix(nodes,R,t);
            R.invert();
        }
        catch (const RError &rError)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Failed to find rotation matrix. %s",rError.getMessage().toUtf8().constData());
        }

        std::vector<RNode> tNodes;
        std::vector<uint> nodeBook;
        for (uint i=0;i<this->size();i++)
        {
            double x = rotatedNodes[i][0];
            double y = rotatedNodes[i][1];
            tNodes.push_back(RNode(x,y,0.0));
            nodeBook.push_back(this->getNodeId(i));
        }

        for (uint i=0;i<breakNodeIDs.size();i++)
        {
            tNodes.push_back(nodes[breakNodeIDs[i]]);
            RNode &rNode = tNodes.at(tNodes.size()-1);
            rNode.transform(R);
            rNode.setZ(0.0);
            nodeBook.push_back(breakNodeIDs[i]);
        }

        std::vector<RElement> elements = RTriangulateNodes(tNodes,true,RConstants::eps);

        bool firstAssigned = false;
        for (uint i=0;i<elements.size();i++)
        {
            if (!firstAssigned)
            {
                this->operator =(elements[i]);
                for (uint j=0;j<this->size();j++)
                {
                    this->setNodeId(j,nodeBook[this->getNodeId(j)]);
                }
                firstAssigned = true;
            }
            else
            {
                newElements.push_back(elements[i]);
                RElement &rElement = newElements.at(newElements.size()-1);
                for (uint j=0;j<rElement.size();j++)
                {
                    rElement.setNodeId(j,nodeBook[rElement.getNodeId(j)]);
                }
            }
        }
    }
    else
    {
        std::vector<RElement> tmpElements;
        tmpElements.push_back(*this);

        for (uint i=0;i<breakNodeIDs.size();i++)
        {
            for (uint j=0;j<tmpElements.size();j++)
            {
                tmpElements[j].breakWithNode(nodes,breakNodeIDs[i],tmpElements);
            }
        }

        if (tmpElements.size() > 0)
        {
            (*this) = tmpElements[0];
            tmpElements.erase(tmpElements.begin());
        }
        for (uint i=0;i<tmpElements.size();i++)
        {
            newElements.push_back(tmpElements[i]);
        }
    }
} /* RElement::breakWithPoints */


void RElement::breakWithNode(const std::vector<RNode> &nodes, uint breakNodeID, std::vector<RElement> &newElements)
{
    const RNode &node = nodes[breakNodeID];
    std::vector<RElement> tmpElements;

//    double tolerance = RConstants::eps;
//    double tolerance = 0.0;
    double tolerance = RConstants::findMachineDoubleEpsilon();

    switch (this->getType())
    {
        case R_ELEMENT_BEAM1:
        case R_ELEMENT_TRUSS1:
        {
            uint n1 = this->getNodeId(0);
            uint n2 = this->getNodeId(1);
            RSegment s(nodes[n1],nodes[n2]);
            if (s.isPointInside(node.toVector(),false))
            {
                RElement e1(R_ELEMENT_TRUSS1);
                e1.setNodeId(0,breakNodeID);
                e1.setNodeId(1,n2);
                tmpElements.push_back(e1);
                RElement e2(R_ELEMENT_TRUSS1);
                e2.setNodeId(0,n1);
                e2.setNodeId(1,breakNodeID);
                tmpElements.push_back(e2);
            }
            break;
        }
        case R_ELEMENT_TRI1:
        {
            uint n1 = this->getNodeId(0);
            uint n2 = this->getNodeId(1);
            uint n3 = this->getNodeId(2);
            RTriangle t(nodes[n1],nodes[n2],nodes[n3]);
            if (t.isPointInside(node.toVector(),false))
            {
                if (RTriangle::findArea(nodes[n1],nodes[n2],node) > tolerance)
                {
                    RElement e(R_ELEMENT_TRI1);
                    e.setNodeId(0,n1);
                    e.setNodeId(1,n2);
                    e.setNodeId(2,breakNodeID);
                    tmpElements.push_back(e);
                }
                if (RTriangle::findArea(nodes[n2],nodes[n3],node) > tolerance)
                {
                    RElement e(R_ELEMENT_TRI1);
                    e.setNodeId(0,n2);
                    e.setNodeId(1,n3);
                    e.setNodeId(2,breakNodeID);
                    tmpElements.push_back(e);
                }
                if (RTriangle::findArea(nodes[n3],nodes[n1],node) > tolerance)
                {
                    RElement e(R_ELEMENT_TRI1);
                    e.setNodeId(0,n3);
                    e.setNodeId(1,n1);
                    e.setNodeId(2,breakNodeID);
                    tmpElements.push_back(e);
                }
            }
            break;
        }
        case R_ELEMENT_QUAD1:
        {
            uint n1 = this->getNodeId(0);
            uint n2 = this->getNodeId(1);
            uint n3 = this->getNodeId(2);
            uint n4 = this->getNodeId(3);
            RTriangle t1(nodes[n1],nodes[n2],nodes[n3]);
            RTriangle t2(nodes[n1],nodes[n3],nodes[n4]);
            if (t1.isPointInside(node.toVector(),false) || t2.isPointInside(node.toVector(),false))
            {
                if (RTriangle::findArea(nodes[n1],nodes[n2],node) > tolerance)
                {
                    RElement e(R_ELEMENT_TRI1);
                    e.setNodeId(0,n1);
                    e.setNodeId(1,n2);
                    e.setNodeId(2,breakNodeID);
                    tmpElements.push_back(e);
                }
                if (RTriangle::findArea(nodes[n2],nodes[n3],node) > tolerance)
                {
                    RElement e(R_ELEMENT_TRI1);
                    e.setNodeId(0,n2);
                    e.setNodeId(1,n3);
                    e.setNodeId(2,breakNodeID);
                    tmpElements.push_back(e);
                }
                if (RTriangle::findArea(nodes[n3],nodes[n4],node) > tolerance)
                {
                    RElement e(R_ELEMENT_TRI1);
                    e.setNodeId(0,n3);
                    e.setNodeId(1,n4);
                    e.setNodeId(2,breakNodeID);
                    tmpElements.push_back(e);
                }
                if (RTriangle::findArea(nodes[n4],nodes[n1],node) > tolerance)
                {
                    RElement e(R_ELEMENT_TRI1);
                    e.setNodeId(0,n4);
                    e.setNodeId(1,n1);
                    e.setNodeId(2,breakNodeID);
                    tmpElements.push_back(e);
                }
            }
            break;
        }
        default:
        {
            // Unsupported element.
            break;
        }
    }

    if (tmpElements.size() > 0)
    {
        (*this) = tmpElements[0];
        tmpElements.erase(tmpElements.begin());
    }
    for (uint i=0;i<tmpElements.size();i++)
    {
        newElements.push_back(tmpElements[i]);
    }
} /* RElement::breakWithNode */


void RElement::findLimitBox(const std::vector<RNode> &nodes, RLimitBox &limitBox) const
{
    double xmin = 0.0, xmax = 0.0;
    double ymin = 0.0, ymax = 0.0;
    double zmin = 0.0, zmax = 0.0;

    for (unsigned int i=0;i<this->size();i++)
    {
        const RNode &rNode = nodes[this->getNodeId(i)];
        if (i == 0)
        {
            xmin = xmax = rNode.getX();
            ymin = ymax = rNode.getY();
            zmin = zmax = rNode.getZ();
        }
        else
        {
            xmin = std::min(xmin,rNode.getX());
            xmax = std::max(xmax,rNode.getX());

            ymin = std::min(ymin,rNode.getY());
            ymax = std::max(ymax,rNode.getY());

            zmin = std::min(zmin,rNode.getZ());
            zmax = std::max(zmax,rNode.getZ());
        }
    }

    limitBox.setLimits(xmin,xmax,ymin,ymax,zmin,zmax);
} /* RElement::findLimitBox */

void RElement::findScalarNodeValues(uint elementID, const RVariable &rVariable, std::vector<double> &values) const
{
    values.resize(this->size());

    double minValue = rVariable.getVariableData().getMinDisplayValue();
    double maxValue = rVariable.getVariableData().getMaxDisplayValue();

    if (rVariable.getApplyType() == R_VARIABLE_APPLY_NODE)
    {
        for (uint i=0;i<this->size();i++)
        {
            values[i] = rVariable.getValue(this->getNodeId(i));
        }
    }
    else if (rVariable.getApplyType() == R_VARIABLE_APPLY_ELEMENT)
    {
        for (uint i=0;i<this->size();i++)
        {
            values[i] = rVariable.getValue(elementID);
        }
    }

    for (uint i=0;i<values.size();i++)
    {
        values[i] = (values[i] - minValue) / (maxValue - minValue);

        values[i] = std::min(values[i],0.99);
        values[i] = std::max(values[i],0.01);
    }
} /* RElement::findScalarNodeValues */

void RElement::findDisplacementNodeValues(uint elementID, const RVariable &rVariable, std::vector<RR3Vector> &values) const
{
    values.resize(this->size());

    if (rVariable.getApplyType() == R_VARIABLE_APPLY_NODE)
    {
        for (uint i=0;i<this->size();i++)
        {
            values[i][0] = rVariable.getValue(0,this->getNodeId(i));
            values[i][1] = rVariable.getValue(1,this->getNodeId(i));
            values[i][2] = rVariable.getValue(2,this->getNodeId(i));
        }
    }
    else if (rVariable.getApplyType() == R_VARIABLE_APPLY_ELEMENT)
    {
        for (uint i=0;i<this->size();i++)
        {
            values[i][0] = rVariable.getValue(0,elementID);
            values[i][1] = rVariable.getValue(1,elementID);
            values[i][2] = rVariable.getValue(2,elementID);
        }
    }

    double scale = rVariable.getVariableData().getScale();

    for (uint i=0;i<values.size();i++)
    {
        values[i] *= scale;
    }
} /* RElement::findDisplacementNodeValues */

void RElement::print(const std::vector<RNode> &nodes) const
{
    RLogger::info("Name: %s\n",RElement::getName(this->getType()).toUtf8().constData());
    RLogger::info("Nodes:\n");
    for (unsigned int i=0;i<this->size();i++)
    {
        RLogger::info("  % 9u:",this->getNodeId(i));
        nodes[this->getNodeId(i)].print();
    }
} /* RElement::print */

void RElement::print(void) const
{
    RLogger::info("Name: %s\n",RElement::getName(this->getType()).toUtf8().constData());
    RLogger::info("Nodes:");
    for (unsigned int i=0;i<this->size();i++)
    {
        RLogger::info(" % 9u",this->getNodeId(i));
    }
    RLogger::info("\n");
} /* RElement::print */


RElementType RElement::getTypeFromId(const QString &elementId)
{
    for (uint type=uint(R_ELEMENT_NONE);type<uint(R_ELEMENT_N_TYPES);type++)
    {
        if (elementDesc[type].id == elementId)
        {
            return RElementType(type);
        }
    }

    return R_ELEMENT_NONE;
} /* RElement::getTypeFromId */


QString RElement::getId(RElementType type)
{
    R_ERROR_ASSERT (R_ELEMENT_TYPE_IS_VALID(type));

    return elementDesc[type].id;
} /* RElement::getId */


const QString &RElement::getName(RElementType type)
{
    R_ERROR_ASSERT (R_ELEMENT_TYPE_IS_VALID(type));

    return elementDesc[type].name;
} /* RElement::getName */


unsigned int RElement::getNNodes(RElementType type)
{
    R_ERROR_ASSERT (R_ELEMENT_TYPE_IS_VALID(type));

    return elementDesc[type].nNodes;
} /* RElement::getNNodes */


unsigned int RElement::getNNeighbors(RElementType type)
{
    R_ERROR_ASSERT (R_ELEMENT_TYPE_IS_VALID(type));

    return elementDesc[type].nNeighbors;
} /* RElement::getNNeighbors */


std::vector<RElementShapeFunction> RElement::generateShapeFunctionValues(RElementType type)
{
    R_ERROR_ASSERT (R_ELEMENT_TYPE_IS_VALID(type));

    std::vector<RElementShapeFunction> shapeFunctions;
    RRVector N;
    RRMatrix dN;
    double ksi, eta, zeta;

    switch (type)
    {
        case R_ELEMENT_POINT:
            break;
        case R_ELEMENT_TRUSS1:
            N.resize(2);
            dN.resize(2,1);
            // Derivated shape function
            dN[0][0] = -0.5;
            dN[1][0] =  0.5;
            // 1-st point
            ksi = -1.0/std::sqrt(3.0);
            N[0] = (1.0 - ksi)/2.0;
            N[1] = (1.0 + ksi)/2.0;
            shapeFunctions.push_back(RElementShapeFunction(N,dN,1.0));
            // 2-nd point
            ksi = 1.0/std::sqrt(3.0);
            N[0] = (1.0 - ksi)/2.0;
            N[1] = (1.0 + ksi)/2.0;
            shapeFunctions.push_back(RElementShapeFunction(N,dN,1.0));
            break;
        case R_ELEMENT_TRUSS2:
            break;
        case R_ELEMENT_BEAM1:
            break;
        case R_ELEMENT_BEAM2:
            break;
        case R_ELEMENT_TRI1:
            N.resize(3);
            dN.resize(3,2);
            // Derivated shape function
            dN[0][0] = -1.0;
            dN[1][0] =  1.0;
            dN[2][0] =  0.0;
            dN[0][1] = -1.0;
            dN[1][1] =  0.0;
            dN[2][1] =  1.0;
            // 1-st point
            ksi = 1.0/6.0;
            eta = 1.0/6.0;
            N[0] = 1.0 - ksi - eta;
            N[1] = ksi;
            N[2] = eta;
            shapeFunctions.push_back(RElementShapeFunction(N,dN,1.0/6.0));
            // 2-nd point
            ksi = 2.0/3.0;
            eta = 1.0/6.0;
            N[0] = 1.0 - ksi - eta;
            N[1] = ksi;
            N[2] = eta;
            shapeFunctions.push_back(RElementShapeFunction(N,dN,1.0/6.0));
            // 3-rd point
            ksi = 1.0/6.0;
            eta = 2.0/3.0;
            N[0] = 1.0 - ksi - eta;
            N[1] = ksi;
            N[2] = eta;
            shapeFunctions.push_back(RElementShapeFunction(N,dN,1.0/6.0));
            break;
        case R_ELEMENT_TRI2:
            break;
        case R_ELEMENT_QUAD1:
            N.resize(4);
            dN.resize(4,2);
            // 1-st point
            ksi = -1.0/std::sqrt(3.0);
            eta = -1.0/std::sqrt(3.0);
            N[0] = (1.0-ksi)*(1.0-eta)/4.0;
            N[1] = (1.0+ksi)*(1.0-eta)/4.0;
            N[2] = (1.0+ksi)*(1.0+eta)/4.0;
            N[3] = (1.0-ksi)*(1.0+eta)/4.0;
            dN[0][0] = (-1)*(1.0-eta)/4.0;
            dN[1][0] =      (1.0-eta)/4.0;
            dN[2][0] =      (1.0+eta)/4.0;
            dN[3][0] = (-1)*(1.0+eta)/4.0;
            dN[0][1] = (-1)*(1.0-ksi)/4.0;
            dN[1][1] = (-1)*(1.0+ksi)/4.0;
            dN[2][1] =      (1.0+ksi)/4.0;
            dN[3][1] =      (1.0-ksi)/4.0;
            shapeFunctions.push_back(RElementShapeFunction(N,dN,1.0));
            // 2-nd point
            ksi =  1.0/std::sqrt(3.0);
            eta = -1.0/std::sqrt(3.0);
            N[0] = (1.0-ksi)*(1.0-eta)/4.0;
            N[1] = (1.0+ksi)*(1.0-eta)/4.0;
            N[2] = (1.0+ksi)*(1.0+eta)/4.0;
            N[3] = (1.0-ksi)*(1.0+eta)/4.0;
            dN[0][0] = (-1)*(1.0-eta)/4.0;
            dN[1][0] =      (1.0-eta)/4.0;
            dN[2][0] =      (1.0+eta)/4.0;
            dN[3][0] = (-1)*(1.0+eta)/4.0;
            dN[0][1] = (-1)*(1.0-ksi)/4.0;
            dN[1][1] = (-1)*(1.0+ksi)/4.0;
            dN[2][1] =      (1.0+ksi)/4.0;
            dN[3][1] =      (1.0-ksi)/4.0;
            shapeFunctions.push_back(RElementShapeFunction(N,dN,1.0));
            // 3-rd point
            ksi = 1.0/std::sqrt(3.0);
            eta = 1.0/std::sqrt(3.0);
            N[0] = (1.0-ksi)*(1.0-eta)/4.0;
            N[1] = (1.0+ksi)*(1.0-eta)/4.0;
            N[2] = (1.0+ksi)*(1.0+eta)/4.0;
            N[3] = (1.0-ksi)*(1.0+eta)/4.0;
            dN[0][0] = (-1)*(1.0-eta)/4.0;
            dN[1][0] =      (1.0-eta)/4.0;
            dN[2][0] =      (1.0+eta)/4.0;
            dN[3][0] = (-1)*(1.0+eta)/4.0;
            dN[0][1] = (-1)*(1.0-ksi)/4.0;
            dN[1][1] = (-1)*(1.0+ksi)/4.0;
            dN[2][1] =      (1.0+ksi)/4.0;
            dN[3][1] =      (1.0-ksi)/4.0;
            shapeFunctions.push_back(RElementShapeFunction(N,dN,1.0));
            // 4-th point
            ksi = -1.0/std::sqrt(3.0);
            eta =  1.0/std::sqrt(3.0);
            N[0] = (1.0-ksi)*(1.0-eta)/4.0;
            N[1] = (1.0+ksi)*(1.0-eta)/4.0;
            N[2] = (1.0+ksi)*(1.0+eta)/4.0;
            N[3] = (1.0-ksi)*(1.0+eta)/4.0;
            dN[0][0] = (-1)*(1.0-eta)/4.0;
            dN[1][0] =      (1.0-eta)/4.0;
            dN[2][0] =      (1.0+eta)/4.0;
            dN[3][0] = (-1)*(1.0+eta)/4.0;
            dN[0][1] = (-1)*(1.0-ksi)/4.0;
            dN[1][1] = (-1)*(1.0+ksi)/4.0;
            dN[2][1] =      (1.0+ksi)/4.0;
            dN[3][1] =      (1.0-ksi)/4.0;
            shapeFunctions.push_back(RElementShapeFunction(N,dN,1.0));
            break;
        case R_ELEMENT_QUAD2:
            break;
        case R_ELEMENT_TETRA1:
            N.resize(4);
            dN.resize(4,3);
            // Derivated shape function
            dN[0][0] = -1.0;
            dN[1][0] =  1.0;
            dN[2][0] =  0.0;
            dN[3][0] =  0.0;
            dN[0][1] = -1.0;
            dN[1][1] =  0.0;
            dN[2][1] =  1.0;
            dN[3][1] =  0.0;
            dN[0][2] = -1.0;
            dN[1][2] =  0.0;
            dN[2][2] =  0.0;
            dN[3][2] =  1.0;
            // 1-st point
            ksi = 0.1381966;
            eta = 0.1381966;
            zeta = 0.1381966;
            N[0] = 1.0 - ksi - eta - zeta;
            N[1] = ksi;
            N[2] = eta;
            N[3] = zeta;
            shapeFunctions.push_back(RElementShapeFunction(N,dN,1.0/24.0));
            // 2-st point
            ksi = 0.5854102;
            eta = 0.1381966;
            zeta = 0.1381966;
            N[0] = 1.0 - ksi - eta - zeta;
            N[1] = ksi;
            N[2] = eta;
            N[3] = zeta;
            shapeFunctions.push_back(RElementShapeFunction(N,dN,1.0/24.0));
            // 3-st point
            ksi = 0.1381966;
            eta = 0.5854102;
            zeta = 0.1381966;
            N[0] = 1.0 - ksi - eta - zeta;
            N[1] = ksi;
            N[2] = eta;
            N[3] = zeta;
            shapeFunctions.push_back(RElementShapeFunction(N,dN,1.0/24.0));
            // 4-st point
            ksi = 0.1381966;
            eta = 0.1381966;
            zeta = 0.5854102;
            N[0] = 1.0 - ksi - eta - zeta;
            N[1] = ksi;
            N[2] = eta;
            N[3] = zeta;
            shapeFunctions.push_back(RElementShapeFunction(N,dN,1.0/24.0));
            break;
        case R_ELEMENT_WEDGE1:
            break;
        case R_ELEMENT_HEXA1:
            break;
        case R_ELEMENT_NONE:
        default:
            break;
    }

    return shapeFunctions;
} /* RElement::generateShapeFunctionValues */


RRMatrix RElement::generateMassMatrix(RElementType type)
{
    RRMatrix M;

    std::vector<RElementShapeFunction> shapeFunctions = RElement::generateShapeFunctionValues(type);
    unsigned int nn = RElement::getNNodes(type);
    M.resize(nn,nn);
    for (unsigned int i=0;i<shapeFunctions.size();i++)
    {
        const RRVector &N = shapeFunctions[i].getN();
        double w = shapeFunctions[i].getW();

        for (unsigned int k=0;k<nn;k++)
        {
            for (unsigned int l=0;l<nn;l++)
            {
                M[k][l] += N[k]*N[l]*w;
            }
        }
    }

    return M;
} /* RElement::generateMassMatrix */


RRVector RElement::generateMassVector(RElementType type)
{
    RRVector m;

    std::vector<RElementShapeFunction> shapeFunctions = RElement::generateShapeFunctionValues(type);
    unsigned int nn = RElement::getNNodes(type);
    m.resize(nn);
    for (unsigned int i=0;i<shapeFunctions.size();i++)
    {
        const RRVector &N = shapeFunctions[i].getN();
        double w = shapeFunctions[i].getW();

        for (unsigned int k=0;k<nn;k++)
        {
            m[k] += N[k]*w;
        }
    }

    return m;
} /* RElement::generateMassVector */

double RElement::generateTotalWeightFactor(RElementType type)
{
    double wt = 0.0;

    std::vector<RElementShapeFunction> shapeFunctions = RElement::generateShapeFunctionValues(type);
    for (unsigned int i=0;i<shapeFunctions.size();i++)
    {
        wt += shapeFunctions[i].getW();
    }

    return wt;
} /* RElement::generateTotalWeightFactor */


unsigned int RElement::getNIntegrationPoints(RElementType type)
{
    R_ERROR_ASSERT (R_ELEMENT_TYPE_IS_VALID(type));
    return static_cast<unsigned int>(elementDesc[type].shapeFunctions.size());
} /* RElement::getNIntegrationPoints */


const RElementShapeFunction &RElement::getShapeFunction(RElementType type, unsigned int point)
{
    R_ERROR_ASSERT (R_ELEMENT_TYPE_IS_VALID(type));
    R_ERROR_ASSERT (point < RElement::getNIntegrationPoints(type));
    return elementDesc[type].shapeFunctions[point];
} /* RElement::getShapeFunction */


const RRMatrix &RElement::getMassMatrix(RElementType type)
{
    R_ERROR_ASSERT (R_ELEMENT_TYPE_IS_VALID(type));
    return elementDesc[type].M;
} /* RElement::getMassMatrix */


const RRVector &RElement::getMassVector(RElementType type)
{
    R_ERROR_ASSERT (R_ELEMENT_TYPE_IS_VALID(type));
    return elementDesc[type].m;
} /* RElement::getMassVector */


double RElement::getTotalWeightFactor(RElementType type)
{
    R_ERROR_ASSERT (R_ELEMENT_TYPE_IS_VALID(type));
    return elementDesc[type].wt;
} /* RElement::getMassVector */


bool RElement::hasConstantDerivative(RElementType type)
{
    R_ERROR_ASSERT (R_ELEMENT_TYPE_IS_VALID(type));
    return elementDesc[type].constantDerivative;
} /* RElement::getMassVector */


bool RElement::findIntersectionPoints(const RElement &e1, const RElement &e2, const std::vector<RNode> &nodes, std::set<RR3Vector> &x)
{
    std::vector<RNode> p1;
    std::vector<RNode> p2;
    std::vector<RSegment> s1;
    std::vector<RSegment> s2;
    std::vector<RTriangle> t1;
    std::vector<RTriangle> t2;

    double nodeScale = 1.0;

    double xmin = 0.0, xmax = 0.0;
    double ymin = 0.0, ymax = 0.0;
    double zmin = 0.0, zmax = 0.0;

    for (uint i=0;i<e1.size();i++)
    {
        if (i==0)
        {
            xmin = xmax = nodes[e1.getNodeId(i)].getX();
            ymin = ymax = nodes[e1.getNodeId(i)].getY();
            zmin = zmax = nodes[e1.getNodeId(i)].getZ();
        }
        else
        {
            xmin = std::min(nodes[e1.getNodeId(i)].getX(),xmin);
            ymin = std::min(nodes[e1.getNodeId(i)].getY(),ymin);
            zmin = std::min(nodes[e1.getNodeId(i)].getZ(),zmin);

            xmax = std::max(nodes[e1.getNodeId(i)].getX(),xmax);
            ymax = std::max(nodes[e1.getNodeId(i)].getY(),ymax);
            zmax = std::max(nodes[e1.getNodeId(i)].getZ(),zmax);
        }
    }

    for (uint i=0;i<e2.size();i++)
    {
        xmin = std::min(nodes[e2.getNodeId(i)].getX(),xmin);
        ymin = std::min(nodes[e2.getNodeId(i)].getY(),ymin);
        zmin = std::min(nodes[e2.getNodeId(i)].getZ(),zmin);

        xmax = std::max(nodes[e2.getNodeId(i)].getX(),xmax);
        ymax = std::max(nodes[e2.getNodeId(i)].getY(),ymax);
        zmax = std::max(nodes[e2.getNodeId(i)].getZ(),zmax);
    }


    nodeScale = std::sqrt(std::pow(xmax - xmin,2) + std::pow(ymax - ymin,2) + std::pow(zmax - zmin,2));
    if (nodeScale <= RConstants::eps)
    {
        nodeScale = 1.0;
    }
    else
    {
        nodeScale = 1.0/nodeScale;
    }

    if (R_ELEMENT_TYPE_IS_POINT(e1.getType()))
    {
        p1.push_back(nodes[e1.getNodeId(0)]);
        p1.at(0).scale(nodeScale);
    }
    if (R_ELEMENT_TYPE_IS_POINT(e2.getType()))
    {
        p2.push_back(nodes[e2.getNodeId(0)]);
        p2.at(0).scale(nodeScale);
    }
    if (R_ELEMENT_TYPE_IS_LINE(e1.getType()))
    {
        s1 = e1.segmentize(nodes);
        for (uint i=0;i<s1.size();i++)
        {
            s1.at(i).getNode1().scale(nodeScale);
            s1.at(i).getNode2().scale(nodeScale);
        }
    }
    if (R_ELEMENT_TYPE_IS_LINE(e2.getType()))
    {
        s2 = e2.segmentize(nodes);
        for (uint i=0;i<s2.size();i++)
        {
            s2.at(i).getNode1().scale(nodeScale);
            s2.at(i).getNode2().scale(nodeScale);
        }
    }
    if (R_ELEMENT_TYPE_IS_SURFACE(e1.getType()))
    {
        t1 = e1.triangulate(nodes);
        for (uint i=0;i<t1.size();i++)
        {
            t1.at(i).getNode1().scale(nodeScale);
            t1.at(i).getNode2().scale(nodeScale);
            t1.at(i).getNode3().scale(nodeScale);
        }
    }
    if (R_ELEMENT_TYPE_IS_SURFACE(e2.getType()))
    {
        t2 = e2.triangulate(nodes);
        for (uint i=0;i<t2.size();i++)
        {
            t2.at(i).getNode1().scale(nodeScale);
            t2.at(i).getNode2().scale(nodeScale);
            t2.at(i).getNode3().scale(nodeScale);
        }
    }

    bool intersectionFound = false;

    std::set<RR3Vector> xTmp;

    // Segment-point intersection
    for (uint i=0;i<s1.size();i++)
    {
        for (uint j=0;j<p2.size();j++)
        {
            if (s1[i].findPointIntersection(p2[j],xTmp))
            {
                intersectionFound = true;
            }
        }
    }
    for (uint i=0;i<s2.size();i++)
    {
        for (uint j=0;j<p1.size();j++)
        {
            if (s2[i].findPointIntersection(p1[j],xTmp))
            {
                intersectionFound = true;
            }
        }
    }

    // Segment-segment intersection
    for (uint i=0;i<s1.size();i++)
    {
        for (uint j=0;j<s2.size();j++)
        {
            if (s1[i].findSegmentIntersection(s2[j],xTmp))
            {
                intersectionFound = true;
            }
        }
    }

    // Triangle-point intersection
    for (uint i=0;i<t1.size();i++)
    {
        for (uint j=0;j<p2.size();j++)
        {
            if (t1[i].findPointIntersection(p2[j],xTmp))
            {
                intersectionFound = true;
            }
        }
    }
    for (uint i=0;i<t2.size();i++)
    {
        for (uint j=0;j<p1.size();j++)
        {
            if (t2[i].findPointIntersection(p1[j],xTmp))
            {
                intersectionFound = true;
            }
        }
    }

    // Triangle-segment intersection
    for (uint i=0;i<t1.size();i++)
    {
        for (uint j=0;j<s2.size();j++)
        {
            if (t1[i].findSegmentIntersection(s2[j],xTmp))
            {
                intersectionFound = true;
            }
        }
    }
    for (uint i=0;i<t2.size();i++)
    {
        for (uint j=0;j<s1.size();j++)
        {
            if (t2[i].findSegmentIntersection(s1[j],xTmp))
            {
                intersectionFound = true;
            }
        }
    }

    // Triangle-triangle intersection
    for (uint i=0;i<t1.size();i++)
    {
        for (uint j=0;j<t2.size();j++)
        {
            if (t1[i].findTriangleIntersection(t2[j],xTmp))
            {
                intersectionFound = true;
            }
        }
    }

    for (auto f : xTmp)
    {
        f.scale(1.0/nodeScale);
        x.insert(f);
    }

    return intersectionFound;
} /* RElement::findIntersectionPoints */


bool RElement::isInsideBoundingBox(const std::vector<RNode> &nodes, const RNode &node) const
{
    double minX = 0.0, minY = 0.0, minZ = 0.0;
    double maxX = 0.0, maxY = 0.0, maxZ = 0.0;
    for (unsigned int i=0;i<this->size();i++)
    {
        const RNode &eNode = nodes[this->getNodeId(i)];
        if (i == 0)
        {
            minX = maxX = eNode.getX();
            minY = maxY = eNode.getY();
            minZ = maxZ = eNode.getZ();
        }
        else
        {
            minX = std::min(minX,eNode.getX());
            minY = std::min(minY,eNode.getY());
            minZ = std::min(minZ,eNode.getZ());

            maxX = std::max(maxX,eNode.getX());
            maxY = std::max(maxY,eNode.getY());
            maxZ = std::max(maxZ,eNode.getZ());
        }
    }
    return (node.getX() >= minX && node.getX() <= maxX
            &&
            node.getY() >= minY && node.getY() <= maxY
            &&
            node.getZ() >= minZ && node.getZ() <= maxZ);
} /* RElement::isInsideBoundingBox */


bool RElement::testPlaneIntersect(const RPlane &plane,
                                  const std::vector <RNode> &nodes) const
{
    double distance = 0.0;
    bool hasNegative = false;
    bool hasPositive = false;

    for (unsigned int i=0;i<this->size();i++)
    {
        distance = plane.findPointDistance(nodes[this->getNodeId(i)].toVector());
        if (std::abs(distance) < RConstants::eps)
        {
            return true;
        }
        if (distance < 0.0)
        {
            hasNegative = true;
        }
        else
        {
            hasPositive = true;
        }

        if (hasNegative && hasPositive)
        {
            return true;
        }
    }

    return false;
} /* RElement::testPlaneIntersect */

