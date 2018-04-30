/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_cut.cpp                                              *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   20-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Cut class definition                                *
 *********************************************************************/

#include "rml_cut.h"

const QString RCut::defaultName("Cut");

void RCut::_init(const RCut *pCut)
{
    if (pCut)
    {
        this->plane = pCut->plane;
        this->elementGroupIDs = pCut->elementGroupIDs;
    }
}

RCut::RCut()
{
    this->name = RCut::defaultName;
    this->_init();
}

RCut::RCut(const RCut &cut)
    : RInterpolatedEntity(cut)
{
    this->_init(&cut);
}

RCut::~RCut()
{
}

RCut &RCut::operator =(const RCut &cut)
{
    this->RInterpolatedEntity::operator = (cut);
    this->_init(&cut);
    return (*this);
}

const RPlane &RCut::getPlane(void) const
{
    return this->plane;
}

RPlane &RCut::getPlane(void)
{
    return this->plane;
}

void RCut::setPlane(const RPlane &plane)
{
    this->plane = plane;
}

const std::vector<uint> &RCut::getElementGroupIDs(void) const
{
    return this->elementGroupIDs;
}

std::vector<uint> &RCut::getElementGroupIDs(void)
{
    return this->elementGroupIDs;
}

bool RCut::addElementGroupID(uint elementGroupID)
{
    if (std::find(this->elementGroupIDs.begin(),this->elementGroupIDs.end(),elementGroupID) != this->elementGroupIDs.end())
    {
        return false;
    }

    this->elementGroupIDs.push_back(elementGroupID);

    std::sort(this->elementGroupIDs.begin(),this->elementGroupIDs.end());

    return true;
}

bool RCut::removeElementGroupID(uint elementGroupID, bool decrement)
{
    bool removed = false;
    std::vector<uint>::iterator iter = std::find(this->elementGroupIDs.begin(),this->elementGroupIDs.end(),elementGroupID);
    if (iter != this->elementGroupIDs.end())
    {
        this->elementGroupIDs.erase(iter);
        removed = true;
    }
    if (decrement)
    {
        for (uint i=0;i<this->elementGroupIDs.size();i++)
        {
            if (this->elementGroupIDs[i] > elementGroupID)
            {
                this->elementGroupIDs[i]--;
            }
        }
    }
    return removed;
}

void RCut::clearElementGroupIDs(void)
{
    this->elementGroupIDs.clear();
}
