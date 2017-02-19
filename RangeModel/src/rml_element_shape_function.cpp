/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_element_shape_function.cpp                           *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   12-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Element shape function class definition             *
 *********************************************************************/

#include "rml_element_shape_function.h"

void RElementShapeFunction::_init(const RElementShapeFunction *pElementShapeFunction)
{
    if (pElementShapeFunction)
    {
        this->N = pElementShapeFunction->N;
        this->dN = pElementShapeFunction->dN;
        this->w = pElementShapeFunction->w;
    }
}

RElementShapeFunction::RElementShapeFunction(const RRVector &N, const RRMatrix &dN, double w)
    : N(N)
    , dN(dN)
    , w(w)
{
    R_ERROR_ASSERT(N.size() == dN.getNRows());
    this->_init();
}

RElementShapeFunction::RElementShapeFunction(const RElementShapeFunction &elementShapeFunction)
{
    this->_init(&elementShapeFunction);
}

RElementShapeFunction::~RElementShapeFunction()
{
}

RElementShapeFunction &RElementShapeFunction::operator =(const RElementShapeFunction &elementShapeFunction)
{
    this->_init(&elementShapeFunction);
    return (*this);
}

unsigned int RElementShapeFunction::getNNodes(void) const
{
    return this->N.getNRows();
}

const RRVector &RElementShapeFunction::getN(void) const
{
    return this->N;
}

const RRMatrix &RElementShapeFunction::getDN(void) const
{
    return this->dN;
}

const double &RElementShapeFunction::getW(void) const
{
    return this->w;
}
