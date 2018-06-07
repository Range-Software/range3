/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_variable.cpp                                         *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   13-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Variable class definition                           *
 *********************************************************************/

#include <string>
#include <vector>
#include <cmath>

#include <rblib.h>

#include "rml_problem_type.h"
#include "rml_variable.h"

typedef struct _RVariableDesc
{
    QString           id;
    QString           name;
    QString           units;
    double            value;
    RVariableDataType dataType;
    RProblemTypeMask  problemTypeMask;
} RVariableDesc;

const RVariableDesc variableDesc [R_VARIABLE_N_TYPES] =
{
    { "var-none"                         , "None",                                      "N/A",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-acceleration"                 , "Acceleration",                              "m/s^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_STRESS | R_PROBLEM_FLUID },
    { "var-acceleration_x"               , "Acceleration in X direction",               "m/s^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-acceleration_y"               , "Acceleration in Y direction",               "m/s^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-acceleration_z"               , "Acceleration in Z direction",               "m/s^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-acoustic_particle_velocitty"  , "Acoustic particle velocity",                "m/s",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_ACOUSTICS },
    { "var-acoustic_pressure"            , "Acoustic pressure",                         "Pa",         0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_ACOUSTICS },
    { "var-charge_density"               , "Charge density",                            "C",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_ELECTROSTATICS },
    { "var-convection_coefficient"       , "Convection coefficient",                    "W/(m^2*K)",  100.0,   R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_FLUID },
    { "var-current_density"              , "Current density",                           "A/m^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_ELECTROSTATICS },
    { "var-current_density_x"            , "Current density in X direction",            "A/m^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-current_density_y"            , "Current density in Y direction",            "A/m^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-current_density_z"            , "Current density in Z direction",            "A/m^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-density"                      , "Density",                                   "kg/m^3",     0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_FLUID },
    { "var-displacement"                 , "Displacement",                              "m",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "var-displacement_x"               , "Displacement in X direction",               "m",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-displacement_y"               , "Displacement in Y direction",               "m",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-displacement_z"               , "Displacement in Z direction",               "m",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-dynamic_viscosity"            , "Dynamic viscosity",                         "kg/(m*s)",   0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_FLUID },
    { "var-electric_energy"              , "Electric energy",                           "J",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_ELECTROSTATICS },
    { "var-electric_field"               , "Electric field",                            "V/m",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_ELECTROSTATICS },
    { "var-electric_field_x"             , "Electric field in X direction",             "V/m",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-electric_field_y"             , "Electric field in Y direction",             "V/m",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-electric_field_z"             , "Electric field in Z direction",             "V/m",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-electric_potential"           , "Electric potential",                        "V",          1.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_ELECTROSTATICS },
    { "var-electrical_conductivity"      , "Electrical conductivity",                   "S/m",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-electrical_resistivity"       , "Electrical resistivity",                    "V*m/A",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_ELECTROSTATICS },
    { "var-emissivity"                   , "Emissivity",                                "N/A",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-fluid_temperature"            , "Fluid temperature",                         "K",          293.15,  R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-frequency"                    , "Frequency",                                 "Hz",         100.0,   R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL | R_PROBLEM_ACOUSTICS | R_PROBLEM_WAVE },
    { "var-force"                        , "Force",                                     "N",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_STRESS_MODAL },
    { "var-force_x"                      , "Force in X direction",                      "N",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-force_y"                      , "Force in Y direction",                      "N",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-force_z"                      , "Force in Z direction",                      "N",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-force_unit_area"              , "Force per unit area",                       "N/m^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-force_unit_area_x"            , "Force per unit area in X direction",        "N/m^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-force_unit_area_y"            , "Force per unit area in Y direction",        "N/m^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-force_unit_area_z"            , "Force per unit area in Z direction",        "N/m^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-g_acceleration"               , "Gravitational acceleration",                "m/s^2",      9.80665, R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-g_acceleration_x"             , "Gravitational acceleration in X direction", "m/s^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-g_acceleration_y"             , "Gravitational acceleration in Y direction", "m/s^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-g_acceleration_z"             , "Gravitational acceleration in Z direction", "m/s^2",     -9.80665, R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-heat_capacity"                , "Heat capacity",                             "J/(kg*K)",   0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-heat"                         , "Heat",                                      "W",          1000.0,  R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-heat_flux"                    , "Heat flux",                                 "W/m^2",      1000.0,  R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_HEAT },
    { "var-heat_flux_x"                  , "Heat flux in X direction",                  "W/m^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-heat_flux_y"                  , "Heat flux in Y direction",                  "W/m^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-heat_flux_z"                  , "Heat flux in Z direction",                  "W/m^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-heat_radiation"               , "Heat - radiation part",                     "W",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-heat_rate_area"               , "Heat per unit area",                        "W/m^2",      10.0,    R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-heat_rate_volume"             , "Heat per unit volume",                      "W/m^3",      10.0,    R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-heat_transfer_coefficientt"   , "Heat transfer coefficient",                 "W/(m^2*K)",  100.0,   R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_HEAT },
    { "var-hydraulic_diameter"           , "Hydraulic diameter",                        "m",          1.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-iteration"                    , "Iteration",                                 "#",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-joule_heat"                   , "Joule heat",                                "W",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_ELECTROSTATICS },
    { "var-magnetic_field"               , "Magnetic field",                            "T",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_ELECTROSTATICS },
    { "var-magnetic_field_x"             , "Magnetic field in X direction",             "T",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-magnetic_field_y"             , "Magnetic field in Y direction",             "T",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-magnetic_field_z"             , "Magnetic field in Z direction",             "T",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-mass_flow_rate"               , "Mass flow rate",                            "kg/s",       0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_FLUID | R_PROBLEM_POTENTIAL },
    { "var-mass_flow_rate_x"             , "Mass flow rate in X direction",             "kg/s",       0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-mass_flow_rate_y"             , "Mass flow rate in Y direction",             "kg/s",       0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-mass_flow_rate_z"             , "Mass flow rate in Z direction",             "kg/s",       0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-modulus_of_elasticity"        , "Modulus of elasticity",                     "Pa",         0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-particle_concentration"       , "Particle concentration",                    "kg/m^3",     0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_FLUID },
    { "var-particle_flux"                , "Particle flux",                             "kg/(m^3*s)", 0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-particle_rate"                , "Particle rate",                             "kg/(m^3*s)", 0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_FLUID },
    { "var-patch_area"                   , "Patch area",                                "m^2",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-patch_size"                   , "Patch size",                                "#",          0.0,     R_VARIABLE_DATA_UNSIGNED_INTEGER, R_PROBLEM_NONE },
    { "var-permeability_to_fluids"       , "Permeability to fluids",                    "m^2",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-pressure"                     , "Pressure",                                  "Pa",         1000.0,  R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_FLUID },
    { "var-poisson_ratio"                , "Poisson ratio",                             "N/A",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-potential"                    , "Potential",                                 "",           0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_ACOUSTICS },
    { "var-potential_velocity"           , "Potential / s",                             "1/s",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_ACOUSTICS },
    { "var-potential_acceleration"       , "Potential / s^2",                           "1/s^2",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_ACOUSTICS },
    { "var-radiation_emitter"            , "Radiation emitter",                         "N/A",        1.0,     R_VARIABLE_DATA_BOOLEAN, R_PROBLEM_NONE },
    { "var-radiation_receiver"           , "Radiation receiver",                        "N/A",        1.0,     R_VARIABLE_DATA_BOOLEAN, R_PROBLEM_NONE },
    { "var-relative_permittivity"        , "Relative permittivity",                     "C^2",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-separation_angle"             , "Separation angle",                          "Deg",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-sound_speed"                  , "Speed of sound",                            "m/s",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-stress"                       , "Stress",                                    "Pa",         0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-stress_x"                     , "Normal stress in X direction",              "Pa",         0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-stress_y"                     , "Normal stress in Y direction",              "Pa",         0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-stress_z"                     , "Normal stress in Z direction",              "Pa",         0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-stress_xy"                    , "Shear stress in XY plane",                  "Pa",         0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-stress_xz"                    , "Shear stress in XZ plane",                  "Pa",         0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-stress_yz"                    , "Shear stress in YZ plane",                  "Pa",         0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-stress_von_mises"             , "Von Mises stress",                          "Pa",         0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "var-stress_normal"                , "Normal stress",                             "Pa",         0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "var-stress_shear"                 , "Shear stress",                              "Pa",         0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_STRESS | R_PROBLEM_STRESS_MODAL },
    { "var-temperature"                  , "Temperature",                               "K",          293.15,  R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_HEAT | R_PROBLEM_FLUID },
    { "var-thermal_conductivity"         , "Thermal conductivity",                      "W/(m*K)",    0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-thermal_expansion_coefficient", "Thermal expansion coefficient",             "1/K",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-time"                         , "Time",                                      "s",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-velocity"                     , "Velocity",                                  "m/s",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_FLUID | R_PROBLEM_STRESS },
    { "var-velocity_x"                   , "Velocity in X direction",                   "m/s",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-velocity_y"                   , "Velocity in Y direction",                   "m/s",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-velocity_z"                   , "Velocity in Z direction",                   "m/s",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-vof"                          , "Volume of fraction",                        "%",          0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_FLUID },
    { "var-volume_flow_rate"             , "Volumetric flow rate",                      "m^3/s",      0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_FLUID },
    { "var-wave_displacement"            , "Wave displacement",                         "N/A",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_WAVE },
    { "var-weight"                       , "Weight",                                    "kg",         1.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-length"                       , "Length",                                    "m",          1.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-area"                         , "Area",                                      "m^2",        1.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-volume"                       , "Volume",                                    "m^3",        1.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE },
    { "var-custom"                       , "Custom",                                    "N/A",        0.0,     R_VARIABLE_DATA_DOUBLE,  R_PROBLEM_NONE }
};


RVariable::RVariable (RVariableType type, RVariableApplyType applyType)
{
    this->setType(type);
    this->setApplyType(applyType);
    this->_init();
} /* RVariable::RVariable */


RVariable::RVariable (const RVariable &variable)
{
    this->_init (&variable);
} /* RVariable::RVariable (copy) */


RVariable::~RVariable ()
{
} /* RVariable::~RVariable */


void RVariable::_init (const RVariable *pVariable)
{
    if (pVariable)
    {

        this->type = pVariable->type;
        this->applyType = pVariable->applyType;
        this->name = pVariable->name;
        this->units = pVariable->units;
        this->values = pVariable->values;
        this->variableData = pVariable->variableData;
    }
} /* RVariable::_init */


RVariableType RVariable::getType (void) const
{
    return this->type;
} /* RVariable::get_type */


void RVariable::setType (RVariableType type)
{
    R_ERROR_ASSERT (R_VARIABLE_TYPE_IS_VALID (type));
    this->type = type;
    this->setName(RVariable::getName(type));
    this->setUnits(RVariable::getUnits(type));
} /* RVariable::set_type */


RVariableApplyType RVariable::getApplyType (void) const
{
    return this->applyType;
} /* RVariable::get_apply_type */


void RVariable::setApplyType (RVariableApplyType type)
{
    R_ERROR_ASSERT (R_VARIABLE_APPLY_TYPE_IS_VALID (type));
    this->applyType = type;
} /* RVariable::set_apply_type */


const QString &RVariable::getName(void) const
{
    return this->name;
} /* RVariable::get_name */


void RVariable::setName (const QString &name)
{
    this->name = name;
} /* RVariable::set_name */


const QString &RVariable::getUnits(void) const
{
    return this->units;
} /* RVariable::get_units */


void RVariable::setUnits (const QString &units)
{
    this->units = units;
} /* RVariable::set_units */


unsigned int RVariable::getNVectors (void) const
{
    return (unsigned int)this->values.size();
} /* RVariable::getNVectors */


unsigned int RVariable::getNValues (void) const
{
    if (this->getNVectors() > 0)
    {
        return this->values[0].size();
    }
    else
    {
        return 0;
    }
} /* RVariable::getNValues */


void RVariable::clearValues(void)
{
    for (unsigned int i=0;i<this->values.size();i++)
    {
        this->values[i].fill(0.0);
    }
} /* RVariable::clearValues */


void RVariable::resize (unsigned int nvectors,
                        unsigned int nvalues,
                        bool fillInitValues)
{
    this->values.resize (nvectors);
    for (unsigned int i=0;i<nvectors;i++)
    {
        this->values[i].resize(nvalues);
        if (fillInitValues)
        {
            this->values[i].fill(RVariable::getInitValue(this->type));
        }
    }
} /* RVariable::resize */


double RVariable::getValue(unsigned int valpos) const
{
    R_ERROR_ASSERT (this->getNVectors() > 0);
    R_ERROR_ASSERT (valpos < this->getNValues());

    if (this->getNVectors() == 1)
    {
        return this->values[0][valpos];
    }
    else
    {
        double tmpValue = 0.0;
        for (unsigned int i=0;i<this->getNVectors();i++)
        {
            tmpValue += this->getValue(i,valpos)*this->getValue(i,valpos);
        }
        return std::sqrt(tmpValue);
    }
} /* RVariable::getValue */


double RVariable::getValue (unsigned int vecpos,
                            unsigned int valpos) const
{
    R_ERROR_ASSERT (vecpos < this->getNVectors ());
    R_ERROR_ASSERT (valpos < this->getNValues ());
    return this->values[vecpos][valpos];
} /* RVariable::getValue */


RRVector RVariable::getValueVector(unsigned int valpos) const
{
    R_ERROR_ASSERT (valpos < this->getNValues ());

    RRVector valueVector(this->getNVectors());

    for (unsigned int i=0;i<this->getNVectors();i++)
    {
        valueVector[i] = this->getValue(i,valpos);
    }

    return valueVector;
} /* RVariable::getValueVector */


RRVector RVariable::getValues(void) const
{
    RRVector values(this->getNValues());

    for (unsigned int i=0;i<this->getNValues();i++)
    {
        values[i] = this->getValue(i);
    }

    return values;
} /* RVariable::getValues */


RRVector RVariable::getValues(unsigned int vecpos) const
{
    R_ERROR_ASSERT (vecpos < this->getNVectors());

    RRVector values(this->getNValues());

    for (unsigned int i=0;i<this->getNValues();i++)
    {
        values[i] = this->getValue(vecpos,i);
    }

    return values;
}  /* RVariable::getValues */


double RVariable::getMinValue(void) const
{
    double minValue = 0.0;
    bool firstValue = true;

    for (unsigned int i=0;i<this->getNValues();i++)
    {
        if (firstValue)
        {
            minValue = this->getValue(i);
            firstValue = false;
        }
        else
        {
            minValue = std::min(minValue,this->getValue(i));
        }
    }

    return minValue;
} /* RVariable::getMinValue */


double RVariable::getMinValue(unsigned int vecpos) const
{
    double minValue = 0.0;
    bool firstValue = true;

    for (unsigned int i=0;i<this->getNValues();i++)
    {
        if (firstValue)
        {
            minValue = this->getValue(vecpos,i);
            firstValue = false;
        }
        else
        {
            minValue = std::min(minValue,this->getValue(vecpos,i));
        }
    }

    return minValue;
} /* RVariable::getMinValue */


double RVariable::getMaxValue(void) const
{
    double maxValue = 0.0;
    bool firstValue = true;

    for (unsigned int i=0;i<this->getNValues();i++)
    {
        if (firstValue)
        {
            maxValue = this->getValue(i);
            firstValue = false;
        }
        else
        {
            maxValue = std::max(maxValue,this->getValue(i));
        }
    }

    return maxValue;
} /* RVariable::getMaxValue */


double RVariable::getMaxValue(unsigned int vecpos) const
{
    double maxValue = 0.0;
    bool firstValue = true;

    for (unsigned int i=0;i<this->getNValues();i++)
    {
        if (firstValue)
        {
            maxValue = this->getValue(vecpos,i);
            firstValue = false;
        }
        else
        {
            maxValue = std::max(maxValue,this->getValue(vecpos,i));
        }
    }

    return maxValue;
} /* RVariable::getMaxValue */


void RVariable::setValue (unsigned int vecpos,
                          unsigned int valpos,
                          double       value)
{
    R_ERROR_ASSERT (vecpos < this->getNVectors ());
    R_ERROR_ASSERT (valpos < this->getNValues ());
    this->values[vecpos][valpos] = value;
} /* RVariable::setValue */


void RVariable::addValue (double value)
{
    std::vector<RValueVector>::iterator iter;

    for (iter = this->values.begin();
         iter != this->values.end();
         ++iter)
    {
        iter->add (value);
    }
} /* RVariable::addValue */


void RVariable::removeValue (unsigned int valpos)
{
    std::vector<RValueVector>::iterator iter;

    for (iter = this->values.begin();
         iter != this->values.end();
         ++iter)
    {
        iter->remove(valpos);
    }
} /* RVariable::removeValue */


void RVariable::removeValues(const std::vector<uint> &valueBook)
{    std::vector<RValueVector>::iterator iter;

     for (iter = this->values.begin();
          iter != this->values.end();
          ++iter)
     {
         iter->remove(valueBook);
     }

} /* RVariable::removeValues */


const RVariableData &RVariable::getVariableData(void) const
{
    return this->variableData;
} /* RVariable::getVariableData */


RVariableData &RVariable::getVariableData(void)
{
    return this->variableData;
} /* RVariable::getVariableData */


void RVariable::setVariableData(RVariableData &variableData)
{
    this->variableData = variableData;
} /* RVariable::setVariableData */


RVariable & RVariable::operator = (const RVariable &variable)
{
    this->_init (&variable);
    return (*this);
} /* RVariable::operator = */


const RValueVector & RVariable::operator [] (unsigned int vecpos) const
{
    R_ERROR_ASSERT (vecpos < this->getNVectors ());
    return this->values[vecpos];
} /* RVariable::operator [] */


RValueVector & RVariable::operator [] (unsigned int vecpos)
{
    R_ERROR_ASSERT (vecpos < this->getNVectors ());
    return this->values[vecpos];
} /* RVariable::operator [] */


RVariableType RVariable::getTypeFromId(const QString &variableId)
{
    for (uint type=uint(R_VARIABLE_NONE);type<uint(R_VARIABLE_N_TYPES);type++)
    {
        if (variableDesc[type].id == variableId)
        {
            return RVariableType(type);
        }
    }

    return R_VARIABLE_NONE;
} /* RVariable::getTypeFromId */


QString RVariable::getId(RVariableType type)
{
    R_ERROR_ASSERT (R_VARIABLE_TYPE_IS_VALID (type));
    return variableDesc[type].id;
} /* RVariable::getId */


const QString &RVariable::getName(RVariableType type)
{
    R_ERROR_ASSERT (R_VARIABLE_TYPE_IS_VALID (type));
    return variableDesc[type].name;
} /* RVariable::getName */


const QString &RVariable::getUnits(RVariableType type)
{
    R_ERROR_ASSERT (R_VARIABLE_TYPE_IS_VALID (type));
    return variableDesc[type].units;
} /* RVariable::getUnits */


double RVariable::getInitValue (RVariableType type)
{
    R_ERROR_ASSERT (R_VARIABLE_TYPE_IS_VALID (type));
    return variableDesc[type].value;
} /* RVariable::getInitValue */


RVariableDataType RVariable::getDataType(RVariableType type)
{
    R_ERROR_ASSERT (R_VARIABLE_TYPE_IS_VALID (type));
    return variableDesc[type].dataType;
} /* RVariable::getDataType */


RProblemTypeMask RVariable::getResultsProblemTypeMask(RVariableType type)
{
    R_ERROR_ASSERT (R_VARIABLE_TYPE_IS_VALID (type));
    return variableDesc[type].problemTypeMask;
} /* RVariable::getResultsProblemTypeMask */


const QString RVariable::guessVectorName(RVariableType type1,
                                              RVariableType type2,
                                              RVariableType type3)
{
    R_ERROR_ASSERT (R_VARIABLE_TYPE_IS_VALID (type1));
    R_ERROR_ASSERT (R_VARIABLE_TYPE_IS_VALID (type2));
    R_ERROR_ASSERT (R_VARIABLE_TYPE_IS_VALID (type3));

   if (RVariable::checkVectorTypes (type1,type2,type3,
                                    R_VARIABLE_ACCELERATION_X,
                                    R_VARIABLE_ACCELERATION_Y,
                                    R_VARIABLE_ACCELERATION_Z))
   {
       return RVariable::getName (R_VARIABLE_ACCELERATION);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_CURRENT_DENSITY_X,
                                         R_VARIABLE_CURRENT_DENSITY_Y,
                                         R_VARIABLE_CURRENT_DENSITY_Z))
   {
       return RVariable::getName (R_VARIABLE_CURRENT_DENSITY);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_DISPLACEMENT_X,
                                         R_VARIABLE_DISPLACEMENT_Y,
                                         R_VARIABLE_DISPLACEMENT_Z))
   {
       return RVariable::getName (R_VARIABLE_DISPLACEMENT);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_ELECTRIC_FIELD_X,
                                         R_VARIABLE_ELECTRIC_FIELD_Y,
                                         R_VARIABLE_ELECTRIC_FIELD_Z))
   {
       return RVariable::getName (R_VARIABLE_ELECTRIC_FIELD);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_FORCE_X,
                                         R_VARIABLE_FORCE_Y,
                                         R_VARIABLE_FORCE_Z))
   {
       return RVariable::getName (R_VARIABLE_FORCE);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_FORCE_UNIT_AREA_X,
                                         R_VARIABLE_FORCE_UNIT_AREA_Y,
                                         R_VARIABLE_FORCE_UNIT_AREA_Z))
   {
       return RVariable::getName (R_VARIABLE_FORCE_UNIT_AREA);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_G_ACCELERATION_X,
                                         R_VARIABLE_G_ACCELERATION_Y,
                                         R_VARIABLE_G_ACCELERATION_Z))
   {
       return RVariable::getName (R_VARIABLE_G_ACCELERATION);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_HEAT_FLUX_X,
                                         R_VARIABLE_HEAT_FLUX_Y,
                                         R_VARIABLE_HEAT_FLUX_Z))
   {
       return RVariable::getName (R_VARIABLE_HEAT_FLUX);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_MAGNETIC_FIELD_X,
                                         R_VARIABLE_MAGNETIC_FIELD_Y,
                                         R_VARIABLE_MAGNETIC_FIELD_Z))
   {
       return RVariable::getName (R_VARIABLE_MAGNETIC_FIELD);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_MASS_FLOW_RATE_X,
                                         R_VARIABLE_MASS_FLOW_RATE_Y,
                                         R_VARIABLE_MASS_FLOW_RATE_Z))
   {
       return RVariable::getName (R_VARIABLE_MASS_FLOW_RATE);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_STRESS_X,
                                         R_VARIABLE_STRESS_Y,
                                         R_VARIABLE_STRESS_Z))
   {
       return RVariable::getName (R_VARIABLE_STRESS_NORMAL);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_STRESS_XY,
                                         R_VARIABLE_STRESS_XZ,
                                         R_VARIABLE_STRESS_YZ))
   {
       return RVariable::getName (R_VARIABLE_STRESS_SHEAR);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_VELOCITY_X,
                                         R_VARIABLE_VELOCITY_Y,
                                         R_VARIABLE_VELOCITY_Z))
   {
       return RVariable::getName (R_VARIABLE_VELOCITY);
   }
   else
   {
       return "Vector";
   }
} /* RVariable::guessVectorName */

const QString RVariable::guessVectorUnits(RVariableType type1,
                                              RVariableType type2,
                                              RVariableType type3)
{
    R_ERROR_ASSERT (R_VARIABLE_TYPE_IS_VALID (type1));
    R_ERROR_ASSERT (R_VARIABLE_TYPE_IS_VALID (type2));
    R_ERROR_ASSERT (R_VARIABLE_TYPE_IS_VALID (type3));

   if (RVariable::checkVectorTypes (type1,type2,type3,
                                    R_VARIABLE_ACCELERATION_X,
                                    R_VARIABLE_ACCELERATION_Y,
                                    R_VARIABLE_ACCELERATION_Z))
   {
       return RVariable::getUnits (R_VARIABLE_ACCELERATION);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_CURRENT_DENSITY_X,
                                         R_VARIABLE_CURRENT_DENSITY_Y,
                                         R_VARIABLE_CURRENT_DENSITY_Z))
   {
       return RVariable::getUnits (R_VARIABLE_CURRENT_DENSITY);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_DISPLACEMENT_X,
                                         R_VARIABLE_DISPLACEMENT_Y,
                                         R_VARIABLE_DISPLACEMENT_Z))
   {
       return RVariable::getUnits (R_VARIABLE_DISPLACEMENT);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_ELECTRIC_FIELD_X,
                                         R_VARIABLE_ELECTRIC_FIELD_Y,
                                         R_VARIABLE_ELECTRIC_FIELD_Z))
   {
       return RVariable::getUnits (R_VARIABLE_ELECTRIC_FIELD);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_FORCE_X,
                                         R_VARIABLE_FORCE_Y,
                                         R_VARIABLE_FORCE_Z))
   {
       return RVariable::getUnits (R_VARIABLE_FORCE);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_FORCE_UNIT_AREA_X,
                                         R_VARIABLE_FORCE_UNIT_AREA_Y,
                                         R_VARIABLE_FORCE_UNIT_AREA_Z))
   {
       return RVariable::getUnits (R_VARIABLE_FORCE_UNIT_AREA);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_G_ACCELERATION_X,
                                         R_VARIABLE_G_ACCELERATION_Y,
                                         R_VARIABLE_G_ACCELERATION_Z))
   {
       return RVariable::getUnits (R_VARIABLE_G_ACCELERATION);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_HEAT_FLUX_X,
                                         R_VARIABLE_HEAT_FLUX_Y,
                                         R_VARIABLE_HEAT_FLUX_Z))
   {
       return RVariable::getUnits (R_VARIABLE_HEAT_FLUX);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_MAGNETIC_FIELD_X,
                                         R_VARIABLE_MAGNETIC_FIELD_Y,
                                         R_VARIABLE_MAGNETIC_FIELD_Z))
   {
       return RVariable::getUnits (R_VARIABLE_MAGNETIC_FIELD);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_MASS_FLOW_RATE_X,
                                         R_VARIABLE_MASS_FLOW_RATE_Y,
                                         R_VARIABLE_MASS_FLOW_RATE_Z))
   {
       return RVariable::getUnits (R_VARIABLE_MASS_FLOW_RATE);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_STRESS_X,
                                         R_VARIABLE_STRESS_Y,
                                         R_VARIABLE_STRESS_Z))
   {
       return RVariable::getUnits (R_VARIABLE_STRESS_NORMAL);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_STRESS_XY,
                                         R_VARIABLE_STRESS_XZ,
                                         R_VARIABLE_STRESS_YZ))
   {
       return RVariable::getUnits (R_VARIABLE_STRESS_SHEAR);
   }
   else if (RVariable::checkVectorTypes (type1,type2,type3,
                                         R_VARIABLE_VELOCITY_X,
                                         R_VARIABLE_VELOCITY_Y,
                                         R_VARIABLE_VELOCITY_Z))
   {
       return RVariable::getUnits (R_VARIABLE_VELOCITY);
   }
   else
   {
       return "Unknown";
   }
} /* RVariable::guessVectorUnits */


bool RVariable::checkVectorTypes(RVariableType typeA1,
                                 RVariableType typeA2,
                                 RVariableType typeA3,
                                 RVariableType typeB1,
                                 RVariableType typeB2,
                                 RVariableType typeB3)
{
    if ((typeA1 == typeB1 || typeA1 == typeB2 || typeA1 == typeB3) &&
        (typeA2 == typeB1 || typeA2 == typeB2 || typeA2 == typeB3)   )
    {
        if (typeA3 == R_VARIABLE_NONE)
        {
            return true;
        }
        else
        {
            if (typeA3 == typeB1 || typeA3 == typeB2 || typeA3 == typeB3)
            {
                return true;
            }
        }
    }
    return false;
} /* RVariable::checkVectorTypes */
