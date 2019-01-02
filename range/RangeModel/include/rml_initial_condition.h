/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_initial_condition.h                                  *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   17-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Initial condition class declaration                 *
 *********************************************************************/

#ifndef RML_INITIAL_CONDITION_H
#define RML_INITIAL_CONDITION_H

#include "rml_condition.h"
#include "rml_problem.h"

#define R_INITIAL_CONDITION_TYPE_IS_VALID(_type) \
( \
    _type >= R_INITIAL_CONDITION_NONE && \
    _type < R_INITIAL_CONDITION_N_TYPES \
)

//! Initial condition types.
typedef enum _RInitialConditionType
{
    R_INITIAL_CONDITION_NONE = 0,
    R_INITIAL_CONDITION_DISPLACEMENT,
    R_INITIAL_CONDITION_ELECTRIC_POTENTIAL,
    R_INITIAL_CONDITION_PARTICLE_CONCENTRATION,
    R_INITIAL_CONDITION_PRESSURE,
    R_INITIAL_CONDITION_TEMPERATURE,
    R_INITIAL_CONDITION_VELOCITY,
    R_INITIAL_CONDITION_VELOCITY_POTENTIAL,
    R_INITIAL_CONDITION_WAVE_DISPLACEMENT,
    R_INITIAL_CONDITION_CUSTOM,
    R_INITIAL_CONDITION_N_TYPES
} RInitialConditionType;

//! Initial condition class.
class RInitialCondition : public RCondition
{

    private:

        //! Internal initialization function.
        void _init ( const RInitialCondition *pCondition = nullptr );

        //! Set condition type.
        //! Calling thisfunction has no effect on condition
        //! component data.
        void setTypeSafe ( RInitialConditionType type );

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
        RInitialConditionType type;

    public:

        //! Constructor.
        RInitialCondition
            ( RInitialConditionType type = R_INITIAL_CONDITION_NONE );

        //! Copy constructor.
        RInitialCondition ( const RInitialCondition &condition );

        //! Destructor.
        ~RInitialCondition ();

        //! Return condition type.
        RInitialConditionType getType ( void ) const;

        //! Set condition type.
        void setType ( RInitialConditionType type );

        //! Assignment operator.
        RInitialCondition & operator =
                              ( const RInitialCondition &condition );

        //! Return initial condition type for given initial condition ID.
        static RInitialConditionType getTypeFromId( const QString &icId );

        //! Return initial condition ID for specified initial condition type.
        static QString getId ( RInitialConditionType type );

        //! Return initial condition name.
        static const QString & getName ( RInitialConditionType type );

        //! Return problem type mask for given condition type.
        static RProblemTypeMask getProblemTypeMask ( RInitialConditionType type );

        //! Return default initial condition components information.
        static std::vector<RVariableType> getDefaultComponents ( RInitialConditionType type );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

//! ++ operator
void operator ++(RInitialConditionType &type, int increment);

#endif /* RML_INITIAL_CONDITION_H */
