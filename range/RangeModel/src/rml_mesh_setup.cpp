/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_mesh_setup.cpp                                       *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   12-th October 2018                                       *
 *                                                                   *
 *  DESCRIPTION: Mesh setup class definition                         *
 *********************************************************************/

#include "rml_mesh_setup.h"

void RMeshSetup::_init(const RMeshSetup *pMeshSetup)
{
    if (pMeshSetup)
    {
        this->variables = pMeshSetup->variables;
        this->minEdgeLength = pMeshSetup->minEdgeLength;
        this->maxEdgeLength = pMeshSetup->maxEdgeLength;
    }
}

RMeshSetup::RMeshSetup()
    : minEdgeLength(0.0)
    , maxEdgeLength(0.0)
{
    this->_init();
}

RMeshSetup::RMeshSetup(const RMeshSetup &meshSetup)
{
    this->_init(&meshSetup);
}

RMeshSetup::~RMeshSetup()
{

}

RMeshSetup &RMeshSetup::operator =(const RMeshSetup &meshSetup)
{
    this->_init(&meshSetup);
    return (*this);
}

const QSet<RVariableType> &RMeshSetup::getVariables() const
{
    return this->variables;
}

QSet<RVariableType> &RMeshSetup::getVariables()
{
    return this->variables;
}

void RMeshSetup::setVariables(const QSet<RVariableType> &variables)
{
    this->variables = variables;
}

double RMeshSetup::getMinEdgeLength() const
{
    return this->minEdgeLength;
}

void RMeshSetup::setMinEdgeLength(double minEdgeLength)
{
    this->minEdgeLength = minEdgeLength;
}

double RMeshSetup::getMaxEdgeLength() const
{
    return this->maxEdgeLength;
}

void RMeshSetup::setMaxEdgeLength(double maxEdgeLength)
{
    this->maxEdgeLength = maxEdgeLength;
}
