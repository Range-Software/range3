/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_boundary_condition.cpp                               *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   17-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Boundary condition class definition                 *
 *********************************************************************/

#include "rml_boundary_condition.h"
#include "rml_problem.h"

typedef struct _RBoundaryConditionDesc
{
    QString          id;
    QString          name;
    bool             optional;
    bool             isExplicit;
    bool             applyOnPoint;
    bool             applyOnLine;
    bool             applyOnSurface;
    bool             applyOnVolume;
    bool             hasLocalDirections;
    RProblemTypeMask problemTypeMask;
} RBoundaryConditionDesc;

const RBoundaryConditionDesc boundaryConditionDesc [] =
{
    /* ID, Name, optional, explicit, point, line, surface, volume, has local direction */
    { "bc-none", "None", false, false, false, false, false, false, false, R_PROBLEM_NONE },
    { "bc-absorbing_boundary", "Absorbing boundary", false, false, true, true, true, true, false, R_PROBLEM_ACOUSTICS | R_PROBLEM_WAVE},
    { "bc-charge_density", "Charge density", false, false, true, true, true, true, false, R_PROBLEM_ELECTROSTATICS },
    { "bc-forced_convection", "Forced convection", false, false, false, false, true, false, false, R_PROBLEM_HEAT },
    { "bc-natural_convection", "Natural convection", false, false, false, false, true, false, false, R_PROBLEM_HEAT },
    { "bc-simple_convection", "Simple convection", false, false, false, false, true, false, false, R_PROBLEM_HEAT },
    { "bc-displacement", "Displacement", true, true, true, true, true, true, false, R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "bc-normal_displacement", "Normal displacement", false, true, false, false, true, false, true, R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "bc-roller_displacement", "Roller displacement", false, true, true, true, true, false, true, R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "bc-electric_potential", "Electric potential", false, true, true, true, true, true, false, R_PROBLEM_ELECTROSTATICS },
    { "bc-force", "Force", false, false, true, true, true, false, false, R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "bc-force_ua", "Force (unit area)", false, false, false, false, true, false, false, R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "bc-heat", "Heat", false, false, true, true, true, true, false, R_PROBLEM_HEAT | R_PROBLEM_FLUID_HEAT },
    { "bc-heat_rate_ua", "Heat rate (unit area)", false, false, false, false, true, false, false, R_PROBLEM_HEAT | R_PROBLEM_FLUID_HEAT },
    { "bc-heat_rate_uv", "Heat rate (unit volume)", false, false, false, false, false, true, false, R_PROBLEM_HEAT | R_PROBLEM_FLUID_HEAT },
    { "bc-velocity_outflow", "Velocity (inflow)", false, true, false, false, true, false, false, R_PROBLEM_FLUID | R_PROBLEM_POTENTIAL },
    { "bc-volumetric_flow_rate", "Volumetric flow rate (inflow)", false, true, false, false, true, false, false, R_PROBLEM_FLUID | R_PROBLEM_POTENTIAL },
    { "bc-particle_concentration", "Particle concentration", false, true, true, true, true, false, false, R_PROBLEM_FLUID_PARTICLE },
    { "bc-particle_rate", "Particle rate", false, false, true, true, true, true, false, R_PROBLEM_FLUID_PARTICLE },
    { "bc-pressure", "Pressure", false, false, false, false, true, false, false, R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "bc-pressure_explicit", "Pressure (explicit)", false, true, false, false, true, false, false, R_PROBLEM_FLUID | R_PROBLEM_POTENTIAL },
    { "bc-pressure_implicit", "Pressure (implicit)", false, false, false, false, true, false, false, R_PROBLEM_FLUID | R_PROBLEM_POTENTIAL },
    { "bc-radiation_boundary", "Radiation boundary", false, false, false, false, true, false, false, R_PROBLEM_RADIATIVE_HEAT },
    { "bc-temperature", "Temperature", false, true, true, true, true, true, false, R_PROBLEM_HEAT | R_PROBLEM_FLUID_HEAT },
    { "bc-velocity", "Velocity", false, false, false, false, true, false, false, R_PROBLEM_ACOUSTICS },
    { "bc-velocity_potential", "Velocity potential", false, true, true, true, true, false, false, R_PROBLEM_ACOUSTICS },
    { "bc-wall", "Wall", false, true, true, true, true, false, false, R_PROBLEM_FLUID },
    { "bc-wall_frictionless", "Wall (frictionless)", false, true, true, true, true, false, false, R_PROBLEM_FLUID },
    { "bc-wave_displacement", "Wave displacement", false, true, true, true, true, true, false, R_PROBLEM_WAVE },
    { "bc-weight", "Weight", false, false, true, true, true, false, false, R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "bc-custom", "Custom", false, false, false, false, false, false, false, R_PROBLEM_NONE }
};


