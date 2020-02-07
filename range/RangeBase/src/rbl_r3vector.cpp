/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_r3vector.cpp                                         *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   15-th February 2013                                      *
 *                                                                   *
 *  DESCRIPTION: Real vector[3] class declaration                    *
 *********************************************************************/

#include <cmath>

#include "rbl_utils.h"
#include "rbl_r3vector.h"
#include "rbl_error.h"

void RR3Vector::_init(const RR3Vector *pR3Vector)
{
    if (pR3Vector)
    {

    }
} /* RR3Vector::_init */


RR3Vector::RR3Vector()
{
    this->resize(3);
    this->_init();
} /* RR3Vector::RR3Vector */


RR3Vector::RR3Vector(const RR3Vector &r3Vector) : RRVector(r3Vector)
{
    this->_init(&r3Vector);
} /* RR3Vector::RR3Vector */


RR3Vector::RR3Vector(double x, double y, double z)
{
    this->resize(3);
    this->operator [](0) = x;
    this->operator [](1) = y;
    this->operator [](2) = z;
    this->_init();
} /* RR3Vector::RR3Vector */


RR3Vector::~RR3Vector()
{
} /* RR3Vector::~RR3Vector */


RR3Vector &RR3Vector::operator =(const RR3Vector &r3Vector)
{
    RRVector::operator =(r3Vector);
    this->_init(&r3Vector);
    return (*this);
} /* RR3Vector::operator = */

void RR3Vector::findRotationMatrix(RRMatrix &R) const
{
    R.setIdentity(3);

    RR3Vector lx(*this);
    RR3Vector ly;
    RR3Vector lz;

    lx.normalize();

    ly[0] = 1.0;
    ly[1] = 0.0;
    ly[2] = 0.0;

    if (std::fabs(RR3Vector::angle(lx,ly)) < RConstants::pi/10.0)
    {
        ly[0] = 0.0;
        ly[1] = 1.0;
        ly[2] = 0.0;
    }

    RR3Vector::cross(lx,ly,lz);

    if (std::fabs(lz.normalize()) < RConstants::eps)
    {
        lz[0] = 0.0;
        lz[1] = 0.0;
        lz[2] = 1.0;
    }

    RR3Vector::cross(lz,lx,ly);

    ly.normalize();

    R.resize(3,3);

    R[0][0] = lx[0];
    R[0][1] = ly[0];
    R[0][2] = lz[0];

    R[1][0] = lx[1];
    R[1][1] = ly[1];
    R[1][2] = lz[1];

    R[2][0] = lx[2];
    R[2][1] = ly[2];
    R[2][2] = lz[2];
} /* RR3Vector::findRotationMatrix */


RR3Vector RR3Vector::getOpposite() const
{
    return RR3Vector(-this->at(0),-this->at(1),-this->at(2));
} /* RR3Vector::getOpposite */


RR3Vector RR3Vector::findOrthogonal() const
{
    RR3Vector n(*this);
    n.normalize();

    RR3Vector r(1.0,0.0,0.0);

    double angle = RR3Vector::angle(n,r);
    if (angle < (RConstants::pi / 4.0) || angle > (RConstants::pi * 3.0 / 4.0))
    {
        std::swap(r[0],r[1]);
    }

    RR3Vector o;

    RR3Vector::cross(n,r,o);

    return o;
} /* RR3Vector::findOrthogonal */


void RR3Vector::clear()
{
    this->operator [](0) = 0.0;
    this->operator [](1) = 0.0;
    this->operator [](2) = 0.0;
} /* RR3Vector::clear */


double RR3Vector::findDistance(const RR3Vector &v1, const RR3Vector &v2)
{
    double dx = v1[0] - v2[0];
    double dy = v1[1] - v2[1];
    double dz = v1[2] - v2[2];

    return std::sqrt(dx*dx+dy*dy+dz*dz);
} /* RR3Vector::findDistance */


void RR3Vector::cross(const RR3Vector &v1, const RR3Vector &v2, RR3Vector &vp)
{
    R_ERROR_ASSERT(v1.size() == v2.size());

    vp.resize(v1.size());
    if (vp.size() == 3)
    {
        vp[0] = v1[1]*v2[2] - v1[2]*v2[1];
        vp[1] = v1[2]*v2[0] - v1[0]*v2[2];
        vp[2] = v1[0]*v2[1] - v1[1]*v2[0];
    }
} /* RR3Vector::cross */


bool RR3Vector::areParallel(const RR3Vector &v1, const RR3Vector &v2)
{
    R_ERROR_ASSERT(v1.size() == v2.size());

    RR3Vector vx;
    RR3Vector::cross(v1,v2,vx);

    for (uint i=0;i<vx.size();i++)
    {
        if (std::fabs(vx[i]) > RConstants::eps)
        {
            return false;
        }
    }
    return true;
} /* RR3Vector::findNParallelDimensions */

double RR3Vector::angle(const RR3Vector &v1, const RR3Vector &v2)
{
    R_ERROR_ASSERT(v1.size() == v2.size());

    double ln = v1.length() * v2.length();
    if (ln == 0.0)
    {
        return 0.0;
    }

    double dotValue = RRVector::dot(v1,v2)/ln;

    if (dotValue > 1.0)
    {
        dotValue = 1.0;
    }
    if (dotValue < -1.0)
    {
        dotValue = -1.0;
    }

    return acos(dotValue);
} /* RR3Vector::angle */
