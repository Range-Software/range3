/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_boundary_condition.h                                 *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   17-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Boundary condition class declaration                *
 *********************************************************************/

#ifndef RML_BOUNDARY_CONDITION_H
#define RML_BOUNDARY_CONDITION_H

#include "rml_condition.h"

#define R_BOUNDARY_CONDITION_TYPE_IS_VALID(_type) \
( \
    ( \
        _type >= R_BOUNDARY_CONDITION_NONE && \
        _type < R_BOUNDARY_CONDITION_N_TYPES \
    ) \
)

//! Boundary condition types.
typedef enum _RBoundaryConditionType
{
    R_BOUNDARY_CONDITION_NONE = 0,
    R_BOUNDARY_CONDITION_ABSORBING_BOUNDARY,
    R_BOUNDARY_CONDITION_CHARGE_DENSITY,
    R_BOUNDARY_CONDITION_CONVECTION_FORCED,
    R_BOUNDARY_CONDITION_CONVECTION_NATURAL,
    R_BOUNDARY_CONDITION_CONVECTION_SIMPLE,
    R_BOUNDARY_CONDITION_DISPLACEMENT,
    R_BOUNDARY_CONDITION_DISPLACEMENT_NORMAL,
    R_BOUNDARY_CONDITION_DISPLACEMENT_ROLLER,
    R_BOUNDARY_CONDITION_ELECTRIC_POTENTIAL,
    R_BOUNDARY_CONDITION_FORCE,
    R_BOUNDARY_CONDITION_FORCE_UNIT_AREA,
    R_BOUNDARY_CONDITION_HEAT,
    R_BOUNDARY_CONDITION_HEAT_RATE_AREA,
    R_BOUNDARY_CONDITION_HEAT_RATE_VOLUME,
    R_BOUNDARY_CONDITION_INFLOW_VELOCITY,
    R_BOUNDARY_CONDITION_INFLOW_VOLURATE,
    R_BOUNDARY_CONDITION_PARTICLE_CONCENTRATION,
    R_BOUNDARY_CONDITION_PARTICLE_RATE,
    R_BOUNDARY_CONDITION_PRESSURE,
    R_BOUNDARY_CONDITION_PRESSURE_EXPLICIT,
    R_BOUNDARY_CONDITION_PRESSURE_IMPLICIT,
    R_BOUNDARY_CONDITION_RADIATION,
    R_BOUNDARY_CONDITION_TEMPERATURE,
    R_BOUNDARY_CONDITION_VELOCITY_NORMAL,
    R_BOUNDARY_CONDITION_VELOCITY_POTENTIAL,
    R_BOUNDARY_CONDITION_WALL,
    R_BOUNDARY_CONDITION_WALL_FRICTIONLESS,
    R_BOUNDARY_CONDITION_WAVE_DISPLACEMENT,
    R_BOUNDARY_CONDITION_WEIGHT,
    R_BOUNDARY_CONDITION_CUSTOM,
    R_BOUNDARY_CONDITION_N_TYPES,
    R_BOUNDARY_CONDITION_ALL = ~R_BOUNDARY_CONDITION_NONE
} RBoundaryConditionType;


//! Boundary condition class
class RBoundaryCondition : public RCondition
{

    private:

        //! Internal initialization function.
        void _init ( const RBoundaryCondition *pCondition = nullptr );

        //! Set condition type.
        //! Calling thisfunction has no effect on condition
        //! component data.
        void setTypeSafe ( RBoundaryConditionType type );

        //! Disable set_apply_on_point for public use.
        void setApplyOnPoint ( bool apply );

        //! Disable set_apply_on_line for public use.
        void setApplyOnLine ( bool apply );

        //! Disable set_apply_on_surface for public use.
        void setApplyOnSurface ( bool apply );

        //! Disable set_apply_on_volume for public use.
        void setApplyOnVolume ( bool apply );

    protected:

        //! Condition type.
        RBoundaryConditionType type;
        //! Indicates whether the components are optional or not.
        bool optional;
        //! Indicates whether the condition is explicit or not.
        bool isExplicit;
        //! Indicates whether the boundary condition has local direction.
        bool hasLocalDirection;
        //! Local direction.
        RR3Vector direction;

    public:

        //! Constructor.
        RBoundaryCondition
          ( RBoundaryConditionType type = R_BOUNDARY_CONDITION_NONE );

        //! Copy constructor.
        RBoundaryCondition ( const RBoundaryCondition &condition );

        //! Destructor.
        ~RBoundaryCondition ();

        //! Return condition type.
        RBoundaryConditionType getType ( void ) const;

        //! Set condition type.
        void setType ( RBoundaryConditionType type );

        //! Return information on whether the components are optional or not.
        inline bool getOptional ( void ) const
        {
            return this->optional;
        }

        //! Return information on whether the condition is explicit or not.
        inline bool getExplicit ( void ) const
        {
            return this->isExplicit;
        }

        //! Return whether the condition has local direction.
        bool getHasLocalDirection() const;

        //! Return reference local direction.
        const RR3Vector &getLocalDirection() const;

        //! Set local direction.
        void setLocalDirection(const RR3Vector &direction);

        //! Assignment operator.
        RBoundaryCondition & operator =
                              ( const RBoundaryCondition &condition );

        //! Return boundary condition type for given boundary condition ID.
        static RBoundaryConditionType getTypeFromId( const QString &bcId );

        //! Return boundary condition ID for specified boundary condition type.
        static QString getId ( RBoundaryConditionType type );

        //! Return boundary condition name.
        static const QString & getName ( RBoundaryConditionType type );

        //! Return information whether the boundary condition is optional or not.
        static bool getOptional ( RBoundaryConditionType type );

        //! Return information whether the boundary condition is explicit or not.
        static bool getExplicit ( RBoundaryConditionType type );

        //! Return information whether the boundary condition applies on point or not.
        static bool getApplyOnPoint ( RBoundaryConditionType type );

        //! Return information whether the boundary condition applies on line or not.
        static bool getApplyOnLine ( RBoundaryConditionType type );

        //! Return information whether the boundary condition applies on surface or not.
        static bool getApplyOnSurface ( RBoundaryConditionType type );

        //! Return information whether the boundary condition applies on volume or not.
        static bool getApplyOnVolume ( RBoundaryConditionType type );

        //! Return information whether the boundary condition can have local rotation or not.
        static bool getHasLocalDirection ( RBoundaryConditionType type );

        //! Return problem type mask for given condition type.
        static RProblemTypeMask getProblemTypeMask ( RBoundaryConditionType type );

        //! Return list of boundary condition types for given problem type mask.
        //! If bcType is provided problem types which do not include given problem type will be excluded.
        static std::vector<RBoundaryConditionType> getTypes ( RProblemTypeMask problemTypeMask,
                                                              RBoundaryConditionType bcType = R_BOUNDARY_CONDITION_ALL );

        //! Return list of boundary condition types which are in conflict (can not be applied together)
        //! for given problem type mask and boundary condition type.
        static std::vector<RBoundaryConditionType> getConflictTypes ( RProblemTypeMask problemTypeMask,
                                                                      RBoundaryConditionType bcType );

        //! Return true if problem type requires explicit boundary condition.
        static bool getHasExplicit(RProblemType problemType);

        //! Return default boundary condition components information.
        static std::vector<RVariableType> getDefaultComponents ( RBoundaryConditionType type );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

//! ++ operator
void operator ++(RBoundaryConditionType &type, int increment);


#endif /* RML_BOUNDARY_CONDITION_H */
