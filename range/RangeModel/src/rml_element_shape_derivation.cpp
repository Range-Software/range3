/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_element_shape_derivation.cpp                         *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   16-th December 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Element shape derivation class definition           *
 *********************************************************************/

#include "rml_element_shape_derivation.h"


void RElementShapeDerivation::_init(const RElementShapeDerivation *pElementShapeDerivation)
{
    if (pElementShapeDerivation)
    {
        this->B = pElementShapeDerivation->B;
        this->detJ = pElementShapeDerivation->detJ;
    }
}

RElementShapeDerivation::RElementShapeDerivation(const RElement &rElement, const std::vector<RNode> &nodes, RProblemType problemType)
{
    this->_init();

    switch (problemType)
    {
        case R_PROBLEM_FLUID:
        {
            this->generateFluid(rElement,nodes);
            break;
        }
        default:
        {
            break;
        }
    }
}

RElementShapeDerivation::RElementShapeDerivation(const RElementShapeDerivation &elementShapeDerivation)
{
    this->_init(&elementShapeDerivation);
}

RElementShapeDerivation::~RElementShapeDerivation()
{

}

RElementShapeDerivation &RElementShapeDerivation::operator =(const RElementShapeDerivation &elementShapeDerivation)
{
    this->_init(&elementShapeDerivation);
    return (*this);
}

const RRMatrix &RElementShapeDerivation::getDerivative(uint integrationPoint) const
{
    return this->B[integrationPoint];
}

double RElementShapeDerivation::getJacobian(uint integrationPoint) const
{
    return this->detJ[integrationPoint];
}

void RElementShapeDerivation::generateFluid(const RElement &rElement, const std::vector<RNode> &nodes)
{
    uint nen = rElement.size();
    uint nInp = RElement::getNIntegrationPoints(rElement.getType());

    this->detJ.resize(nInp);
    this->B.resize(nInp);

    for (uint intPoint=0;intPoint<nInp;intPoint++)
    {
        const RElementShapeFunction &shapeFunc = RElement::getShapeFunction(rElement.getType(),intPoint);
        const RRMatrix &dN = shapeFunc.getDN();
        RRMatrix J, Rt;

        this->detJ[intPoint] = rElement.findJacobian(nodes,intPoint,J,Rt);

        if (R_ELEMENT_TYPE_IS_VOLUME(rElement.getType()))
        {
            this->B[intPoint].resize(nen,3,0.0);

            for (uint m=0;m<dN.getNRows();m++)
            {
                this->B[intPoint][m][0] += (dN[m][0]*J[0][0] + dN[m][1]*J[0][1] + dN[m][2]*J[0][2]);
                this->B[intPoint][m][1] += (dN[m][0]*J[1][0] + dN[m][1]*J[1][1] + dN[m][2]*J[1][2]);
                this->B[intPoint][m][2] += (dN[m][0]*J[2][0] + dN[m][1]*J[2][1] + dN[m][2]*J[2][2]);
            }
        }
    }
}
