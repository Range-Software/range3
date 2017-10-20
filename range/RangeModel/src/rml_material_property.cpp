/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_material_property.cpp                                *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   15-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Material property class definition                  *
 *********************************************************************/

#include "rml_material_property.h"
#include "rml_problem.h"
#include "rml_variable.h"

typedef struct _RMaterialPropertyDesc
{
    QString          id;
    RVariableType    variableType;
    RProblemTypeMask problemTypeMask;
} RMaterialPropertyDesc;

const RMaterialPropertyDesc materialPropertyDesc [] =
{
    { "prop-none",                          R_VARIABLE_NONE,                          R_PROBLEM_NONE },
    { "prop-density",                       R_VARIABLE_DENSITY,                       R_PROBLEM_ACOUSTICS | R_PROBLEM_FLUID_PARTICLE | R_PROBLEM_ELECTROSTATICS | R_PROBLEM_FLUID | R_PROBLEM_FLUID_HEAT | R_PROBLEM_HEAT | R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL | R_PROBLEM_POTENTIAL | R_PROBLEM_WAVE },
    { "prop-dynamic_viscosity",             R_VARIABLE_DYNAMIC_VISCOSITY,             R_PROBLEM_FLUID | R_PROBLEM_FLUID_HEAT },
    { "prop-electrical_conductivity",       R_VARIABLE_ELECTRICAL_CONDUCTIVITY,       R_PROBLEM_ELECTROSTATICS },
    { "prop-emissivity",                    R_VARIABLE_EMISSIVITY,                    R_PROBLEM_HEAT },
    { "prop-heat_capacity",                 R_VARIABLE_HEAT_CAPACITY,                 R_PROBLEM_HEAT | R_PROBLEM_FLUID_HEAT },
    { "prop-modulus_of_elasticity",         R_VARIABLE_MODULUS_OF_ELASTICITY,         R_PROBLEM_ACOUSTICS | R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "prop-permeability_to_fluids",        R_VARIABLE_PERMEABILITY_TO_FLUIDS,        R_PROBLEM_POTENTIAL },
    { "prop-poisson_ratio",                 R_VARIABLE_POISSON_RATIO,                 R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "prop-relative_permittivity",         R_VARIABLE_RELATIVE_PERMITTIVITY,         R_PROBLEM_ELECTROSTATICS },
    { "prop-soubd_speed",                   R_VARIABLE_SOUND_SPEED,                   R_PROBLEM_NONE },
    { "prop-thermal_conductivity",          R_VARIABLE_THERMAL_CONDUCTIVITY,          R_PROBLEM_HEAT | R_PROBLEM_FLUID_HEAT },
    { "prop-thermal_expansion_coefficient", R_VARIABLE_THERMAL_EXPANSION_COEFFICIENT, R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "prop-custom",                        R_VARIABLE_CUSTOM,                        R_PROBLEM_NONE }
};


void RMaterialProperty::_init
                          (const RMaterialProperty *pMaterialProperty)
{
    this->keyType = R_VARIABLE_TEMPERATURE;
    this->RValueTable::setKeyName (RVariable::getName (this->getKeyType()));
    this->RValueTable::setKeyUnits (RVariable::getUnits (this->getKeyType()));
    if (pMaterialProperty)
    {
        this->type = pMaterialProperty->getType();
        this->RValueTable::setValueName (RMaterialProperty::getName (this->type));
        this->RValueTable::setValueUnits (RMaterialProperty::getUnits (this->type));
    }
} /* RMaterialProperty::_init */


RMaterialProperty::RMaterialProperty (RMaterialPropertyType type)
{
    this->_init ();
    this->setType (type);
} /* RMaterialProperty::RMaterialProperty */


RMaterialProperty::RMaterialProperty
                           (const RMaterialProperty &materialProperty)
                                      : RValueTable (materialProperty)
{
    this->_init (&materialProperty);
} /* RMaterialProperty::RMaterialProperty (copy) */


RMaterialProperty::~RMaterialProperty ()
{
} /* RMaterialProperty::~RMaterialProperty */


RMaterialProperty & RMaterialProperty::operator =
                           (const RMaterialProperty &materialProperty)
{
    this->RValueTable::operator = (materialProperty);
    this->_init (&materialProperty);
    return (*this);
} /* RMaterialProperty::operator = */


bool RMaterialProperty::operator ==(const RMaterialProperty &materialProperty) const
{
    if (this->keyType != materialProperty.keyType)
    {
        return false;
    }
    if (this->type != materialProperty.type)
    {
        return false;
    }
    if (this->RValueTable::operator !=(materialProperty))
    {
        return false;
    }
    return true;
} /* RMaterialProperty::operator == */


bool RMaterialProperty::operator !=(const RMaterialProperty &materialProperty) const
{
    return !(this->operator ==(materialProperty));
} /* RMaterialProperty::operator != */


RVariableType RMaterialProperty::getKeyType (void) const
{
    return this->keyType;
} /* RMaterialProperty::get_key_type */


