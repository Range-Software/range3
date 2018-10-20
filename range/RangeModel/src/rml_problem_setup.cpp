/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_problem_setup.cpp                                    *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   4-th July 2014                                           *
 *                                                                   *
 *  DESCRIPTION: Problem setup class definition                      *
 *********************************************************************/

#include "rml_problem_setup.h"

void RProblemSetup::_init(const RProblemSetup *pProblemSetup)
{
    if (pProblemSetup)
    {
        this->restart = pProblemSetup->restart;
        this->radiationSetup = pProblemSetup->radiationSetup;
        this->modalSetup = pProblemSetup->modalSetup;
        this->meshSetup = pProblemSetup->meshSetup;
    }
}

RProblemSetup::RProblemSetup()
    : restart(false)
{
    this->_init();
}

RProblemSetup::RProblemSetup(const RProblemSetup &problemSetup)
{
    this->_init(&problemSetup);
}

RProblemSetup::~RProblemSetup()
{

}

RProblemSetup &RProblemSetup::operator =(const RProblemSetup &problemSetup)
{
    this->_init(&problemSetup);
    return (*this);
}

bool RProblemSetup::getRestart(void) const
{
    return this->restart;
}

void RProblemSetup::setRestart(bool restart)
{
    this->restart = restart;
}

const RRadiationSetup &RProblemSetup::getRadiationSetup(void) const
{
    return this->radiationSetup;
}

RRadiationSetup &RProblemSetup::getRadiationSetup(void)
{
    return this->radiationSetup;
}

void RProblemSetup::setRadiationSetup(const RRadiationSetup &radiationSetup)
{
    this->radiationSetup = radiationSetup;
}

const RModalSetup &RProblemSetup::getModalSetup(void) const
{
    return this->modalSetup;
}

RModalSetup &RProblemSetup::getModalSetup(void)
{
    return this->modalSetup;
}

void RProblemSetup::setModalSetup(const RModalSetup &modalSetup)
{
    this->modalSetup = modalSetup;
}

const RMeshSetup &RProblemSetup::getMeshSetup() const
{
    return this->meshSetup;
}

RMeshSetup &RProblemSetup::getMeshSetup()
{
    return this->meshSetup;
}

void RProblemSetup::setMeshSetup(const RMeshSetup &meshSetup)
{
    this->meshSetup = meshSetup;
}
