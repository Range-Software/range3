/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_point.h                                              *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   8-th December 2011                                       *
 *                                                                   *
 *  DESCRIPTION: Point class definition                              *
 *********************************************************************/

#ifndef RML_POINT_H
#define RML_POINT_H

#include <vector>

#include "rml_element.h"
#include "rml_element_group.h"

//! Point element group class.
class RPoint : public RElementGroup
{

    public:

        //! Default entity name.
        const static QString defaultName;

    private:

        //! Internal initialization function.
        void _init ( const RPoint *pPoint = nullptr );

    protected:

        //! Point artificial volume.
        double volume;

    public:

        //! Constructor.
        RPoint ();

        //! Copy constructor.
        RPoint (const RPoint &point);

        //! Destructor.
        ~RPoint ();

        //! Return point group artificial volume.
        double getVolume ( void ) const;

        //! Set point group artificial volume.
        void setVolume ( double volume );

        //! Assignment operator.
        RPoint & operator = ( const RPoint &point );

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif /* RML_POINT_H */
