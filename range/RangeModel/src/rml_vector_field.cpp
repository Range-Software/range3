/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_vector_field.cpp                                     *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   13-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Vector field class definition                       *
 *********************************************************************/

#include "rml_vector_field.h"

const QString RVectorField::defaultName("Vector field");

void RVectorField::_init(const RVectorField *pVectorField)
{
    if (pVectorField)
    {
        this->variableType = pVectorField->variableType;
        this->type3d = pVectorField->type3d;
        this->elementGroupIDs = pVectorField->elementGroupIDs;
    }
}

RVectorField::RVectorField()
    : variableType(R_VARIABLE_NONE)
    , type3d(true)
{
    this->name = RVectorField::defaultName;
    this->_init();
}

RVectorField::RVectorField(const RVectorField &vectorField) :
    REntityGroup(vectorField)
{
    this->_init(&vectorField);
}

RVectorField::~RVectorField()
{
}

RVectorField &RVectorField::operator =(const RVectorField &vectorField)
{
    this->REntityGroup::operator = (vectorField);
    this->_init(&vectorField);
    return (*this);
}

RVariableType RVectorField::getVariableType(void) const
{
    return this->variableType;
}

void RVectorField::setVariableType(RVariableType variableType)
{
    this->variableType = variableType;
}

bool RVectorField::getType3D(void) const
{
    return this->type3d;
}

void RVectorField::setType3D(bool type3d)
{
    this->type3d = type3d;
}

const std::vector<uint> &RVectorField::getElementGroupIDs(void) const
{
    return this->elementGroupIDs;
}

std::vector<uint> &RVectorField::getElementGroupIDs(void)
{
    return this->elementGroupIDs;
}

bool RVectorField::addElementGroupID(uint elementGroupID)
{
    if (std::find(this->elementGroupIDs.begin(),this->elementGroupIDs.end(),elementGroupID) != this->elementGroupIDs.end())
    {
        return false;
    }

    this->elementGroupIDs.push_back(elementGroupID);

    std::sort(this->elementGroupIDs.begin(),this->elementGroupIDs.end());

    return true;
}

bool RVectorField::removeElementGroupID(uint elementGroupID, bool decrement)
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

void RVectorField::clearElementGroupIDs(void)
{
    this->elementGroupIDs.clear();
}