RBoundaryCondition::RBoundaryCondition (RBoundaryConditionType type)
    : direction(0.0,0.0,1.0)
{
    this->setType(type);
    this->_init();
} /* RBoundaryCondition::RBoundaryCondition */


RBoundaryCondition::RBoundaryCondition
                                 (const RBoundaryCondition &condition)
                                              : RCondition (condition)
{
    this->_init (&condition);
} /* RBoundaryCondition::RBoundaryCondition (copy) */


RBoundaryCondition::~RBoundaryCondition ()
{
} /* RBoundaryCondition::~RBoundaryCondition */


void RBoundaryCondition::_init (const RBoundaryCondition *pCondition)
{
    if (pCondition)
    {
        this->type = pCondition->type;
        this->optional = pCondition->optional;
        this->isExplicit = pCondition->isExplicit;
        this->hasLocalDirection = pCondition->hasLocalDirection;
        this->direction = pCondition->direction;
    }
} /* RBoundaryCondition::_init */


RBoundaryConditionType RBoundaryCondition::getType(void) const
{
    return this->type;
} /* RBoundaryCondition::get_type */


void RBoundaryCondition::setType(RBoundaryConditionType type)
{
    std::vector<RVariableType> componentTypes;
    RConditionComponent component;

    this->setTypeSafe(type);
    this->clear();

    componentTypes = RBoundaryCondition::getDefaultComponents(type);

    for (unsigned int i=0;i<componentTypes.size();i++)
    {
        component.setType(componentTypes[i]);
        this->addComponent(component);
    }
} /* RBoundaryCondition::set_type */


void RBoundaryCondition::setTypeSafe(RBoundaryConditionType type)
{
    this->type = type;
    this->name = RBoundaryCondition::getName(type);
    this->optional = RBoundaryCondition::getOptional(type);
    this->isExplicit = RBoundaryCondition::getExplicit(type);
    this->applyOnPoint = RBoundaryCondition::getApplyOnPoint(type);
    this->applyOnLine = RBoundaryCondition::getApplyOnLine(type);
    this->applyOnSurface = RBoundaryCondition::getApplyOnSurface(type);
    this->applyOnVolume = RBoundaryCondition::getApplyOnVolume(type);
    this->hasLocalDirection = RBoundaryCondition::getHasLocalDirection(type);
    this->problemTypeMask = RBoundaryCondition::getProblemTypeMask(type);
} /* RBoundaryCondition::set_type_safe */


bool RBoundaryCondition::getHasLocalDirection() const
{
    return this->hasLocalDirection;
} /* RBoundaryCondition::getHasLocalDirection */

const RR3Vector &RBoundaryCondition::getLocalDirection() const
{
    return this->direction;
} /* RBoundaryCondition::getLocalDirection */


void RBoundaryCondition::setLocalDirection(const RR3Vector &direction)
{
    this->direction = direction;
} /* RBoundaryCondition::setLocalRotations */


RBoundaryCondition & RBoundaryCondition::operator =
                                 (const RBoundaryCondition &condition)
{
    this->RCondition::operator = (condition);
    this->_init (&condition);
    return (*this);
} /* RBoundaryCondition::operator = */


RBoundaryConditionType RBoundaryCondition::getTypeFromId(const QString &bcId)
{
    for (uint type=uint(R_BOUNDARY_CONDITION_NONE);type<uint(R_BOUNDARY_CONDITION_N_TYPES);type++)
    {
        if (boundaryConditionDesc[type].id == bcId)
        {
            return RBoundaryConditionType(type);
        }
    }

    return R_BOUNDARY_CONDITION_NONE;
} /* RBoundaryCondition::getTypeFromId */


