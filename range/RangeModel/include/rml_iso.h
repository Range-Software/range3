/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_iso.h                                                *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   20-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: ISO class declaration                               *
 *********************************************************************/

#ifndef RML_ISO_H
#define RML_ISO_H

#include <string>
#include <vector>

#include "rml_variable.h"
#include "rml_interpolated_entity.h"

class RIso : public RInterpolatedEntity
{

    public:

        //! Default entity name.
        const static QString defaultName;

    protected:

        //! Variable type.
        RVariableType variableType;
        //! Variable value.
        double variableValue;
        //! List of element group IDs from which cut is constructed.
        std::vector<uint> elementGroupIDs;

    private:

        //! Internal initialization function.
        void _init ( const RIso *pIso = nullptr );

    public:

        //! Constructor.
        RIso();

        //! Copy constructor.
        RIso ( const RIso &iso );

        //! Destructor.
        ~RIso ();

        //! Assignment operator.
        RIso & operator = ( const RIso &iso );

        //! Return variable type.
        RVariableType getVariableType ( void ) const;

        //! Set variable type.
        void setVariableType ( RVariableType variableType );

        //! Return variable value.
        double getVariableValue ( void ) const;

        //! Set variable value.
        void setVariableValue ( double variableValue );

        //! Return const reference to element group IDs.
        const std::vector<uint> & getElementGroupIDs ( void ) const;

        //! Return reference to element group IDs.
        std::vector<uint> & getElementGroupIDs ( void );

        //! Add element group ID.
        bool addElementGroupID ( uint elementGroupID );

        //! Remove element group ID.
        bool removeElementGroupID ( uint elementGroupID, bool decrement = false );

        //! Clear element group IDs.
        void clearElementGroupIDs ( void );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif /* RML_ISO_H */
