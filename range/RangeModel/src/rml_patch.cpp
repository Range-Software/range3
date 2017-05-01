/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_patch.cpp                                            *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   5-th August 2014                                         *
 *                                                                   *
 *  DESCRIPTION: Patch class definition                              *
 *********************************************************************/

#include <algorithm>

#include "rml_patch.h"

void RPatch::_init(const RPatch *pPatch)
{
    if (pPatch)
    {
        this->surfaceID = pPatch->surfaceID;
        this->elementIDs = pPatch->elementIDs;
    }
}

RPatch::RPatch(uint surfaceID)
    : surfaceID(surfaceID)
{
    this->_init();
}

RPatch::RPatch(const RPatch &patch)
{
    this->_init(&patch);
}

RPatch::~RPatch()
{

}

RPatch &RPatch::operator =(const RPatch &patch)
{
    this->_init(&patch);
    return (*this);
}

uint RPatch::getSurfaceID(void) const
{
    return this->surfaceID;
}

const RUVector &RPatch::getElementIDs(void) const
{
    return this->elementIDs;
}

bool RPatch::addElementID(uint elementID)
{
    if (this->hasElementID(elementID))
    {
        return false;
    }
    this->elementIDs.push_back(elementID);
    std::sort(this->elementIDs.begin(),this->elementIDs.end());
    return false;
}

bool RPatch::removeElementID(uint elementID)
{
    std::vector<unsigned int>::iterator iter = std::find(this->elementIDs.begin(),this->elementIDs.end(),elementID);
    if (iter == this->elementIDs.end())
    {
        return false;
    }
    this->elementIDs.erase(iter);
    return true;
}

bool RPatch::hasElementID(uint elementID) const
{
    return std::binary_search(this->elementIDs.begin(),this->elementIDs.end(),elementID);
}
