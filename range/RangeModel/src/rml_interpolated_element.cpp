/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_interpolated_element.cpp                             *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   31-st May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Interpolated element class definition               *
 *********************************************************************/

#include <cmath>

#include "rml_interpolated_element.h"
#include "rml_polygon.h"
#include "rml_triangle.h"

void RInterpolatedElement::_init(const RInterpolatedElement *pInterpolatedElement)
{
    if (pInterpolatedElement)
    {

    }
}

RInterpolatedElement::RInterpolatedElement()
{
    this->_init();
}

RInterpolatedElement::RInterpolatedElement(const RInterpolatedElement &interpolatedElement)
    : std::vector<RInterpolatedNode>(interpolatedElement)
{
    this->_init(&interpolatedElement);
}

RInterpolatedElement::~RInterpolatedElement()
{
}

RInterpolatedElement &RInterpolatedElement::operator =(const RInterpolatedElement &interpolatedElement)
{
    this->std::vector<RInterpolatedNode>::operator = (interpolatedElement);
    this->_init(&interpolatedElement);
    return (*this);
}

void RInterpolatedElement::sortNodes(void)
{
    this->removeDuplicateNodes();

    if (this->size() < 3)
    {
        return;
    }

    if (this->size() == 4)
    {
        RTriangle t1(this->operator [](0),
                     this->operator [](1),
                     this->operator [](2));
        RTriangle t2(this->operator [](0),
                     this->operator [](2),
                     this->operator [](3));

        double angleNormal = std::fabs(RR3Vector::angle(t1.getNormal(),t2.getNormal()));
        if (angleNormal > RConstants::pi/2.0 && angleNormal < 3.0*RConstants::pi/2.0)
        {
            RInterpolatedNode nTmp = this->operator [](2);
            this->operator [](2) = this->operator [](3);
            this->operator [](3) = nTmp;
        }
        return;
    }

    RR3Vector c(0.0,0.0,0.0);

    for (unsigned int i=0;i<this->size();i++)
    {
        c[0] += this->operator [](i).getX();
        c[1] += this->operator [](i).getY();
        c[2] += this->operator [](i).getZ();
    }

    if (this->size() > 1)
    {
        c[0] /= double(this->size());
        c[1] /= double(this->size());
        c[2] /= double(this->size());
    }

    RTriangle triangle(this->operator [](0),
                       this->operator [](1),
                       RNode(c[0],c[1],c[2]));

    const RR3Vector &normal = triangle.getNormal();

    RR3Vector v1(this->operator [](0).getX() - c[0],
                 this->operator [](0).getY() - c[1],
                 this->operator [](0).getZ() - c[2]);

    std::map<double,RInterpolatedNode> nodeBook;

    nodeBook[0.0] = this->operator [](0);

    RR3Vector vn;
    for (unsigned int i=1;i<this->size();i++)
    {
        RR3Vector v2(this->operator [](i).getX() - c[0],
                     this->operator [](i).getY() - c[1],
                     this->operator [](i).getZ() - c[2]);

        double angle = std::fabs(RR3Vector::angle(v1,v2));

        if (angle < RConstants::pi)
        {
            RR3Vector::cross(v1,v2,vn);

            double angleNormal = std::fabs(RR3Vector::angle(normal,vn));
            if (angleNormal > RConstants::pi/2.0 && angleNormal < 3.0*RConstants::pi/2.0)
            {
                angle = 2.0 * RConstants::pi - angle;
            }
        }

        nodeBook[angle] = this->operator [](i);
    }

    this->clear();

    for (std::map<double,RInterpolatedNode>::iterator iter = nodeBook.begin(); iter != nodeBook.end(); iter++)
    {
        this->push_back(iter->second);
    }
}

void RInterpolatedElement::removeDuplicateNodes(void)
{
    RInterpolatedElement nodes;

    for (unsigned int i=0;i<this->size();i++)
    {
        bool duplicateFound = false;
        for (unsigned int j=i+1;j<this->size();j++)
        {
            if (std::fabs(this->operator [](i).getDistance(this->operator [](j))) < RConstants::eps)
            {
                duplicateFound = true;
            }
        }
        if (!duplicateFound)
        {
            nodes.push_back(this->operator [](i));
        }
    }
    this->operator =(nodes);
}

void RInterpolatedElement::findCenter(double &cx, double &cy, double &cz) const
{
    cx = cy = cz = 0.0;
    for (unsigned int i=0;i<this->size();i++)
    {
        cx += this->at(i).getX();
        cy += this->at(i).getY();
        cz += this->at(i).getZ();
    }
    if (this->size() != 0)
    {
        cx /= double(this->size());
        cy /= double(this->size());
        cz /= double(this->size());
    }
}

bool RInterpolatedElement::findNormal(double &nx, double &ny, double &nz) const
{
    if (this->size() < 3)
    {
        return false;
    }
    std::vector<RR3Vector> nodes;
    for (unsigned int i=0;i<this->size();i++)
    {
        nodes.push_back(RR3Vector(this->at(i).getX(),this->at(i).getY(),this->at(i).getZ()));
    }
    RPolygon p(nodes);
    nx = p.getNormal()[0];
    ny = p.getNormal()[1];
    nz = p.getNormal()[2];

    return true;
}

