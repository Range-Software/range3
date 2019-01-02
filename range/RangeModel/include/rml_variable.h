/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_variable.h                                           *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   13-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Variable class declaration                          *
 *********************************************************************/

#ifndef RML_VARIABLE_H
#define RML_VARIABLE_H

#include <string>
#include <vector>

#include <rblib.h>

#include "rml_problem_type.h"
#include "rml_variable_data.h"

#define R_VARIABLE_TYPE_IS_VALID(_type) \
( \
    _type >= R_VARIABLE_NONE && _type < R_VARIABLE_N_TYPES \
)

#define R_VARIABLE_APPLY_TYPE_IS_VALID(_type) \
(                                             \
    _type == R_VARIABLE_APPLY_NONE    ||      \
    _type == R_VARIABLE_APPLY_NODE    ||      \
    _type == R_VARIABLE_APPLY_ELEMENT ||      \
    _type == R_VARIABLE_APPLY_CUSTOM          \
)

//! Variable type definition
typedef enum _RVariableType
{
    R_VARIABLE_NONE = 0,
    R_VARIABLE_ACCELERATION,
    R_VARIABLE_ACCELERATION_X,
    R_VARIABLE_ACCELERATION_Y,
    R_VARIABLE_ACCELERATION_Z,
    R_VARIABLE_ACOUSTIC_PARTICLE_VELOCITY,
    R_VARIABLE_ACOUSTIC_PRESSURE,
    R_VARIABLE_CHARGE_DENSITY,
    R_VARIABLE_CONVECTION_COEFFICIENT,
    R_VARIABLE_CURRENT_DENSITY,
    R_VARIABLE_CURRENT_DENSITY_X,
    R_VARIABLE_CURRENT_DENSITY_Y,
    R_VARIABLE_CURRENT_DENSITY_Z,
    R_VARIABLE_DENSITY,
    R_VARIABLE_DISPLACEMENT,
    R_VARIABLE_DISPLACEMENT_X,
    R_VARIABLE_DISPLACEMENT_Y,
    R_VARIABLE_DISPLACEMENT_Z,
    R_VARIABLE_DYNAMIC_VISCOSITY,
    R_VARIABLE_ELECTRIC_ENERGY,
    R_VARIABLE_ELECTRIC_FIELD,
    R_VARIABLE_ELECTRIC_FIELD_X,
    R_VARIABLE_ELECTRIC_FIELD_Y,
    R_VARIABLE_ELECTRIC_FIELD_Z,
    R_VARIABLE_ELECTRIC_POTENTIAL,
    R_VARIABLE_ELECTRICAL_CONDUCTIVITY,
    R_VARIABLE_ELECTRICAL_RESISTIVITY,
    R_VARIABLE_EMISSIVITY,
    R_VARIABLE_FLUID_TEMPERATURE,
    R_VARIABLE_FREQUENCY,
    R_VARIABLE_FORCE,
    R_VARIABLE_FORCE_X,
    R_VARIABLE_FORCE_Y,
    R_VARIABLE_FORCE_Z,
    R_VARIABLE_FORCE_UNIT_AREA,
    R_VARIABLE_FORCE_UNIT_AREA_X,
    R_VARIABLE_FORCE_UNIT_AREA_Y,
    R_VARIABLE_FORCE_UNIT_AREA_Z,
    R_VARIABLE_G_ACCELERATION,
    R_VARIABLE_G_ACCELERATION_X,
    R_VARIABLE_G_ACCELERATION_Y,
    R_VARIABLE_G_ACCELERATION_Z,
    R_VARIABLE_HEAT_CAPACITY,
    R_VARIABLE_HEAT,
    R_VARIABLE_HEAT_FLUX,
    R_VARIABLE_HEAT_FLUX_X,
    R_VARIABLE_HEAT_FLUX_Y,
    R_VARIABLE_HEAT_FLUX_Z,
    R_VARIABLE_HEAT_RADIATION,
    R_VARIABLE_HEAT_RATE_AREA,
    R_VARIABLE_HEAT_RATE_VOLUME,
    R_VARIABLE_HEAT_TRANSFER_COEFFICIENT,
    R_VARIABLE_HYDRAULIC_DIAMETER,
    R_VARIABLE_ITERATION,
    R_VARIABLE_JOULE_HEAT,
    R_VARIABLE_MAGNETIC_FIELD,
    R_VARIABLE_MAGNETIC_FIELD_X,
    R_VARIABLE_MAGNETIC_FIELD_Y,
    R_VARIABLE_MAGNETIC_FIELD_Z,
    R_VARIABLE_MASS_FLOW_RATE,
    R_VARIABLE_MASS_FLOW_RATE_X,
    R_VARIABLE_MASS_FLOW_RATE_Y,
    R_VARIABLE_MASS_FLOW_RATE_Z,
    R_VARIABLE_MODULUS_OF_ELASTICITY,
    R_VARIABLE_PARTICLE_CONCENTRATION,
    R_VARIABLE_PARTICLE_FLUX,
    R_VARIABLE_PARTICLE_RATE,
    R_VARIABLE_PATCH_AREA,
    R_VARIABLE_PATCH_SIZE,
    R_VARIABLE_PERMEABILITY_TO_FLUIDS,
    R_VARIABLE_PRESSURE,
    R_VARIABLE_POISSON_RATIO,
    R_VARIABLE_POTENTIAL,
    R_VARIABLE_POTENTIAL_VELOCITY,
    R_VARIABLE_POTENTIAL_ACCELERATION,
    R_VARIABLE_RADIATION_EMITTER,
    R_VARIABLE_RADIATION_RECEIVER,
    R_VARIABLE_RELATIVE_PERMITTIVITY,
    R_VARIABLE_SEPARATION_ANGLE,
    R_VARIABLE_SOUND_SPEED,
    R_VARIABLE_STRESS,
    R_VARIABLE_STRESS_X,
    R_VARIABLE_STRESS_Y,
    R_VARIABLE_STRESS_Z,
    R_VARIABLE_STRESS_XY,
    R_VARIABLE_STRESS_XZ,
    R_VARIABLE_STRESS_YZ,
    R_VARIABLE_STRESS_VON_MISES,
    R_VARIABLE_STRESS_NORMAL,
    R_VARIABLE_STRESS_SHEAR,
    R_VARIABLE_TEMPERATURE,
    R_VARIABLE_THERMAL_CONDUCTIVITY,
    R_VARIABLE_THERMAL_EXPANSION_COEFFICIENT,
    R_VARIABLE_TIME,
    R_VARIABLE_VELOCITY,
    R_VARIABLE_VELOCITY_X,
    R_VARIABLE_VELOCITY_Y,
    R_VARIABLE_VELOCITY_Z,
    R_VARIABLE_VOF,
    R_VARIABLE_VOLUME_FLOW_RATE,
    R_VARIABLE_WAVE_DISPLACEMENT,
    R_VARIABLE_WEIGHT,
    R_VARIABLE_LENGTH,
    R_VARIABLE_AREA,
    R_VARIABLE_VOLUME,
    R_VARIABLE_CUSTOM,
    R_VARIABLE_N_TYPES
} RVariableType;

