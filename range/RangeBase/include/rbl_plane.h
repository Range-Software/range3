/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_plane.h                                              *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   20-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Plane class declaration                             *
 *********************************************************************/

#ifndef RBL_PLANE_H
#define RBL_PLANE_H

#include "rbl_r3vector.h"
#include "rbl_rmatrix.h"

class RPlane
{

    protected:

        //! Position vector.
        RR3Vector position;
        //! Normal vector.
        RR3Vector normal;

    private:

        //! Internal initialization function.
        void _init ( const RPlane *pPlane = nullptr );

    public:

        //! Constructor.
        RPlane ();

        //! Constructor (from position and normal).
        RPlane ( const RR3Vector &position,
                 const RR3Vector &normal );

        //! Constructor (from triangle).
        RPlane ( const RR3Vector &node1,
                 const RR3Vector &node2,
                 const RR3Vector &node3 );

        //! Copy constructor.
        RPlane ( const RPlane &rPlane );

        //! Destructor.
        ~RPlane ();

        //! Assignment operator.
        RPlane & operator = ( const RPlane &rPlane );

        //! Return const reference to position vector.
        const RR3Vector & getPosition ( void ) const;

        //! Set position vector.
        void setPosition ( const RR3Vector &position );

        //! Return const reference to normal vector.
        const RR3Vector & getNormal ( void ) const;

        //! Set normal vector.
        void setNormal ( const RR3Vector &normal );

        //! Calculate rotation matrix from local to global coordinates.
        //! To get matrix from global to local just invert R matrix.
        void findRotationMatrix ( RRMatrix &R ) const;

        //! Find three plane points.
        void findPoints ( RR3Vector &point1, RR3Vector &point2, RR3Vector &point3 ) const;

        //! Find line plane intersection point.
        //! Return true if intersection was found.
        bool findLineIntersection ( const RR3Vector &position, const RR3Vector &direction, RR3Vector &x, double &u ) const;

        //! Find segment plane intersection point.
        //! Return true if intersection was found.
        bool findSegmentIntersection ( const RR3Vector &node1, const RR3Vector &node2, RR3Vector &x ) const;

        //! Find distance from point to the plane.
        double findPointDistance( const RR3Vector &point ) const;

};

#endif /* RBL_PLANE_H */
