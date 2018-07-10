/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_tetrahedron.cpp                                      *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   11-th June 2013                                          *
 *                                                                   *
 *  DESCRIPTION: Tetrahedron class definition                        *
 *********************************************************************/

#include <cmath>

#include "rml_tetrahedron.h"

void RTetrahedron::_init(const RTetrahedron *pTetrahedron)
{
    if (pTetrahedron)
    {
        this->node1 = pTetrahedron->node1;
        this->node2 = pTetrahedron->node2;
        this->node3 = pTetrahedron->node3;
        this->node4 = pTetrahedron->node4;
    }
}

RTetrahedron::RTetrahedron(const RNode &node1, const RNode &node2, const RNode &node3, const RNode &node4) :
    node1(node1),
    node2(node2),
    node3(node3),
    node4(node4)
{
    this->_init();
}

RTetrahedron::RTetrahedron(const RTetrahedron &tetrahedron)
{
    this->_init(&tetrahedron);
}

RTetrahedron::~RTetrahedron()
{
}

RTetrahedron &RTetrahedron::operator =(const RTetrahedron &tetrahedron)
{
    this->_init(&tetrahedron);
    return (*this);
}

void RTetrahedron::setNodes(const RNode &node1, const RNode &node2, const RNode &node3, const RNode &node4)
{
    this->node1 = node1;
    this->node2 = node2;
    this->node3 = node3;
    this->node4 = node4;
}

double RTetrahedron::findVolume(void) const
{
    return RTetrahedron::findVolume(this->node1.getX(),this->node1.getY(),this->node1.getZ(),
                                    this->node2.getX(),this->node2.getY(),this->node2.getZ(),
                                    this->node3.getX(),this->node3.getY(),this->node3.getZ(),
                                    this->node4.getX(),this->node4.getY(),this->node4.getZ());
}

double RTetrahedron::findVolume(const RNode &node1, const RNode &node2, const RNode &node3, const RNode &node4)
{
    return RTetrahedron::findVolume(node1.getX(),node1.getY(),node1.getZ(),
                                    node2.getX(),node2.getY(),node2.getZ(),
                                    node3.getX(),node3.getY(),node3.getZ(),
                                    node4.getX(),node4.getY(),node4.getZ());
}

double RTetrahedron::findVolume(double xi, double yi, double zi, double xj, double yj, double zj, double xk, double yk, double zk, double xl, double yl, double zl)
{
    RRMatrix mV(4,4);

    mV[0][0] = xi;
    mV[0][1] = yi;
    mV[0][2] = zi;
    mV[0][3] = 1.0;

    mV[1][0] = xj;
    mV[1][1] = yj;
    mV[1][2] = zj;
    mV[1][3] = 1.0;

    mV[2][0] = xk;
    mV[2][1] = yk;
    mV[2][2] = zk;
    mV[2][3] = 1.0;

    mV[3][0] = xl;
    mV[3][1] = yl;
    mV[3][2] = zl;
    mV[3][3] = 1.0;

    return std::abs(mV.getDeterminant() / 6.0);
}

