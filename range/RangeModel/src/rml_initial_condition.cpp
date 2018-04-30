/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_initial_condition.cpp                                *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   17-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Initial condition class definition                  *
 *********************************************************************/

#include "rml_initial_condition.h"
#include "rml_problem.h"

typedef struct _RInitialConditionDesc
{
    QString          id;
    QString          name;
    RProblemTypeMask problemTypeMask;
} RInitialConditionDesc;

const RInitialConditionDesc initialConditionDesc [] =
{
    { "ic-none", "None",
      R_PROBLEM_NONE },
    { "ic-displacement", "Displacement",
      R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "ic-electric_potential", "Electric potential",
      R_PROBLEM_ELECTROSTATICS },
    { "ic-particle_contamination", "Particle concentration",
      R_PROBLEM_FLUID_PARTICLE },
    { "ic-pressure", "Pressure",
      R_PROBLEM_FLUID | R_PROBLEM_POTENTIAL | R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "ic-temperature", "Temperature",
      R_PROBLEM_HEAT | R_PROBLEM_RADIATIVE_HEAT | R_PROBLEM_FLUID_HEAT },
    { "ic-velocity", "Velocity",
      R_PROBLEM_FLUID | R_PROBLEM_POTENTIAL | R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "ic-velocity_potential", "Velocity potential",
      R_PROBLEM_ACOUSTICS },
    { "ic-wave_displacement", "Wave displacement",
      R_PROBLEM_WAVE },
    { "ic-custom", "Custom",
      R_PROBLEM_NONE }
};


RInitialCondition::RInitialCondition (RInitialConditionType type)
{
    this->setType (type);
    this->_init ();
} /* RInitialCondition::RInitialCondition */


RInitialCondition::RInitialCondition
                                  (const RInitialCondition &condition)
                                              : RCondition (condition)
{
    this->_init (&condition);
} /* RInitialCondition::RInitialCondition (copy) */


RInitialCondition::~RInitialCondition ()
{
} /* RInitialCondition::~RInitialCondition */


void RInitialCondition::_init (const RInitialCondition *pCondition)
{
    if (pCondition)
    {
        this->setTypeSafe (pCondition->getType());
    }
} /* RInitialCondition::_init */


RInitialConditionType RInitialCondition::getType (void) const
{
    return this->type;
} /* RInitialCondition::getType */


void RInitialCondition::setType (RInitialConditionType type)
{
    std::vector<RVariableType> componentTypes;
    RConditionComponent component;

    this->setTypeSafe (type);
    this->clear ();

    componentTypes = RInitialCondition::getDefaultComponents (type);

    for (unsigned int i=0;i<componentTypes.size();i++)
    {
        component.setType (componentTypes[i]);
        this->addComponent (component);
    }
} /* RInitialCondition::setType */


void RInitialCondition::setTypeSafe (RInitialConditionType type)
{
    this->type = type;
    this->name = RInitialCondition::getName (type);
    this->problemTypeMask = RInitialCondition::getProblemTypeMask (type);
} /* RInitialCondition::setTypeSafe */


RInitialCondition & RInitialCondition::operator =
                                  (const RInitialCondition &condition)
{
    this->RCondition::operator = (condition);
    this->_init (&condition);
    return (*this);
} /* RInitialCondition::operator = */


RInitialConditionType RInitialCondition::getTypeFromId(const QString &icId)
{
    for (uint type=uint(R_INITIAL_CONDITION_NONE);type<uint(R_INITIAL_CONDITION_N_TYPES);type++)
    {
        if (initialConditionDesc[type].id == icId)
        {
            return RInitialConditionType(type);
        }
    }

    return R_INITIAL_CONDITION_NONE;
} /* RInitialCondition::getTypeFromId */


QString RInitialCondition::getId(RInitialConditionType type)
{
    R_ERROR_ASSERT (R_INITIAL_CONDITION_TYPE_IS_VALID (type));
    return initialConditionDesc[type].id;
} /* RInitialCondition::getId */


const QString &RInitialCondition::getName(RInitialConditionType type)
{
    R_ERROR_ASSERT (R_INITIAL_CONDITION_TYPE_IS_VALID (type));
    return initialConditionDesc[type].name;
} /* RInitialCondition::getName */


RProblemTypeMask RInitialCondition::getProblemTypeMask
                                          (RInitialConditionType type)
{
    R_ERROR_ASSERT (R_INITIAL_CONDITION_TYPE_IS_VALID (type));
    return initialConditionDesc[type].problemTypeMask;
} /* RInitialCondition::getProblemTypeMask */


std::vector<RVariableType> RInitialCondition::getDefaultComponents
                                          (RInitialConditionType type)
{
    std::vector<RVariableType> componentTypes;

    R_ERROR_ASSERT (R_INITIAL_CONDITION_TYPE_IS_VALID (type));

    switch (type)
    {
        case R_INITIAL_CONDITION_NONE:
            break;
        case R_INITIAL_CONDITION_DISPLACEMENT:
            componentTypes.push_back (R_VARIABLE_DISPLACEMENT_X);
            componentTypes.push_back (R_VARIABLE_DISPLACEMENT_Y);
            componentTypes.push_back (R_VARIABLE_DISPLACEMENT_Z);
            break;
        case R_INITIAL_CONDITION_ELECTRIC_POTENTIAL:
            componentTypes.push_back (R_VARIABLE_ELECTRIC_POTENTIAL);
            break;
        case R_INITIAL_CONDITION_PARTICLE_CONCENTRATION:
            componentTypes.push_back (R_VARIABLE_PARTICLE_CONCENTRATION);
            break;
        case R_INITIAL_CONDITION_PRESSURE:
            componentTypes.push_back (R_VARIABLE_PRESSURE);
            break;
        case R_INITIAL_CONDITION_TEMPERATURE:
            componentTypes.push_back (R_VARIABLE_TEMPERATURE);
            break;
        case R_INITIAL_CONDITION_VELOCITY:
            componentTypes.push_back (R_VARIABLE_VELOCITY_X);
            componentTypes.push_back (R_VARIABLE_VELOCITY_Y);
            componentTypes.push_back (R_VARIABLE_VELOCITY_Z);
            break;
        case R_INITIAL_CONDITION_VELOCITY_POTENTIAL:
            componentTypes.push_back (R_VARIABLE_POTENTIAL);
            break;
        case R_INITIAL_CONDITION_WAVE_DISPLACEMENT:
            componentTypes.push_back (R_VARIABLE_WAVE_DISPLACEMENT);
            break;
        case R_INITIAL_CONDITION_CUSTOM:
            break;
        default:
            break;
    }

    return componentTypes;
} /* RInitialCondition::getDefaultComponents */


void operator ++(RInitialConditionType &type, int increment)
{
    type = (RInitialConditionType)((int)type+increment+1);
} /* operator ++ */
