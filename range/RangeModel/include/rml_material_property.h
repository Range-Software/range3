/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_material_property.h                                  *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   15-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Material property class declaration                 *
 *********************************************************************/

#ifndef RML_MATERIAL_PROPERTY_H
#define RML_MATERIAL_PROPERTY_H

#include <string>
#include <map>

#include <rblib.h>

#include "rml_variable.h"
#include "rml_problem.h"

#define R_MATERIAL_PROPERTY_TYPE_IS_VALID(_type) \
( \
    _type >= R_MATERIAL_PROPERTY_NONE && \
    _type < R_MATERIAL_PROPERTY_N_TYPES \
)

typedef enum _RMaterialPropertyType
{
    R_MATERIAL_PROPERTY_NONE = 0,
    R_MATERIAL_PROPERTY_DENSITY,
    R_MATERIAL_PROPERTY_DYNAMIC_VISCOSITY,
    R_MATERIAL_PROPERTY_ELECTRICAL_CONDUCTIVITY,
    R_MATERIAL_PROPERTY_EMISSIVITY,
    R_MATERIAL_PROPERTY_HEAT_CAPACITY,
    R_MATERIAL_PROPERTY_MODULUS_OF_ELASTICITY,
    R_MATERIAL_PROPERTY_PERMEABILITY_TO_FLUIDS,
    R_MATERIAL_PROPERTY_POISSON_RATIO,
    R_MATERIAL_PROPERTY_RELATIVE_PERMITTIVITY,
    R_MATERIAL_PROPERTY_SOUND_SPEED,
    R_MATERIAL_PROPERTY_THERMAL_CONDUCTIVITY,
    R_MATERIAL_PROPERTY_THERMAL_EXPANSION_COEFFICIENT,
    R_MATERIAL_PROPERTY_CUSTOM,
    R_MATERIAL_PROPERTY_N_TYPES
} RMaterialPropertyType;

//! Material property class.
class RMaterialProperty : public RValueTable
{

    private:

        //! Internal initialization function.
        void _init ( const RMaterialProperty *pMaterialProperty = nullptr );

        //! Set table key name.
        void setKeyName ( const QString &name );

        //! Set table key units.
        void setKeyUnits ( const QString &units );

        //! Set table value name.
        void setValueName ( const QString &name );

        //! Set table value units.
        void setValueUnits ( const QString &units );

    protected:

        //! Material table key type
        RVariableType keyType;
        //! Material property type.
        RMaterialPropertyType type;

    public:

        //! Constructor.
        RMaterialProperty
            ( RMaterialPropertyType type = R_MATERIAL_PROPERTY_NONE );

        //! Copy constructor.
        RMaterialProperty
                        ( const RMaterialProperty &materialProperty );

        //! Destructor.
        ~RMaterialProperty ();

        //! Assignment operator.
        RMaterialProperty & operator =
                        ( const RMaterialProperty &materialProperty );

        //! Equal operator.
        bool operator ==
                        ( const RMaterialProperty &materialProperty ) const;

        //! Not equal operator.
        bool operator !=
                        ( const RMaterialProperty &materialProperty ) const;

        //! Return material table key type
        RVariableType getKeyType ( void ) const;

        //! Return material property type.
        RMaterialPropertyType getType ( void ) const;

        //! Set material property type.
        void setType ( RMaterialPropertyType type );

        //! Return table value name.
        const QString & getName ( void ) const;

        //! Set table value name.
        void setName ( const QString &name );

        //! Return table value units.
        const QString & getUnits ( void ) const;

        //! Set table value units.
        void setUnits ( const QString &units );

        //! Return material type for given material ID.
        static RMaterialPropertyType getTypeFromId( const QString &materialId );

        //! Return material ID for specified material type.
        static QString getId ( RMaterialPropertyType type );

        //! Return material property name
        static const QString & getName ( RMaterialPropertyType type );

        //! Return material property units
        static const QString & getUnits ( RMaterialPropertyType type );

        //! Return material property initial value
        static double getInitValue ( RMaterialPropertyType type );

        //! Return problem type mask for given condition type.
        static RProblemTypeMask getProblemTypeMask ( RMaterialPropertyType type );

        //! Return list of material properties required for given problem type mask.
        static std::vector<RMaterialPropertyType> getTypes( RProblemTypeMask problemTypeMask );

        //! Convert material property type to variable type.
        static RVariableType getVariableType ( RMaterialPropertyType type );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

//! ++ operator
void operator ++(RMaterialPropertyType &type, int increment);

#endif /* RML_MATERIAL_PROPERTY_H */