//! Variable data type definition
typedef enum _RVariableDataType
{
    R_VARIABLE_DATA_DOUBLE = 0,
    R_VARIABLE_DATA_INTEGER,
    R_VARIABLE_DATA_UNSIGNED_INTEGER,
    R_VARIABLE_DATA_BOOLEAN,
    R_VARIABLE_DATA_N_TYPES
} RVariableDataType;

//! Specifies how should be variable applied
typedef enum _RVariableApplyType
{
    R_VARIABLE_APPLY_NONE    = 0,
    R_VARIABLE_APPLY_NODE    = 1 << 0,
    R_VARIABLE_APPLY_ELEMENT = 1 << 1,
    R_VARIABLE_APPLY_CUSTOM  = 1 << 2
} RVariableApplyType;

//! Variable apply type mask.
typedef int RVariableApplyTypeMask;


//! Variable class.
class RVariable
{

    private:

        //! Internal initialization function
        void _init ( const RVariable *variable = nullptr );

    protected:

        //! Variable type.
        RVariableType type;
        //! Variable apply type.
        RVariableApplyType applyType;
        //! Variable name.
        QString name;
        //! Variable units.
        QString units;
        //! Values.
        std::vector<RValueVector> values;
        //! Variable data.
        RVariableData variableData;

    public:

        //! Constructor.
        RVariable (RVariableType type = R_VARIABLE_NONE, RVariableApplyType applyType = R_VARIABLE_APPLY_NONE);

