/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_problem_task_item.cpp                                *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   4-th November 2015                                       *
 *                                                                   *
 *  DESCRIPTION: Problem task item class definition                  *
 *********************************************************************/

#include <list>

#include <rblib.h>

#include "rml_problem.h"

void RProblemTaskItem::_init(const RProblemTaskItem *pSolverTaskItem)
{
    if (pSolverTaskItem)
    {
        this->problemType = pSolverTaskItem->problemType;
        this->nIterations = pSolverTaskItem->nIterations;
        this->children = pSolverTaskItem->children;
    }
}

RProblemTaskItem::RProblemTaskItem(RProblemType problemType)
    : problemType(problemType)
    , nIterations(1)
{
    this->_init();
}

RProblemTaskItem::RProblemTaskItem(const RProblemTaskItem &solverTaskItem)
{
    this->_init(&solverTaskItem);
}

RProblemTaskItem::~RProblemTaskItem()
{

}

RProblemTaskItem &RProblemTaskItem::operator =(const RProblemTaskItem &solverTaskItem)
{
    this->_init(&solverTaskItem);
    return (*this);
}

RProblemTypeMask RProblemTaskItem::getProblemTypeMask(void) const
{
    RProblemTypeMask problemTypeMask = this->getProblemType();

    for (uint i=0;i<this->getNChildren();i++)
    {
        problemTypeMask |= this->getChild(i).getProblemTypeMask();
    }

    return problemTypeMask;
}

RProblemType RProblemTaskItem::getProblemType(void) const
{
    return this->problemType;
}

void RProblemTaskItem::setProblemType(RProblemType problemType)
{
    this->problemType = problemType;
    if (this->problemType != R_PROBLEM_NONE)
    {
        this->nIterations = 0;
        this->children.resize(0);
    }
}

unsigned int RProblemTaskItem::getNIterations(void) const
{
    return this->nIterations;
}

void RProblemTaskItem::setNIterations(unsigned int nIterations)
{
    this->nIterations = nIterations;
}

unsigned int RProblemTaskItem::getNChildren(void) const
{
    return (unsigned int)this->children.size();
}

const RProblemTaskItem &RProblemTaskItem::getChild(unsigned int position) const
{
    return this->children.at(position);
}

RProblemTaskItem &RProblemTaskItem::getChild(unsigned int position)
{
    return this->children.at(position);
}

void RProblemTaskItem::addChild(const RProblemTaskItem &solverTaskItem)
{
    this->children.push_back(solverTaskItem);
}

void RProblemTaskItem::insertChild(unsigned int position, const RProblemTaskItem &solverTaskItem)
{
    this->children.insert(this->children.begin()+position,1,solverTaskItem);
}

void RProblemTaskItem::removeChild(unsigned int position)
{
    this->children.erase(this->children.begin()+position);
}

void RProblemTaskItem::print(bool printTitle) const
{
    if (printTitle)
    {
        RLogger::info("Task-tree to be executed:\n");
        RLogger::indent();
    }
    if (this->getProblemType() == R_PROBLEM_NONE)
    {
        RLogger::info("Number of iterations: %u\n",this->getNIterations());
        RLogger::indent();
        for (unsigned int i=0;i<this->getNChildren();i++)
        {
            this->getChild(i).print(false);
        }
        RLogger::unindent(false);
    }
    else
    {
        RLogger::info("Problem type: %s\n",RProblem::getName(this->getProblemType()).toUtf8().constData());
    }
    if (printTitle)
    {
        RLogger::unindent(false);
    }
}

RProblemTaskItem RProblemTaskItem::generateDefaultTree(RProblemTypeMask problemTypeMask)
{
    RProblemTaskItem root;
    unsigned int nTypes = 0;
    unsigned int nNonlinearIterations = 5;

    bool isNonlinear = false;

    if (problemTypeMask & R_PROBLEM_RADIATIVE_HEAT && problemTypeMask & R_PROBLEM_HEAT)
    {
        isNonlinear = true;
    }

    if (problemTypeMask & R_PROBLEM_RADIATIVE_HEAT)
    {
        root.addChild(RProblemTaskItem(R_PROBLEM_RADIATIVE_HEAT));
        nTypes ++;
    }
    if (problemTypeMask & R_PROBLEM_HEAT)
    {
        root.addChild(RProblemTaskItem(R_PROBLEM_HEAT));
        nTypes ++;
    }
    if (problemTypeMask & R_PROBLEM_ACOUSTICS)
    {
        root.addChild(RProblemTaskItem(R_PROBLEM_ACOUSTICS));
        nTypes ++;
    }
    if (problemTypeMask & R_PROBLEM_FLUID_PARTICLE)
    {
        root.addChild(RProblemTaskItem(R_PROBLEM_FLUID_PARTICLE));
        nTypes ++;
    }
    if (problemTypeMask & R_PROBLEM_FLUID)
    {
        root.addChild(RProblemTaskItem(R_PROBLEM_FLUID));
        nTypes ++;
    }
    if (problemTypeMask & R_PROBLEM_ELECTROSTATICS)
    {
        root.addChild(RProblemTaskItem(R_PROBLEM_ELECTROSTATICS));
        nTypes ++;
    }
    if (problemTypeMask & R_PROBLEM_MAGNETOSTATICS)
    {
        root.addChild(RProblemTaskItem(R_PROBLEM_MAGNETOSTATICS));
        nTypes ++;
    }
    if (problemTypeMask & R_PROBLEM_STRESS)
    {
        root.addChild(RProblemTaskItem(R_PROBLEM_STRESS));
        nTypes ++;
    }
    if (problemTypeMask & R_PROBLEM_STRESS_MODAL)
    {
        root.addChild(RProblemTaskItem(R_PROBLEM_STRESS_MODAL));
        nTypes ++;
    }
    if (problemTypeMask & R_PROBLEM_POTENTIAL)
    {
        root.addChild(RProblemTaskItem(R_PROBLEM_POTENTIAL));
        nTypes ++;
    }
    if (problemTypeMask & R_PROBLEM_WAVE)
    {
        root.addChild(RProblemTaskItem(R_PROBLEM_WAVE));
        nTypes ++;
    }
    if (problemTypeMask & R_PROBLEM_MESH)
    {
        root.addChild(RProblemTaskItem(R_PROBLEM_MESH));
        nTypes ++;
    }

    root.setNIterations(nTypes > 1 && isNonlinear ? nNonlinearIterations : 1);

    return root;
}