QString RBoundaryCondition::getId(RBoundaryConditionType type)
{
    R_ERROR_ASSERT (R_BOUNDARY_CONDITION_TYPE_IS_VALID (type));
    return boundaryConditionDesc[type].id;
} /* RBoundaryCondition::getId */


const QString &RBoundaryCondition::getName(RBoundaryConditionType type)
{
    R_ERROR_ASSERT (R_BOUNDARY_CONDITION_TYPE_IS_VALID (type));
    return boundaryConditionDesc[type].name;
} /* RBoundaryCondition::getName */


bool RBoundaryCondition::getOptional (RBoundaryConditionType type)
{
    R_ERROR_ASSERT (R_BOUNDARY_CONDITION_TYPE_IS_VALID (type));
    return boundaryConditionDesc[type].optional;
} /* RBoundaryCondition::getOptional */


bool RBoundaryCondition::getExplicit (RBoundaryConditionType type)
{
    R_ERROR_ASSERT (R_BOUNDARY_CONDITION_TYPE_IS_VALID (type));
    return boundaryConditionDesc[type].isExplicit;
} /* RBoundaryCondition::getExplicit */


bool RBoundaryCondition::getApplyOnPoint (RBoundaryConditionType type)
{
    R_ERROR_ASSERT (R_BOUNDARY_CONDITION_TYPE_IS_VALID (type));
    return boundaryConditionDesc[type].applyOnPoint;
} /* RBoundaryCondition::getApplyOnPoint */


bool RBoundaryCondition::getApplyOnLine (RBoundaryConditionType type)
{
    R_ERROR_ASSERT (R_BOUNDARY_CONDITION_TYPE_IS_VALID (type));
    return boundaryConditionDesc[type].applyOnLine;
} /* RBoundaryCondition::getApplyOnLine */


bool RBoundaryCondition::getApplyOnSurface (RBoundaryConditionType type)
{
    R_ERROR_ASSERT (R_BOUNDARY_CONDITION_TYPE_IS_VALID (type));
    return boundaryConditionDesc[type].applyOnSurface;
} /* RBoundaryCondition::getApplyOnSurface */


bool RBoundaryCondition::getApplyOnVolume (RBoundaryConditionType type)
{
    R_ERROR_ASSERT (R_BOUNDARY_CONDITION_TYPE_IS_VALID (type));
    return boundaryConditionDesc[type].applyOnVolume;
} /* RBoundaryCondition::getApplyOnVolume */


bool RBoundaryCondition::getHasLocalDirection (RBoundaryConditionType type)
{
    R_ERROR_ASSERT (R_BOUNDARY_CONDITION_TYPE_IS_VALID (type));
    return boundaryConditionDesc[type].hasLocalDirections;
} /* RBoundaryCondition::getHasLocalDirection */


RProblemTypeMask RBoundaryCondition::getProblemTypeMask (RBoundaryConditionType type)
{
    R_ERROR_ASSERT (R_BOUNDARY_CONDITION_TYPE_IS_VALID (type));
    return boundaryConditionDesc[type].problemTypeMask;
} /* RBoundaryCondition::getProblemTypeMask */


std::vector<RBoundaryConditionType> RBoundaryCondition::getTypes (RProblemTypeMask problemTypeMask,
                                                                  RBoundaryConditionType bcType)
{
    R_ERROR_ASSERT (R_PROBLEM_TYPE_MASK_IS_VALID (problemTypeMask));

    std::vector<RProblemType> prbTypes = RProblem::getTypes(problemTypeMask);

    std::vector<bool> bcBook;
    bcBook.resize(R_BOUNDARY_CONDITION_N_TYPES,false);

    for (unsigned int i=0;i<prbTypes.size();i++)
    {
        if (bcType != R_BOUNDARY_CONDITION_ALL)
        {
            if (!(RBoundaryCondition::getProblemTypeMask(bcType) & prbTypes[i]))
            {
                continue;
            }
        }
        for (unsigned int j=0;j<bcBook.size();j++)
        {
            if (bcBook[j])
            {
                continue;
            }
            if (RBoundaryCondition::getProblemTypeMask(RBoundaryConditionType(j)) & prbTypes[i])
            {
                bcBook[j] = true;
            }
        }
    }

    std::vector<RBoundaryConditionType> bcTypes;
    for (unsigned int i=0;i<bcBook.size();i++)
    {
        if (bcBook[i])
        {
            bcTypes.push_back(RBoundaryConditionType(i));
        }
    }
    return bcTypes;
} /* RBoundaryCondition::getTypes */


