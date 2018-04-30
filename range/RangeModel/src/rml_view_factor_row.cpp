/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_view_factor_row.cpp                                  *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   15-th July 2014                                          *
 *                                                                   *
 *  DESCRIPTION: View-Factor row class definition                    *
 *********************************************************************/

#include "rml_view_factor_row.h"

void RViewFactorRow::_init(const RViewFactorRow *pViewFactorRow)
{
    if (pViewFactorRow)
    {
        this->viewFactors = pViewFactorRow->viewFactors;
    }
}

RViewFactorRow::RViewFactorRow()
{
    this->_init();
}

RViewFactorRow::RViewFactorRow(const RViewFactorRow &viewFactorRow)
{
    this->_init(&viewFactorRow);
}

RViewFactorRow::~RViewFactorRow()
{

}

RViewFactorRow &RViewFactorRow::operator =(const RViewFactorRow &viewFactorRow)
{
    this->_init(&viewFactorRow);
    return (*this);
}

const RSparseVector<double> &RViewFactorRow::getViewFactors(void) const
{
    return this->viewFactors;
}

RSparseVector<double> &RViewFactorRow::getViewFactors(void)
{
    return this->viewFactors;
}
