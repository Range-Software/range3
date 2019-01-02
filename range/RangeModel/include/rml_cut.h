/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_cut.h                                                *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   20-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Cut class declaration                               *
 *********************************************************************/

#ifndef RML_CUT_H
#define RML_CUT_H

#include <rblib.h>

#include "rml_interpolated_entity.h"

class RCut : public RInterpolatedEntity
{

    public:

        //! Default entity name.
        const static QString defaultName;

    protected:

        //! Cut plane.
        RPlane plane;
        //! List of element group IDs from which cut is constructed.
        std::vector<uint> elementGroupIDs;

    private:

        //! Internal initialization function.
        void _init ( const RCut *pCut = nullptr );

    public:

        //! Constructor.
        RCut();

        //! Copy constructor.
        RCut ( const RCut &cut );

        //! Destructor.
        ~RCut ();

        //! Assignment operator.
        RCut & operator = ( const RCut &cut );

        //! Return const reference to plane.
        const RPlane & getPlane ( void ) const;

        //! Return reference to plane.
        RPlane & getPlane ( void );

        //! Set variable type.
        void setPlane ( const RPlane &plane );

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

#endif /* RML_CUT_H */
