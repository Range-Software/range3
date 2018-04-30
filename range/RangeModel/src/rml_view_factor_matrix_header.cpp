/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_view_factor_matrix_header.cpp                        *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   5-th August 2014                                         *
 *                                                                   *
 *  DESCRIPTION: View-Factor matrix header class definition          *
 *********************************************************************/

#include "rml_view_factor_matrix_header.h"
#include "rml_radiation_setup.h"


void RViewFactorMatrixHeader::_init(const RViewFactorMatrixHeader *pViewFactorMatrixHeader)
{
    if (pViewFactorMatrixHeader)
    {
        this->patchInput = pViewFactorMatrixHeader->patchInput;
        this->hemicubeResolution = pViewFactorMatrixHeader->hemicubeResolution;
        this->nElements = pViewFactorMatrixHeader->nElements;
    }
}

RViewFactorMatrixHeader::RViewFactorMatrixHeader()
    : hemicubeResolution(R_RADIATION_RESOLUTION_MEDIUM)
    , nElements(0)
{
    this->_init();
}

RViewFactorMatrixHeader::RViewFactorMatrixHeader(const RViewFactorMatrixHeader &viewFactorMatrixHeader)
{
    this->_init(&viewFactorMatrixHeader);
}

RViewFactorMatrixHeader::~RViewFactorMatrixHeader()
{

}

RViewFactorMatrixHeader &RViewFactorMatrixHeader::operator =(const RViewFactorMatrixHeader &viewFactorMatrixHeader)
{
    this->_init(&viewFactorMatrixHeader);
    return (*this);
}

bool RViewFactorMatrixHeader::operator ==(const RViewFactorMatrixHeader &viewFactorMatrixHeader)
{
    if (this->patchInput.size() != viewFactorMatrixHeader.patchInput.size())
    {
        return false;
    }
    for (unsigned int i=0;i<this->patchInput.size();i++)
    {
        if (this->patchInput[i] != viewFactorMatrixHeader.patchInput[i])
        {
            return false;
        }
    }
    if (this->hemicubeResolution != viewFactorMatrixHeader.hemicubeResolution)
    {
        return false;
    }
    if (this->nElements != viewFactorMatrixHeader.nElements)
    {
        return false;
    }
    return true;
}

bool RViewFactorMatrixHeader::operator !=(const RViewFactorMatrixHeader &viewFactorMatrixHeader)
{
    return !this->operator ==(viewFactorMatrixHeader);
}

const std::vector<RPatchInput> &RViewFactorMatrixHeader::getPatchInput(void) const
{
    return this->patchInput;
}

std::vector<RPatchInput> &RViewFactorMatrixHeader::getPatchInput(void)
{
    return this->patchInput;
}

unsigned int RViewFactorMatrixHeader::getHemicubeResolution(void) const
{
    return this->hemicubeResolution;
}

void RViewFactorMatrixHeader::setHemicubeResolution(unsigned int hemicubeResolution)
{
    this->hemicubeResolution = hemicubeResolution;
}

unsigned int RViewFactorMatrixHeader::getNElements(void) const
{
    return this->nElements;
}

void RViewFactorMatrixHeader::setNElements(unsigned int nElements)
{
    this->nElements = nElements;
}

void RViewFactorMatrixHeader::clear(void)
{
    this->hemicubeResolution = 0;
    this->patchInput.clear();
}
