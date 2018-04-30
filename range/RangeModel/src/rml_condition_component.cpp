/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_condition_component.cpp                              *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   17-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Condition component class definition                *
 *********************************************************************/

#include "rml_condition_component.h"
#include "rml_variable.h"

RConditionComponent::RConditionComponent (RVariableType type) : keyType(R_VARIABLE_TIME)
{
    this->setType (type);
    this->_init ();
} /* RConditionComponent::RConditionComponent */


RConditionComponent::RConditionComponent
                       (const RConditionComponent &conditionComponent)
                                    : RValueTable (conditionComponent)
{
    this->_init (&conditionComponent);
} /* RConditionComponent::RConditionComponent (copy) */


RConditionComponent::~RConditionComponent ()
{
} /* RConditionComponent::~RConditionComponent */


void RConditionComponent::_init
                      (const RConditionComponent *pConditionComponent)
{
    this->keyType = R_VARIABLE_TIME;
    this->RValueTable::setKeyName (RVariable::getName (this->getKeyType()));
    this->RValueTable::setKeyUnits (RVariable::getUnits (this->getKeyType()));
    if (pConditionComponent)
    {
        this->keyType = pConditionComponent->keyType;
        this->type = pConditionComponent->type;
        this->enabled = pConditionComponent->enabled;
    }
} /* RConditionComponent::_init */


RVariableType RConditionComponent::getKeyType (void) const
{
    return this->keyType;
} /* RConditionComponent::get_key_type */


RVariableType RConditionComponent::getType (void) const
{
    return this->type;
} /* RConditionComponent::get_type */


void RConditionComponent::setType (RVariableType type, bool clearContent)
{
    R_ERROR_ASSERT (R_VARIABLE_TYPE_IS_VALID(type));

    this->type = type;
    this->RValueTable::setValueName (RVariable::getName (type));
    this->RValueTable::setValueUnits (RVariable::getUnits (type));
    if (clearContent)
    {
        this->clear ();
        this->add (RVariable::getInitValue (this->getKeyType()),
                   RVariable::getInitValue (this->type));
    }
} /* RConditionComponent::setType */


const QString &RConditionComponent::getName(void) const
{
    return this->getValueName ();
} /* RConditionComponent::get_name */


void RConditionComponent::setName (const QString &name)
{
    this->RValueTable::setValueName (name);
} /* RConditionComponent::set_name */


const QString &RConditionComponent::getUnits(void) const
{
    return this->getValueUnits ();
} /* RConditionComponent::get_units */


void RConditionComponent::setUnits (const QString &units)
{
    this->RValueTable::setValueUnits (units);
} /* RConditionComponent::set_units */


RConditionComponent & RConditionComponent::operator =
                       (const RConditionComponent &conditionComponent)
{
    this->RValueTable::operator = (conditionComponent);
    this->_init (&conditionComponent);
    return (*this);
} /* RConditionComponent::operator = */
