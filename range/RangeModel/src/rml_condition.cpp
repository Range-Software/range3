/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_condition.cpp                                        *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   17-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Condition class definition                          *
 *********************************************************************/

#include "rml_condition.h"

RCondition::RCondition ()
{
    this->_init ();
} /* RCondition::RCondition */


RCondition::RCondition (const RCondition &condition)
{
    this->_init (&condition);
} /* RCondition::RCondition (copy) */


RCondition::~RCondition ()
{
} /* RCondition::~RCondition */


void RCondition::_init (const RCondition *pCondition)
{
    if (pCondition)
    {
        this->name = pCondition->name;
        this->components = pCondition->components;
        this->applyOnPoint = pCondition->applyOnPoint;
        this->applyOnLine = pCondition->applyOnLine;
        this->applyOnSurface = pCondition->applyOnSurface;
        this->applyOnVolume = pCondition->applyOnVolume;
        this->problemTypeMask = pCondition->problemTypeMask;
    }
} /* RCondition::_init */


const QString &RCondition::getName(void) const
{
    return this->name;
} /* RCondition::get_name */


void RCondition::setName (const QString &name)
{
    this->name = name;
} /* RCondition::set_name */


bool RCondition::getApplyOnPoint ( void ) const
{
    return this->applyOnPoint;
} /* RCondition::get_apply_on_point */


void RCondition::setApplyOnPoint ( bool apply )
{
    this->applyOnPoint = apply;
} /* RCondition::set_apply_on_point */


bool RCondition::getApplyOnLine ( void ) const
{
    return this->applyOnLine;
} /* RCondition::get_apply_on_line */


void RCondition::setApplyOnLine ( bool apply )
{
    this->applyOnLine = apply;
} /* RCondition::set_apply_on_line */


bool RCondition::getApplyOnSurface ( void ) const
{
    return this->applyOnSurface;
} /* RCondition::get_apply_on_surface */


void RCondition::setApplyOnSurface ( bool apply )
{
    this->applyOnSurface = apply;
} /* RCondition::set_apply_on_surface */


bool RCondition::getApplyOnVolume ( void ) const
{
    return this->applyOnVolume;
} /* RCondition::get_apply_on_volume */


void RCondition::setApplyOnVolume ( bool apply )
{
    this->applyOnVolume = apply;
} /* RCondition::set_apply_on_volume */


RProblemTypeMask RCondition::getProblemTypeMask (void) const
{
    return this->problemTypeMask;
} /* RCondition::get_problem_type_mask */


void RCondition::setProblemTypeMask (RProblemTypeMask typeMask)
{
    R_ERROR_ASSERT (R_PROBLEM_TYPE_MASK_IS_VALID (typeMask));
    this->problemTypeMask = typeMask;
} /* RCondition::set_problem_type_mask */


bool RCondition::checkProblemTypeMask (RProblemType type) const
{
    R_ERROR_ASSERT (R_PROBLEM_TYPE_IS_VALID (type));
    return (type & this->getProblemTypeMask()) ? true : false;
} /* RCondition::check_problem_type_mask */


unsigned int RCondition::size (void) const
{
    return (unsigned int)this->components.size();
} /* RCondition::size */


bool RCondition::addComponent (const RConditionComponent &component)
{
    unsigned int position = 0;

    position = this->findComponentPosition (component.getType());

    if (position == RConstants::eod)
    {
        this->components.push_back(component);
        return true;
    }
    else
    {
        this->components[position] = component;
        return false;
    }
} /* RCondition::add */


const RConditionComponent & RCondition::getComponent (unsigned int position) const
{
    R_ERROR_ASSERT (position < this->size());
    return this->components[position];
} /* RCondition::get */


RConditionComponent & RCondition::getComponent (unsigned int position)
{
    R_ERROR_ASSERT (position < this->size());
    return this->components[position];
} /* RCondition::get */


void RCondition::removeComponent (unsigned int position)
{
    std::vector<RConditionComponent>::iterator iter;

    iter = this->components.begin();
    std::advance (iter, position);

    this->components.erase (iter);
} /* RCondition::remove */


unsigned int RCondition::findComponentPosition (RVariableType type) const
{
    std::vector<RConditionComponent>::const_iterator iter;
    unsigned int i = 0;

    for (iter = this->components.begin();
         iter != this->components.end();
         ++iter)
    {
        if (iter->getType() == type)
        {
           return i;
        }
        i++;
    }

    return RConstants::eod;
} /* RCondition::find_position */


RCondition & RCondition::operator = (const RCondition &condition)
{
    this->_init (&condition);
    return (*this);
} /* RCondition::operator = */
