/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rlocalrotation.cpp                                       *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   20-th January 2016                                       *
 *                                                                   *
 *  DESCRIPTION: Local rotation class definition                     *
 *********************************************************************/

#include "rlocalrotation.h"

void RLocalRotation::_init(const RLocalRotation *pLocalRotation)
{
    if (pLocalRotation)
    {
        this->active = pLocalRotation->active;
        this->R = pLocalRotation->R;
        this->iR = pLocalRotation->iR;
    }
}

RLocalRotation::RLocalRotation()
    : active(false)
{
    this->_init();
}

RLocalRotation::RLocalRotation(const RLocalRotation &localRotation)
{
    this->_init(&localRotation);
}

RLocalRotation::~RLocalRotation()
{

}

RLocalRotation &RLocalRotation::operator =(const RLocalRotation &localRotation)
{
    this->_init(&localRotation);
    return (*this);
}

bool RLocalRotation::isActive(void) const
{
    return this->active;
}

void RLocalRotation::activate(const RRMatrix &matrix)
{
    R_ERROR_ASSERT(matrix.getNRows() == matrix.getNColumns() && matrix.getNRows() == 3);

    this->active = true;
    this->R = matrix;
    this->iR = matrix;
    this->iR.invert();
}

void RLocalRotation::deactivate(void)
{
    this->active = false;
    this->R.clear();
    this->iR.clear();
}

void RLocalRotation::rotateResultsVector(RRVector &v) const
{
    R_ERROR_ASSERT(v.getNRows() % 3 == 0);

    RR3Vector v3;

    uint nRotations = v.getNRows() / 3;

    for (uint i=0;i<nRotations;i++)
    {
        v3[0] = v[3*i+0];
        v3[1] = v[3*i+1];
        v3[2] = v[3*i+2];

        v[3*i+0] = this->iR[0][0] * v3[0] + this->iR[1][0] * v3[1] + this->iR[2][0] * v3[2];
        v[3*i+1] = this->iR[0][1] * v3[0] + this->iR[1][1] * v3[1] + this->iR[2][1] * v3[2];
        v[3*i+2] = this->iR[0][2] * v3[0] + this->iR[1][2] * v3[1] + this->iR[2][2] * v3[2];
    }
}
