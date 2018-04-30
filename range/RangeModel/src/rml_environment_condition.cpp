/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_environment_condition.cpp                            *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   17-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Environment condition class definition              *
 *********************************************************************/

#include "rml_environment_condition.h"
#include "rml_problem.h"

typedef struct _REnvironmentConditionDesc
{
    QString          id;
    QString          name;
    RProblemTypeMask problemTypeMask;
} REnvironmentConditionDesc;

const REnvironmentConditionDesc environmentConditionDesc [] =
{
    { "ec-none", "None",
      R_PROBLEM_NONE },
    { "ec-gravitational_acceleration", "Gravitational acceleration",
      R_PROBLEM_FLUID | R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL | R_PROBLEM_POTENTIAL },
    { "ec-temperature", "Temperature",
      R_PROBLEM_ACOUSTICS | R_PROBLEM_FLUID_PARTICLE | R_PROBLEM_ELECTROSTATICS | R_PROBLEM_FLUID | R_PROBLEM_HEAT | R_PROBLEM_RADIATIVE_HEAT | R_PROBLEM_FLUID_HEAT | R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL | R_PROBLEM_POTENTIAL | R_PROBLEM_WAVE },
    { "ec-custom", "Custom",
      R_PROBLEM_NONE }
};


REnvironmentCondition::REnvironmentCondition (REnvironmentConditionType type)
{
    this->setType (type);
    this->_init ();
} /* REnvironmentCondition::REnvironmentCondition */


REnvironmentCondition::REnvironmentCondition
                                 (const REnvironmentCondition &condition)
                                              : RCondition (condition)
{
    this->_init (&condition);
} /* REnvironmentCondition::REnvironmentCondition (copy) */


REnvironmentCondition::~REnvironmentCondition ()
{
} /* REnvironmentCondition::~REnvironmentCondition */


void REnvironmentCondition::_init (const REnvironmentCondition *pCondition)
{
    if (pCondition)
    {
        this->setTypeSafe (pCondition->getType());
    }
} /* REnvironmentCondition::_init */


REnvironmentConditionType REnvironmentCondition::getType (void) const
{
    return this->type;
} /* REnvironmentCondition::getType */


void REnvironmentCondition::setType (REnvironmentConditionType type)
{
    std::vector<RVariableType> componentTypes;
    RConditionComponent component;

    this->setTypeSafe (type);
    this->clear ();

    componentTypes = REnvironmentCondition::getDefaultComponents (type);

    for (unsigned int i=0;i<componentTypes.size();i++)
    {
        component.setType (componentTypes[i]);
        this->addComponent (component);
    }
} /* REnvironmentCondition::setType */


void REnvironmentCondition::setTypeSafe (REnvironmentConditionType type)
{
    this->type = type;
    this->name = REnvironmentCondition::getName (type);
    this->problemTypeMask = REnvironmentCondition::getProblemTypeMask (type);
} /* REnvironmentCondition::setTypeSafe */


REnvironmentCondition & REnvironmentCondition::operator =
                              (const REnvironmentCondition &condition)
{
    this->RCondition::operator = (condition);
    this->_init (&condition);
    return (*this);
} /* REnvironmentCondition::operator = */


REnvironmentConditionType REnvironmentCondition::getTypeFromId(const QString &ecId)
{
    for (uint type=uint(R_ENVIRONMENT_CONDITION_NONE);type<uint(R_ENVIRONMENT_CONDITION_N_TYPES);type++)
    {
        if (environmentConditionDesc[type].id == ecId)
        {
            return REnvironmentConditionType(type);
        }
    }

    return R_ENVIRONMENT_CONDITION_NONE;
} /* REnvironmentCondition::getTypeFromId */


QString REnvironmentCondition::getId(REnvironmentConditionType type)
{
    R_ERROR_ASSERT (R_ENVIRONMENT_CONDITION_TYPE_IS_VALID (type));
    return environmentConditionDesc[type].id;
} /* REnvironmentCondition::getId */


const QString &REnvironmentCondition::getName(REnvironmentConditionType type)
{
    R_ERROR_ASSERT (R_ENVIRONMENT_CONDITION_TYPE_IS_VALID (type));
    return environmentConditionDesc[type].name;
} /* REnvironmentCondition::getName */


RProblemTypeMask REnvironmentCondition::getProblemTypeMask
                                      (REnvironmentConditionType type)
{
    R_ERROR_ASSERT (R_ENVIRONMENT_CONDITION_TYPE_IS_VALID (type));
    return environmentConditionDesc[type].problemTypeMask;
} /* REnvironmentCondition::getProblemTypeMask */


std::vector<RVariableType> REnvironmentCondition::getDefaultComponents
                                         (REnvironmentConditionType type)
{
    std::vector<RVariableType> componentTypes;

    R_ERROR_ASSERT (R_ENVIRONMENT_CONDITION_TYPE_IS_VALID (type));

    switch (type)
    {
        case R_ENVIRONMENT_CONDITION_NONE:
            break;
        case R_ENVIRONMENT_CONDITION_G_ACCELERATION:
            componentTypes.push_back(R_VARIABLE_G_ACCELERATION_X);
            componentTypes.push_back(R_VARIABLE_G_ACCELERATION_Y);
            componentTypes.push_back(R_VARIABLE_G_ACCELERATION_Z);
            break;
        case R_ENVIRONMENT_CONDITION_TEMPERATURE:
            componentTypes.push_back(R_VARIABLE_TEMPERATURE);
            break;
        case R_ENVIRONMENT_CONDITION_CUSTOM:
            break;
        default:
            break;
    }

    return componentTypes;
} /* REnvironmentCondition::getDefaultComponents */


void operator ++(REnvironmentConditionType &type, int increment)
{
    type = (REnvironmentConditionType)((int)type+increment+1);
} /* operator ++ */