bool RInterpolatedElement::findPickDistance(const RR3Vector &position, const RR3Vector &direction, double tolerance, double &distance) const
{
    bool found = false;

    if (this->size() == 1)
    {
        RSegment ray(RNode(position),RNode(position[0]+direction[0],position[1]+direction[1],position[2]+direction[2]));
        double u = ray.findPointDistance(this->at(0).toVector());
        if (u <= tolerance)
        {
            distance = this->at(0).getDistance(position);
            found = true;
        }
    }
    else if (this->size() == 2)
    {
        std::vector<RNode> nodes(this->size());
        for (uint i=0;i<this->size();i++)
        {
            nodes[i] = this->at(i);
        }
        RElement line(R_ELEMENT_TRUSS1);
        line.setNodeId(0,0);
        line.setNodeId(1,1);
        double u;
        if (line.findPickDistance(nodes,position,direction,tolerance,u))
        {
            distance = u;
            found = true;
        }
    }
    else if (this->size() >= 3)
    {
        std::vector<RNode> nodes(this->size());
        for (uint i=0;i<this->size();i++)
        {
            nodes[i] = this->at(i);
        }

        std::vector<RElement> triangles = RPolygon::triangulate(nodes,true);

        for (uint i=0;i<triangles.size();i++)
        {
            const RElement &rElement = triangles[i];
            double u;
            if (rElement.findPickDistance(nodes,position,direction,tolerance,u))
            {
                if (!found || u < distance)
                {
                    distance = u;
                    found = true;
                }
            }
        }
    }

    return found;
}

void RInterpolatedElement::findScalarNodeValues(const std::vector<RNode> &rNodes,
                                                const std::vector<RElement> &rElements,
                                                const RVariable &rVariable,
                                                std::vector<double> &values) const
{
    values.resize(this->size());

    double minValue = rVariable.getVariableData().getMinDisplayValue();
    double maxValue = rVariable.getVariableData().getMaxDisplayValue();

    if (rVariable.getApplyType() == R_VARIABLE_APPLY_NODE)
    {
        for (uint i=0;i<this->size();i++)
        {
            const RElement &element = rElements[this->at(i).getElementID()];
            std::vector<double> nodeValues;
            nodeValues.resize(element.size(),0.0);
            for (uint j=0;j<element.size();j++)
            {
                nodeValues[j] = rVariable.getValue(element.getNodeId(j));
            }
            values[i] = element.interpolate(rNodes,this->at(i),nodeValues);
        }
    }
    else if (rVariable.getApplyType() == R_VARIABLE_APPLY_ELEMENT)
    {
        for (uint i=0;i<this->size();i++)
        {
            values[i] = rVariable.getValue(this->at(i).getElementID());
        }
    }

    for (uint i=0;i<values.size();i++)
    {
        values[i] = (values[i] - minValue) / (maxValue - minValue);

        values[i] = std::min(values[i],0.99);
        values[i] = std::max(values[i],0.01);
    }
}

void RInterpolatedElement::findDisplacementNodeValues(const std::vector<RNode> &rNodes,
                                                      const std::vector<RElement> &rElements,
                                                      const RVariable &rVariable,
                                                      std::vector<RR3Vector> &values) const
{
    values.resize(this->size());

    if (rVariable.getApplyType() == R_VARIABLE_APPLY_NODE)
    {
        for (uint i=0;i<this->size();i++)
        {
            const RElement &element = rElements[this->at(i).getElementID()];
            std::vector<double> xNodeValues;
            std::vector<double> yNodeValues;
            std::vector<double> zNodeValues;
            xNodeValues.resize(element.size(),0.0);
            yNodeValues.resize(element.size(),0.0);
            zNodeValues.resize(element.size(),0.0);
            for (uint j=0;j<element.size();j++)
            {
                xNodeValues[j] = rVariable.getValue(0,element.getNodeId(j));
                yNodeValues[j] = rVariable.getValue(1,element.getNodeId(j));
                zNodeValues[j] = rVariable.getValue(2,element.getNodeId(j));
            }
            values[i][0] = element.interpolate(rNodes,this->at(i),xNodeValues);
            values[i][1] = element.interpolate(rNodes,this->at(i),yNodeValues);
            values[i][2] = element.interpolate(rNodes,this->at(i),zNodeValues);
        }
    }
    else if (rVariable.getApplyType() == R_VARIABLE_APPLY_ELEMENT)
    {
        for (uint i=0;i<this->size();i++)
        {
            values[i][0] = rVariable.getValue(0,this->at(i).getElementID());
            values[i][1] = rVariable.getValue(1,this->at(i).getElementID());
            values[i][2] = rVariable.getValue(2,this->at(i).getElementID());
        }
    }

    double scale = rVariable.getVariableData().getScale();

    for (uint i=0;i<values.size();i++)
    {
        values[i] *= scale;
    }
}
