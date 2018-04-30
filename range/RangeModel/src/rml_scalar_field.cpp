/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_scalar_field.cpp                                     *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   13-th February 2014                                      *
 *                                                                   *
 *  DESCRIPTION: Scalar field class definition                       *
 *********************************************************************/

#include "rml_scalar_field.h"

const QString RScalarField::defaultName("Scalar field");

void RScalarField::_init(const RScalarField *pScalarField)
{
    if (pScalarField)
    {
        this->minPointSize = pScalarField->minPointSize;
        this->maxPointSize = pScalarField->maxPointSize;
    }
}

RScalarField::RScalarField()
    : minPointSize(0.0)
    , maxPointSize(RScalarField::getDefaultMaxPointSize())
{
    this->name = RScalarField::defaultName;
    this->_init();
}

RScalarField::RScalarField(const RScalarField &scalarField)
    : RVectorField(scalarField)
{
    this->_init(&scalarField);
}

RScalarField::~RScalarField()
{

}

RScalarField &RScalarField::operator =(const RScalarField &scalarField)
{
    this->RVectorField::operator =(scalarField);
    this->_init(&scalarField);
    return (*this);
}

double RScalarField::getMaxPointSize(void) const
{
    return this->maxPointSize;
}

void RScalarField::setMaxPointSize(double maxPointSize)
{
    this->maxPointSize = maxPointSize;
}

double RScalarField::getDefaultMaxPointSize(void)
{
    return 10.0;
}
