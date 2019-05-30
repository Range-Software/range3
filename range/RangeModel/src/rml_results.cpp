/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_results.cpp                                          *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   14-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Results class definition                            *
 *********************************************************************/

#include <string>
#include <vector>

#include "rml_results.h"


void RResults::_init (const RResults *pResults)
{
    this->nnodes = 0;
    this->nelements = 0;

    if (pResults)
    {
//        this->compTime = compTime;
        this->setNVariables (pResults->getNVariables());
        this->setNNodes (pResults->getNNodes());
        this->setNElements (pResults->getNElements());
        for (unsigned int i=0;i<this->getNVariables();i++)
        {
            this->setVariable(i, pResults->getVariable(i));
        }
    }
} /* RResults::_init */

RResults::RResults ()
//    : compTime(0.0)
{
    this->_init ();
} /* RResults::RResults */

RResults::RResults (const RResults &results)
{
    this->_init (&results);
} /* RResults::RResults (copy) */


RResults::~RResults ()
{
} /* RResults::~RResults */


RResults & RResults::operator = (const RResults &results)
{
    this->_init (&results);
    return (*this);
} /* RResults::operator = */


void RResults::clearResults(void)
{
    for (unsigned int i=0;i<this->variables.size();i++)
    {
        this->variables[i].clearValues();
    }
    this->variables.clear();
} /* RResults::clearResults */


//double RResults::getCompTime(void) const
//{
//    return this->compTime;
//} /* RResults::getCompTime */


//void RResults::setCompTime(double compTime)
//{
//    this->compTime = compTime;
//} /* RResults::setCompTime */


unsigned int RResults::getNVariables (void) const
{
    return uint(this->variables.size());
} /* RResults::getNVariables */


void RResults::setNVariables (unsigned int nvariables)
{
    this->variables.resize(nvariables);
} /* RResults::set_n_variables */


const RVariable & RResults::getVariable (unsigned int position) const
{
    R_ERROR_ASSERT (position < this->getNVariables());
    return this->variables[position];
} /* RResults::getVariable */


RVariable & RResults::getVariable (unsigned int position)
{
    R_ERROR_ASSERT (position < this->getNVariables());
    return this->variables[position];
} /* RResults::getVariable */


unsigned int RResults::findVariable(RVariableType variableType) const
{
    for (unsigned int i=0;i<this->getNVariables();i++)
    {
        if (this->getVariable(i).getType() == variableType)
        {
            return i;
        }
    }
    return RConstants::eod;
} /* RResults::findVariable */


//unsigned int RResults::findVariable(RVariableType variableType, bool create)
//{
//    for (unsigned int i=0;i<this->getNVariables();i++)
//    {
//        if (this->getVariable(i).getType() == variableType)
//        {
//            return i;
//        }
//    }
//    if (create)
//    {
//        this->addVariable(RVariable(variableType));
//        return this->getNVariables()-1;
//    }
//    return RConstants::eod;
//} /* RResults::findVariable */


unsigned int RResults::addVariable(const RVariable &variable)
{
    unsigned int variablePosition = this->findVariable(variable.getType());
    if (variablePosition == RConstants::eod)
    {
        this->variables.push_back(variable);
        variablePosition = uint(this->variables.size()-1);
    }
    else
    {
        this->setVariable(variablePosition,variable);
    }
    return variablePosition;
} /* RResults::addVariable */


void RResults::setVariable (unsigned int     position,
                            const RVariable &variable)
{
    R_ERROR_ASSERT (position < this->getNVariables());
    this->variables[position] = variable;
} /* RResults::setVariable */


void RResults::removeVariable(uint position)
{
    R_ERROR_ASSERT (position < this->getNVariables());
    this->variables.erase(this->variables.begin() + position);
} /* RResults::removeVariable */


void RResults::removeAllVariables(void)
{
    this->variables.clear();
} /* RResults::removeAllVariables */


unsigned int RResults::getNNodes (void) const
{
    return this->nnodes;
} /* RResults::getNNodes */


void RResults::setNNodes (unsigned int nnodes)
{
    std::vector<RVariable>::iterator iter;

    this->nnodes = nnodes;

    for (iter = this->variables.begin();
         iter != this->variables.end();
         ++iter)
    {
        if (iter->getApplyType() == R_VARIABLE_APPLY_NODE)
        {
            iter->resize (iter->getNVectors(), this->nnodes);
        }
    }
} /* RResults::setNNodes */


void RResults::addNode (double value)
{
    std::vector<RVariable>::iterator iter;

    for (iter = this->variables.begin();
         iter != this->variables.end();
         ++iter)
    {
        if (iter->getApplyType() == R_VARIABLE_APPLY_NODE)
        {
            iter->addValue (value);
        }
    }

    this->nnodes ++;
} /* RResults::addNode */


void RResults::removeNode (unsigned int position)
{
    std::vector<RVariable>::iterator iter;

    R_ERROR_ASSERT (position < this->getNNodes());

    for (iter = this->variables.begin();
         iter != this->variables.end();
         ++iter)
    {
        if (iter->getApplyType() == R_VARIABLE_APPLY_NODE)
        {
            iter->removeValue (position);
        }
    }

    this->nnodes --;
} /* RResults::removeNode */


unsigned int RResults::getNElements (void) const
{
    return this->nelements;
} /* RResults::getNElements */


void RResults::setNElements (unsigned int nelements)
{
    std::vector<RVariable>::iterator iter;

    this->nelements = nelements;

    for (iter = this->variables.begin();
         iter != this->variables.end();
         ++iter)
    {
        if (iter->getApplyType() == R_VARIABLE_APPLY_ELEMENT)
        {
            iter->resize (iter->getNVectors(), this->nelements);
        }
    }
} /* RResults::setNElements */


void RResults::addElement (double value)
{
    std::vector<RVariable>::iterator iter;

    for (iter = this->variables.begin();
         iter != this->variables.end();
         ++iter)
    {
        if (iter->getApplyType() == R_VARIABLE_APPLY_ELEMENT)
        {
            iter->addValue (value);
        }
    }

    this->nelements ++;
} /* RResults::addElement */


void RResults::removeElement (unsigned int position)
{
    std::vector<RVariable>::iterator iter;

    R_ERROR_ASSERT (position < this->getNElements());

    for (iter = this->variables.begin();
         iter != this->variables.end();
         ++iter)
    {
        if (iter->getApplyType() == R_VARIABLE_APPLY_ELEMENT)
        {
            iter->removeValue(position);
        }
    }

    this->nelements--;
}

void RResults::removeElements(const std::vector<uint> &elementBook)
{
    std::vector<RVariable>::iterator iter;

    for (iter = this->variables.begin();
         iter != this->variables.end();
         ++iter)
    {
        if (iter->getApplyType() == R_VARIABLE_APPLY_ELEMENT)
        {
            iter->removeValues(elementBook);
        }
    }

    this->nelements = 0;
    for (uint i=0;i<uint(elementBook.size());i++)
    {
        if (elementBook[i] != RConstants::eod)
        {
            this->nelements++;
        }
    }
} /* RResults::removeElements */
