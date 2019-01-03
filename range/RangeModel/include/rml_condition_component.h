/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_condition_component.h                                *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   17-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Condition component class declaration               *
 *********************************************************************/

#ifndef RML_CONDITION_COMPONENT_H
#define RML_CONDITION_COMPONENT_H

#include <string>
#include <map>

#include <rblib.h>

#include "rml_variable.h"

//! Condition component class.
class RConditionComponent : public RValueTable
{

    private:

        //! Internal initialization function.
        void _init
                 ( const RConditionComponent *pConditionComponent = nullptr );

        //! Set table key name.
        void setKeyName ( const QString &name );

        //! Set table key units.
        void setKeyUnits ( const QString &units );

        //! Set table value name.
        void setValueName ( const QString &name );

        //! Set table value units.
        void setValueUnits ( const QString &units );

    protected:

        //! Condition table key type.
        RVariableType keyType;
        //! Condition table key type.
        RVariableType type;
        //! Whether the condition is enabled.
        bool enabled;

    public:

        //! Constructor
        RConditionComponent ( RVariableType type = R_VARIABLE_NONE );

        //! Copy constructor
        RConditionComponent
                      ( const RConditionComponent &conditionComponent );

        //! Destructor
        ~RConditionComponent ();

        //! Return condition table key type
        RVariableType getKeyType ( void ) const;

        //! Return variable type.
        RVariableType getType ( void ) const;

        //! Set variable type.
        void setType ( RVariableType type, bool clearContent = true );

        //! Return table value name.
        const QString & getName ( void ) const;

        //! Set table value name.
        void setName ( const QString &name );

        //! Return table value units.
        const QString & getUnits ( void ) const;

        //! Set table value units.
        void setUnits ( const QString &units );

        //! Return whether the component is enabled
        inline bool getEnabled ( void ) const
        {
            return this->enabled;
        }

        //! Enable or disable compontnent
        inline void setEnabled ( bool enabled )
        {
            this->enabled = enabled;
        }

        //! Assignment operator.
        RConditionComponent & operator =
                      ( const RConditionComponent &conditionComponent );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif /* RML_CONDITION_COMPONENT_H */
