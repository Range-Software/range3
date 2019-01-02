/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_element_group.h                                      *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   8-th December 2011                                       *
 *                                                                   *
 *  DESCRIPTION: ElementGroup class declaration                      *
 *********************************************************************/

#ifndef RML_ELEMENT_GROUP_H
#define RML_ELEMENT_GROUP_H

#include <vector>
#include <map>

#include "rml_entity_group.h"
#include "rml_element.h"
#include "rml_entity_group_data.h"
#include "rml_boundary_condition.h"
#include "rml_environment_condition.h"
#include "rml_initial_condition.h"
#include "rml_material.h"

//! Element group class.
class RElementGroup : public REntityGroup
{

    private:

        //! Internal initialization function.
        void _init ( const RElementGroup *pElementGroup = nullptr );

    protected:

        //! List of element IDs.
        std::vector <unsigned int> elementIDs;
        //! List of boundary conditions.
        std::map <RBoundaryConditionType,RBoundaryCondition> bConditions;
        //! List of environment conditions.
        std::map <REnvironmentConditionType,REnvironmentCondition> eConditions;
        //! List of initial conditions.
        std::map <RInitialConditionType,RInitialCondition> iConditions;
        //! Material.
        RMaterial material;

    public:

        //! Constructor.
        RElementGroup ();

        //! Copy constructor.
        RElementGroup (const RElementGroup &elementGroup);

        //! Destructor.
        ~RElementGroup ();

        //! Assignment operator.
        RElementGroup & operator = ( const RElementGroup &elementGroup );

        //! Add element to the group.
        void add ( unsigned int elementID );

        //! Return element ID at given position in the group.
        unsigned int get ( unsigned int position ) const;

        //! Remove element ID at given position from the group.
        void remove ( unsigned int position );

        //! Replace element ID at given position in the group.
        void set ( unsigned int position,
                   unsigned int elementID );

        //! Find position of given element ID in the group.
        bool findPosition ( unsigned int  elementID,
                            unsigned int *position  ) const;

        //! Return number of elements in the group.
        unsigned int size ( void ) const;

        //! Resize the group.
        void resize ( unsigned int nmemb );

        //! Test if element group is empty.
        inline bool empty ( void ) const
        {
            return (this->size() == 0 ? true : false);
        }

        //! Return number of applied boundary conditions.
        inline unsigned int getNBoundaryConditions ( void ) const
        {
            return (unsigned int)this->bConditions.size();
        }

        //! Add boundary condition.
        //! If condition with its type already exist then the
        //! existing one will be replaced by the specified condition.
        void addBoundaryCondition
                            ( const RBoundaryCondition & bCondition );

        //! Return true if boundary condition with given type is assigned.
        bool hasBoundaryCondition
                                ( RBoundaryConditionType type ) const;

        //! Return boundary condition with given type.
        const RBoundaryCondition & getBoundaryCondition
                                ( RBoundaryConditionType type ) const;

        //! Return boundary condition with given type.
        RBoundaryCondition & getBoundaryCondition
                                      ( RBoundaryConditionType type );

        //! Return boundary condition at specified position.
        const RBoundaryCondition & getBoundaryCondition
                                      ( unsigned int position ) const;

        //! Return boundary condition at specified position.
        RBoundaryCondition & getBoundaryCondition
                                            ( unsigned int position );

        //! Remove boundary condition with given type.
        void removeBoundaryCondition ( RBoundaryConditionType type );

        //! Remove boundary condition at specified position.
        void removeBoundaryCondition ( unsigned int position );

        //! Remove all boundary conditions.
        void removeAllBoundaryConditions(void);

        //! Return number of applied environment conditions.
        inline unsigned int getNEnvironmentConditions ( void ) const
        {
            return (unsigned int)this->eConditions.size();
        }

        //! Add environment condition.
        //! If condition with its type already exist then the
        //! existing one will be replaced by the specified condition.
        void addEnvironmentCondition
                         ( const REnvironmentCondition & bCondition );

        //! Return true if environment condition with given type is assigned.
        bool hasEnvironmentCondition
                                ( REnvironmentConditionType type ) const;

        //! Return environment condition with given type.
        const REnvironmentCondition & getEnvironmentCondition
                             ( REnvironmentConditionType type ) const;

        //! Return environment condition with given type.
        REnvironmentCondition & getEnvironmentCondition
                                   ( REnvironmentConditionType type );

        //! Return environment condition at specified position.
        const REnvironmentCondition & getEnvironmentCondition
                                      ( unsigned int position ) const;

        //! Return environment condition at specified position.
        REnvironmentCondition & getEnvironmentCondition
                                            ( unsigned int position );

        //! Remove environment condition with given type.
        void removeEnvironmentCondition
                                   ( REnvironmentConditionType type );

        //! Remove environment condition at specified position.
        void removeEnvironmentCondition ( unsigned int position );

        //! Remove all environment conditions.
        void removeAllEnvironmentConditions(void);

        //! Return number of applied initial conditions.
        inline unsigned int getNInitialConditions ( void ) const
        {
            return (unsigned int)this->iConditions.size();
        }

        //! Add initial condition.
        //! If condition with its type already exist then the
        //! existing one will be replaced by the specified condition.
        void addInitialCondition
                             ( const RInitialCondition & bCondition );

        //! Return true if initial condition with given type is assigned.
        bool hasInitialCondition
                                ( RInitialConditionType type ) const;

        //! Return initial condition with given type.
        const RInitialCondition & getInitialCondition
                                 ( RInitialConditionType type ) const;

        //! Return initial condition with given type.
        RInitialCondition & getInitialCondition
                                 ( RInitialConditionType type );

        //! Return initial condition at specified position.
        const RInitialCondition & getInitialCondition
                                      ( unsigned int position ) const;

        //! Return initial condition at specified position.
        RInitialCondition & getInitialCondition
                                      ( unsigned int position );

        //! Remove initial condition with given type.
        void removeInitialCondition ( RInitialConditionType type );

        //! Remove initial condition at specified position.
        void removeInitialCondition ( unsigned int position );

        //! Remove all initial conditions.
        void removeAllInitialConditions(void);

        //! Return const reference to material.
        inline const RMaterial & getMaterial ( void ) const
        {
            return this->material;
        }

        //! Return reference to material.
        inline RMaterial & getMaterial ( void )
        {
            return this->material;
        }

        //! Set material.
        inline void setMaterial ( const RMaterial &material )
        {
            this->material = material;
        }

        //! Return element group type to which given element type belongs.
        static REntityGroupType getGroupType ( RElementType elementType );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

    protected:

        //! Set default environment conditions.
        void setDefaultEnvironmentConditions(void);

};

#endif /* RML_ELEMENT_GROUP_H */