RMaterialPropertyType RMaterialProperty::getType (void) const
{
    return this->type;
} /* RMaterialProperty::get_type */


void RMaterialProperty::setType (RMaterialPropertyType type)
{
    R_ERROR_ASSERT (R_MATERIAL_PROPERTY_TYPE_IS_VALID(type));

    this->type = type;
    this->RValueTable::setValueName (RMaterialProperty::getName (type));
    this->RValueTable::setValueUnits (RMaterialProperty::getUnits (type));
    this->clear ();
    this->add (RVariable::getInitValue (this->getKeyType()),
               RMaterialProperty::getInitValue (this->type));
} /* RMaterialProperty::setType */


const QString &RMaterialProperty::getName(void) const
{
    return this->getValueName ();
} /* RMaterialProperty::set_name */


void RMaterialProperty::setName (const QString &name)
{
    this->RValueTable::setValueName (name);
} /* RMaterialProperty::set_name */


const QString &RMaterialProperty::getUnits(void) const
{
    return this->getValueUnits ();
} /* RMaterialProperty::getUnits */


void RMaterialProperty::setUnits (const QString &units)
{
    this->RValueTable::setValueUnits (units);
} /* RMaterialProperty::setUnits */


RMaterialPropertyType RMaterialProperty::getTypeFromId(const QString &materialId)
{
    for (uint type=uint(R_MATERIAL_PROPERTY_NONE);type<uint(R_MATERIAL_PROPERTY_N_TYPES);type++)
    {
        if (materialPropertyDesc[type].id == materialId)
        {
            return RMaterialPropertyType(type);
        }
    }

    return R_MATERIAL_PROPERTY_NONE;
} /* RMaterialProperty::getTypeFromId */


QString RMaterialProperty::getId(RMaterialPropertyType type)
{
    R_ERROR_ASSERT (R_MATERIAL_PROPERTY_TYPE_IS_VALID (type));
    return materialPropertyDesc[type].id;
} /* RMaterialProperty::getId */


const QString &RMaterialProperty::getName(RMaterialPropertyType type)
{
    R_ERROR_ASSERT (R_MATERIAL_PROPERTY_TYPE_IS_VALID (type));
    return RVariable::getName(RMaterialProperty::getVariableType(type));
} /* RMaterialProperty::getName */


const QString &RMaterialProperty::getUnits(RMaterialPropertyType type)
{
    R_ERROR_ASSERT (R_MATERIAL_PROPERTY_TYPE_IS_VALID (type));
    return RVariable::getUnits(RMaterialProperty::getVariableType(type));
} /* RMaterialProperty::getUnits */


double RMaterialProperty::getInitValue (RMaterialPropertyType type)
{
    R_ERROR_ASSERT (R_MATERIAL_PROPERTY_TYPE_IS_VALID (type));
    return RVariable::getInitValue(RMaterialProperty::getVariableType(type));
} /* RMaterialProperty::getInitValue */


RProblemTypeMask RMaterialProperty::getProblemTypeMask(RMaterialPropertyType type)
{
    R_ERROR_ASSERT (R_MATERIAL_PROPERTY_TYPE_IS_VALID (type));
    return materialPropertyDesc[type].problemTypeMask;
} /* RMaterialProperty::getProblemTypeMask */


std::vector<RMaterialPropertyType> RMaterialProperty::getTypes(RProblemTypeMask problemTypeMask)
{
    R_ERROR_ASSERT (R_PROBLEM_TYPE_MASK_IS_VALID (problemTypeMask));

    std::vector<RProblemType> prbTypes = RProblem::getTypes(problemTypeMask);

    std::vector<bool> bcBook;
    bcBook.resize(R_MATERIAL_PROPERTY_N_TYPES,false);

    for (unsigned int i=0;i<prbTypes.size();i++)
    {
        for (unsigned int j=0;j<bcBook.size();j++)
        {
            if (bcBook[j])
            {
                continue;
            }
            if (RMaterialProperty::getProblemTypeMask(RMaterialPropertyType(j)) & prbTypes[i])
            {
                bcBook[j] = true;
            }
        }
    }

    std::vector<RMaterialPropertyType> propertyTypes;
    for (unsigned int i=0;i<bcBook.size();i++)
    {
        if (bcBook[i])
        {
            propertyTypes.push_back(RMaterialPropertyType(i));
        }
    }

    return propertyTypes;
} /* RMaterialProperty::getTypes */


RVariableType RMaterialProperty::getVariableType(RMaterialPropertyType type)
{
    R_ERROR_ASSERT (R_MATERIAL_PROPERTY_TYPE_IS_VALID (type));
    return materialPropertyDesc[type].variableType;
} /* RMaterialProperty::getVariableType */


void operator ++(RMaterialPropertyType &type, int increment)
{
    type = (RMaterialPropertyType)((int)type+increment+1);
} /* operator ++ */
