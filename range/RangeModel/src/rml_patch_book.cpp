/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_patch_book.cpp                                       *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   5-th August 2014                                         *
 *                                                                   *
 *  DESCRIPTION: Patch book class definition                         *
 *********************************************************************/

#include "rml_patch_book.h"

void RPatchBook::_init(const RPatchBook *pPatchBook)
{
    if (pPatchBook)
    {
        this->patches = pPatchBook->patches;
        this->elementPatchMap = pPatchBook->elementPatchMap;
    }
}

RPatchBook::RPatchBook()
{
    this->_init();
}

RPatchBook::RPatchBook(const RPatchBook &patchBook)
{
    this->_init(&patchBook);
}

RPatchBook::~RPatchBook()
{

}

RPatchBook &RPatchBook::operator =(const RPatchBook &patchBook)
{
    this->_init(&patchBook);
    return (*this);
}

uint RPatchBook::getNPatches(void) const
{
    return (unsigned int)this->patches.size();
}

const RPatch &RPatchBook::getPatch(uint patchID) const
{
    R_ERROR_ASSERT(patchID < this->patches.size());

    return this->patches[patchID];
}

uint RPatchBook::findPatchID(uint elementID) const
{
    std::map<uint,uint>::const_iterator iter = this->elementPatchMap.find(elementID);
    if (iter == this->elementPatchMap.end())
    {
        return RConstants::eod;
    }
    return iter->second;
}

void RPatchBook::clear(void)
{
    this->patches.clear();
    this->elementPatchMap.clear();
}

uint RPatchBook::createNewPatch(uint surfaceID)
{
    this->patches.push_back(RPatch(surfaceID));
    return (unsigned int)this->patches.size() - 1;
}

void RPatchBook::registerElementID(uint patchID, uint elementID)
{
    R_ERROR_ASSERT(patchID < this->patches.size());

    this->patches[patchID].addElementID(elementID);
    this->elementPatchMap[elementID] = patchID;
}
