/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_environment_condition.h                                 *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   17-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Environment condition class declaration                *
 *********************************************************************/

#ifndef RML_ENVIRONMENT_CONDITION_H
#define RML_ENVIRONMENT_CONDITION_H

#include "rml_condition.h"
#include "rml_problem.h"

#define R_ENVIRONMENT_CONDITION_TYPE_IS_VALID(_type) \
( \
    _type >= R_ENVIRONMENT_CONDITION_NONE && \
    _type < R_ENVIRONMENT_CONDITION_N_TYPES \
)

//! Initial condition types.
typedef enum _REnvironmentConditionType
{
    R_ENVIRONMENT_CONDITION_NONE = 0,
    R_ENVIRONMENT_CONDITION_G_ACCELERATION,
    R_ENVIRONMENT_CONDITION_TEMPERATURE,
    R_ENVIRONMENT_CONDITION_CUSTOM,
    R_ENVIRONMENT_CONDITION_N_TYPES
} REnvironmentConditionType;


//! Environment condition class.
class REnvironmentCondition : public RCondition
{

    private:

        //! Internal initialization function.
        void _init ( const REnvironmentCondition *pCondition = nullptr );

        //! Set condition type.
        //! Calling thisfunction has no effect on condition
        //! component data.
        void setTypeSafe ( REnvironmentConditionType type );

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
        REnvironmentConditionType type;

    public:

        //! Constructor.
        REnvironmentCondition
          ( REnvironmentConditionType type = R_ENVIRONMENT_CONDITION_NONE );

        //! Copy constructor.
        REnvironmentCondition ( const REnvironmentCondition &condition );

        //! Destructor.
        ~REnvironmentCondition ();

        //! Return condition type.
        REnvironmentConditionType getType ( void ) const;

        //! Set condition type.
        void setType ( REnvironmentConditionType type );

        //! Assignment operator.
        REnvironmentCondition & operator =
                              ( const REnvironmentCondition &condition );

        //! Return environment condition type for given environment condition ID.
        static REnvironmentConditionType getTypeFromId( const QString &ecId );

        //! Return environment condition ID for specified environment condition type.
        static QString getId ( REnvironmentConditionType type );

        //! Return environment condition name.
        static const QString & getName ( REnvironmentConditionType type );

        //! Return problem type mask for given condition type.
        static RProblemTypeMask getProblemTypeMask ( REnvironmentConditionType type );

        //! Return default initial condition components information.
        static std::vector<RVariableType> getDefaultComponents ( REnvironmentConditionType type );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

//! ++ operator
void operator ++(REnvironmentConditionType &type, int increment);

#endif /* RML_ENVIRONMENT_CONDITION_H */