std::vector<RBoundaryConditionType> RBoundaryCondition::getConflictTypes(RProblemTypeMask problemTypeMask,
                                                                         RBoundaryConditionType bcType)
{
    std::vector<RBoundaryConditionType> bcTypes;

    bcTypes = RBoundaryCondition::getTypes(problemTypeMask,bcType);
    for (unsigned int i=0;i<bcTypes.size();i++)
    {
        if (bcTypes[i] == bcType)
        {
            bcTypes.erase(bcTypes.begin() + i);
        }
    }

    return bcTypes;
} /* RBoundaryCondition::getConflictTypes */


bool RBoundaryCondition::getHasExplicit(RProblemType problemType)
{
    std::vector<RBoundaryConditionType> types = RBoundaryCondition::getTypes(problemType);
    for (unsigned int i=0;i<types.size();i++)
    {
        if (RBoundaryCondition::getExplicit(types[i]))
        {
            return true;
        }
    }
    return false;
} /* RBoundaryCondition::getNeedsExplicit */


std::vector<RVariableType> RBoundaryCondition::getDefaultComponents
                                         (RBoundaryConditionType type)
{
    std::vector<RVariableType> componentTypes;

    R_ERROR_ASSERT (R_BOUNDARY_CONDITION_TYPE_IS_VALID (type));

    switch (type)
    {
        case R_BOUNDARY_CONDITION_NONE:
            break;
        case R_BOUNDARY_CONDITION_ABSORBING_BOUNDARY:
            break;
        case R_BOUNDARY_CONDITION_CHARGE_DENSITY:
            componentTypes.push_back (R_VARIABLE_CHARGE_DENSITY);
            break;
        case R_BOUNDARY_CONDITION_CONVECTION_FORCED:
            componentTypes.push_back (R_VARIABLE_DENSITY);
            componentTypes.push_back (R_VARIABLE_DYNAMIC_VISCOSITY);
            componentTypes.push_back (R_VARIABLE_HEAT_CAPACITY);
            componentTypes.push_back (R_VARIABLE_HYDRAULIC_DIAMETER);
            componentTypes.push_back (R_VARIABLE_THERMAL_CONDUCTIVITY);
            componentTypes.push_back (R_VARIABLE_VELOCITY);
            break;
        case R_BOUNDARY_CONDITION_CONVECTION_NATURAL:
            componentTypes.push_back (R_VARIABLE_DENSITY);
            componentTypes.push_back (R_VARIABLE_DYNAMIC_VISCOSITY);
            componentTypes.push_back (R_VARIABLE_FLUID_TEMPERATURE);
            componentTypes.push_back (R_VARIABLE_HEAT_CAPACITY);
            componentTypes.push_back (R_VARIABLE_HYDRAULIC_DIAMETER);
            componentTypes.push_back (R_VARIABLE_THERMAL_CONDUCTIVITY);
            componentTypes.push_back (R_VARIABLE_THERMAL_EXPANSION_COEFFICIENT);
            break;
        case R_BOUNDARY_CONDITION_CONVECTION_SIMPLE:
            componentTypes.push_back (R_VARIABLE_CONVECTION_COEFFICIENT);
            componentTypes.push_back (R_VARIABLE_FLUID_TEMPERATURE);
            break;
        case R_BOUNDARY_CONDITION_DISPLACEMENT:
            componentTypes.push_back (R_VARIABLE_DISPLACEMENT_X);
            componentTypes.push_back (R_VARIABLE_DISPLACEMENT_Y);
            componentTypes.push_back (R_VARIABLE_DISPLACEMENT_Z);
            break;
        case R_BOUNDARY_CONDITION_DISPLACEMENT_NORMAL:
            componentTypes.push_back (R_VARIABLE_DISPLACEMENT);
            break;
        case R_BOUNDARY_CONDITION_DISPLACEMENT_ROLLER:
            componentTypes.push_back (R_VARIABLE_DISPLACEMENT);
            break;
        case R_BOUNDARY_CONDITION_ELECTRIC_POTENTIAL:
            componentTypes.push_back (R_VARIABLE_ELECTRIC_POTENTIAL);
            break;
        case R_BOUNDARY_CONDITION_FORCE:
            componentTypes.push_back (R_VARIABLE_FORCE_X);
            componentTypes.push_back (R_VARIABLE_FORCE_Y);
            componentTypes.push_back (R_VARIABLE_FORCE_Z);
            break;
        case R_BOUNDARY_CONDITION_FORCE_UNIT_AREA:
            componentTypes.push_back (R_VARIABLE_FORCE_UNIT_AREA_X);
            componentTypes.push_back (R_VARIABLE_FORCE_UNIT_AREA_Y);
            componentTypes.push_back (R_VARIABLE_FORCE_UNIT_AREA_Z);
            break;
        case R_BOUNDARY_CONDITION_HEAT:
            componentTypes.push_back (R_VARIABLE_HEAT);
            break;
        case R_BOUNDARY_CONDITION_HEAT_RATE_AREA:
            componentTypes.push_back (R_VARIABLE_HEAT_RATE_AREA);
            break;
        case R_BOUNDARY_CONDITION_HEAT_RATE_VOLUME:
            componentTypes.push_back (R_VARIABLE_HEAT_RATE_VOLUME);
            break;
        case R_BOUNDARY_CONDITION_PRESSURE_EXPLICIT:
            componentTypes.push_back (R_VARIABLE_PRESSURE);
            break;
        case R_BOUNDARY_CONDITION_INFLOW_VELOCITY:
            componentTypes.push_back (R_VARIABLE_VELOCITY);
            break;
        case R_BOUNDARY_CONDITION_INFLOW_VOLURATE:
            componentTypes.push_back (R_VARIABLE_VOLUME_FLOW_RATE);
            break;
        case R_BOUNDARY_CONDITION_PARTICLE_CONCENTRATION:
            componentTypes.push_back (R_VARIABLE_PARTICLE_CONCENTRATION);
            break;
        case R_BOUNDARY_CONDITION_PRESSURE_IMPLICIT:
            componentTypes.push_back (R_VARIABLE_PRESSURE);
            break;
        case R_BOUNDARY_CONDITION_PARTICLE_RATE:
            componentTypes.push_back (R_VARIABLE_PARTICLE_RATE);
            break;
        case R_BOUNDARY_CONDITION_PRESSURE:
            componentTypes.push_back (R_VARIABLE_PRESSURE);
            break;
        case R_BOUNDARY_CONDITION_RADIATION:
            componentTypes.push_back (R_VARIABLE_RADIATION_EMITTER);
            componentTypes.push_back (R_VARIABLE_RADIATION_RECEIVER);
            componentTypes.push_back (R_VARIABLE_PATCH_AREA);
            componentTypes.push_back (R_VARIABLE_PATCH_SIZE);
            componentTypes.push_back (R_VARIABLE_SEPARATION_ANGLE);
            break;
        case R_BOUNDARY_CONDITION_TEMPERATURE:
            componentTypes.push_back (R_VARIABLE_TEMPERATURE);
            break;
        case R_BOUNDARY_CONDITION_VELOCITY_NORMAL:
            componentTypes.push_back (R_VARIABLE_VELOCITY);
            break;
        case R_BOUNDARY_CONDITION_VELOCITY_POTENTIAL:
            componentTypes.push_back (R_VARIABLE_POTENTIAL);
            break;
        case R_BOUNDARY_CONDITION_WALL:
            break;
        case R_BOUNDARY_CONDITION_WAVE_DISPLACEMENT:
            componentTypes.push_back (R_VARIABLE_WAVE_DISPLACEMENT);
            break;
        case R_BOUNDARY_CONDITION_WEIGHT:
            componentTypes.push_back (R_VARIABLE_WEIGHT);
            break;
        case R_BOUNDARY_CONDITION_CUSTOM:
            break;
        default:
            break;
    }

    return componentTypes;
} /* RBoundaryCondition::getDefaultComponents */


void operator ++(RBoundaryConditionType &type, int increment)
{
    type = (RBoundaryConditionType)((int)type+increment+1);
} /* operator ++ */
