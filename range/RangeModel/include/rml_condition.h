/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_condition.h                                          *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   17-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Condition class declaration                         *
 *********************************************************************/

#ifndef RML_CONDITION_H
#define RML_CONDITION_H

#include <string>
#include <vector>

#include "rml_condition_component.h"
#include "rml_problem.h"

//! Condition class
class RCondition
{

    private:

        //! Internal initialization function.
        void _init ( const RCondition *pCondition = nullptr );

    protected:

        //! Condition name.
        QString name;
        //! Condition components.
        std::vector<RConditionComponent> components;
        //! If condition can be applied on point groups.
        bool applyOnPoint;
        //! If condition can be applied on line groups.
        bool applyOnLine;
        //! If condition can be applied on surface groups.
        bool applyOnSurface;
        //! If condition can be applied on volume groups.
        bool applyOnVolume;
        //! Problem type mask.
        //! Specifies on which problem types is given condition
        //! applicable.
        RProblemTypeMask problemTypeMask;

    public:

        //! Constructor.
        RCondition ();

        //! Copy constructor.
        RCondition ( const RCondition &condition );

        //! Destructor.
        ~RCondition ();

        //! Return condition name.
        const QString & getName ( void ) const;

        //! Set condition name.
        void setName ( const QString &name );

        //! Return if condition can be applied on point groups.
        bool getApplyOnPoint ( void ) const;

        //! Set if condition can be applied on point groups.
        void setApplyOnPoint ( bool apply );

        //! Return if condition can be applied on line groups.
        bool getApplyOnLine ( void ) const;

        //! Set if condition can be applied on line groups.
        void setApplyOnLine ( bool apply );

        //! Return if condition can be applied on surface groups.
        bool getApplyOnSurface ( void ) const;

        //! Set if condition can be applied on surface groups.
        void setApplyOnSurface ( bool apply );

        //! Return if condition can be applied on volume groups.
        bool getApplyOnVolume ( void ) const;

        //! Set if condition can be applied on volume groups.
        void setApplyOnVolume ( bool apply );

        //! Return problem type nask.
        RProblemTypeMask getProblemTypeMask ( void ) const;

        //! Set problem type nask.
        void setProblemTypeMask ( RProblemTypeMask typeMask );

        //! Check if type is present in problem type nask.
        bool checkProblemTypeMask ( RProblemType type ) const;

        //! Return number of components.
        unsigned int size ( void ) const;

        //! Add new condition component.
        //! If a component with such type already exist an old
        //! component will be replaced.
        //! Return true if new component was added false on replace.
        bool addComponent ( const RConditionComponent &component );

        //! Return const reference to condition component at given position.
        const RConditionComponent & getComponent ( unsigned int position ) const;

        //! Return reference to condition component at given position.
        RConditionComponent & getComponent ( unsigned int position );

        //! Remove condition component at given position.
        void removeComponent ( unsigned int position );

        //! Find condition component position by its type.
        //! If no such component is present in condition RConstants::eod is returned.
        unsigned int findComponentPosition ( RVariableType type ) const;

        //! Drop all keys and values
        inline void clear ( void )
        {
            this->components.clear();
        }

        //! Test if table is empty.
        inline bool empty ( void ) const
        {
            return this->components.empty();
        }

        //! Assignment operator.
        RCondition & operator = ( const RCondition &condition );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif /* RML_CONDITION_H */
