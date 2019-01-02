/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_vector_field.h                                       *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   13-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Vector field class declaration                      *
 *********************************************************************/

#ifndef RML_VECTOR_FIELD_H
#define RML_VECTOR_FIELD_H

#include <vector>

#include "rml_variable.h"
#include "rml_entity_group.h"

class RVectorField : public REntityGroup
{

    public:

        //! Default entity name.
        const static QString defaultName;

    protected:

        //! Variable type.
        RVariableType variableType;
        //! 3D type.
        bool type3d;
        //! List of element group IDs from which field is constructed.
        std::vector<uint> elementGroupIDs;

    private:

        //! Internal initialization function.
        void _init ( const RVectorField *pVectorField = nullptr );

    public:

        //! Constructor.
        RVectorField();

        //! Copy constructor.
        RVectorField (const RVectorField &vectorField);

        //! Destructor.
        ~RVectorField ();

        //! Assignment operator.
        RVectorField & operator = ( const RVectorField &vectorField );

        //! Return variable type.
        RVariableType getVariableType ( void ) const;

        //! Set variable type.
        void setVariableType ( RVariableType variableType );

        //! Return 3D type.
        bool getType3D(void) const;

        //! Set 3D type.
        void setType3D(bool type3d);

        //! Return const reference to element group IDs.
        const std::vector<uint> & getElementGroupIDs ( void ) const;

        //! Return reference to element group IDs.
        std::vector<uint> & getElementGroupIDs ( void );

        //! Add element group ID.
        bool addElementGroupID ( uint elementGroupID );

        //! Remove element group ID.
        //! If decrement is seto to true all element group ID higher than the provided one
        //! will be decremented by one.
        bool removeElementGroupID ( uint elementGroupID, bool decrement = false );

        //! Clear element group IDs.
        void clearElementGroupIDs ( void );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif /* RML_VECTOR_FIELD_H */
