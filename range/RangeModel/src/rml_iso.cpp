/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_iso.cpp                                              *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   20-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: ISO class definition                                *
 *********************************************************************/

#include "rml_iso.h"

const QString RIso::defaultName("ISO");

void RIso::_init(const RIso *pIso)
{
    if (pIso)
    {
        this->variableType = pIso->variableType;
        this->variableValue = pIso->variableValue;
        this->elementGroupIDs = pIso->elementGroupIDs;
    }
}

RIso::RIso()
{
    this->name = RIso::defaultName;
    this->_init();
}

RIso::RIso(const RIso &iso)
    : RInterpolatedEntity(iso)
{
    this->_init(&iso);
}

RIso::~RIso()
{
}

RIso &RIso::operator =(const RIso &iso)
{
    this->RInterpolatedEntity::operator = (iso);
    this->_init(&iso);
    return (*this);
}

RVariableType RIso::getVariableType(void) const
{
    return this->variableType;
}

void RIso::setVariableType(RVariableType variableType)
{
    this->variableType = variableType;
}

double RIso::getVariableValue(void) const
{
    return this->variableValue;
}

void RIso::setVariableValue(double variableValue)
{
    this->variableValue = variableValue;
}

const std::vector<uint> &RIso::getElementGroupIDs(void) const
{
    return this->elementGroupIDs;
}

std::vector<uint> &RIso::getElementGroupIDs(void)
{
    return this->elementGroupIDs;
}

bool RIso::addElementGroupID(uint elementGroupID)
{
    if (std::find(this->elementGroupIDs.begin(),this->elementGroupIDs.end(),elementGroupID) != this->elementGroupIDs.end())
    {
        return false;
    }

    this->elementGroupIDs.push_back(elementGroupID);

    std::sort(this->elementGroupIDs.begin(),this->elementGroupIDs.end());

    return true;
}

bool RIso::removeElementGroupID(uint elementGroupID, bool decrement)
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

void RIso::clearElementGroupIDs(void)
{
    this->elementGroupIDs.clear();
}