        //! Copy constructor.
        RVariable (const RVariable &variable);

        //! Destructor.
        ~RVariable ();

        //! Return variable type.
        RVariableType getType ( void ) const;

        //! Set variable type.
        void setType ( RVariableType type );

        //! Return variable apply type.
        RVariableApplyType getApplyType ( void ) const;

        //! Set variable apply type.
        void setApplyType ( RVariableApplyType type );

        //! Return variable name.
        const QString & getName ( void ) const;

        //! Set variable name.
        void setName ( const QString &name );

        //! Return variable units.
        const QString & getUnits ( void ) const;

        //! Set variable units.
        void setUnits ( const QString &units );

        //! Return number of value vectors.
        unsigned int getNVectors ( void ) const;

        //! Return number of values in one vector.
        unsigned int getNValues ( void ) const;

        //! Clear values.
        void clearValues(void);

        //! Resize value vectors.
        void resize ( unsigned int nvectors,
                      unsigned int nvalues,
                      bool fillInitValues = false  );

        //! Return value at given position.
        //! If variable is vector type magnitude will be returned.
        double getValue ( unsigned int valpos ) const;

        //! Return value at given position.
        double getValue ( unsigned int vecpos,
                          unsigned int valpos ) const;

        //! Return value vector at given position.
        //! If variable is scalar vector will have only one component.
        RRVector getValueVector ( unsigned int valpos ) const;

        //! Return values.
        //! If variable is vector type magnitude values will be returned.
        RRVector getValues ( void ) const;

        //! Return values for given vector position.
        RRVector getValues ( unsigned int vecpos ) const;

        //! Return minimum value.
        //! If variable is vector type min. magnitude will be returned.
        double getMinValue ( void ) const;

        //! Return minimum value for a given vector.
        double getMinValue ( unsigned int vecpos ) const;

        //! Return maximum value.
        //! If variable is vector type max. magnitude will be returned.
        double getMaxValue ( void ) const;

        //! Return maximum value for a given vector.
        double getMaxValue ( unsigned int vecpos ) const;

        //! Set value at given position.
        void setValue ( unsigned int vecpos,
                        unsigned int valpos,
                        double       value  );

        //! Add new value to all vectors.
        void addValue ( double value = 0 );

        //! Remove value at given position from all vectors.
        void removeValue ( unsigned int valpos );

        //! Remove values at given positions from all vectors.
        //! If valueBook[i] == RConstants::eod then value will be removed.
        void removeValues(const std::vector<uint> &valueBook);

        //! Return const reference to variable data.
        const RVariableData & getVariableData ( void ) const;

        //! Return reference to variable data.
        RVariableData & getVariableData ( void );

        //! Set variable data.
        void setVariableData ( RVariableData &variableData );

        //! Assignment operator.
        RVariable & operator = ( const RVariable &variable );

        //! Return value at given position.
        const RValueVector & operator [] ( unsigned int vecpos ) const;

        //! Set value at given position.
        RValueVector & operator [] ( unsigned int vecpos );

        //! Return variable type for given variable ID.
        static RVariableType getTypeFromId( const QString &variableId );

        //! Return variable ID for specified variable type.
        static QString getId ( RVariableType type );

        //! Return variable type name.
        static const QString & getName ( RVariableType type );

        //! Return variable type units.
        static const QString & getUnits ( RVariableType type );

        //! Return variable type initial value.
        static double getInitValue ( RVariableType type );

        //! Return variable type data type.
        static RVariableDataType getDataType ( RVariableType type );

        //! Return problem type mask which has given variable type as a results.
        static RProblemTypeMask getResultsProblemTypeMask ( RVariableType type );

        //! Return variable vector name.
        static const QString guessVectorName ( RVariableType type1,
                                               RVariableType type2,
                                               RVariableType type3 = R_VARIABLE_NONE );

        //! Return variable vector units.
        static const QString guessVectorUnits ( RVariableType type1,
                                                RVariableType type2,
                                                RVariableType type3 = R_VARIABLE_NONE );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

    protected:

        //! Compare two variable vectors A and B whether their types match.
        static bool checkVectorTypes ( RVariableType typeA1,
                                       RVariableType typeA2,
                                       RVariableType typeA3,
                                       RVariableType typeB1,
                                       RVariableType typeB2,
                                       RVariableType typeB3 );

};

#endif /* RML_VARIABLE_H */
