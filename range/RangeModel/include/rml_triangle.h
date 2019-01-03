/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_triangle.h                                           *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   13-th September 2012                                     *
 *                                                                   *
 *  DESCRIPTION: Triangle class declaration                          *
 *********************************************************************/


#ifndef RML_TRIANGLE_H
#define RML_TRIANGLE_H

#include <rblib.h>

#include "rml_node.h"
#include "rml_segment.h"

class RTriangle
{

    protected:

        RR3Vector normal;
        uint node1ID;
        uint node2ID;
        uint node3ID;
        RNode node1;
        RNode node2;
        RNode node3;

    private:

        //! Internal initialization function.
        void _init ( const RTriangle *pTriangle = nullptr );

    public:

        //! Constructor.
        explicit RTriangle ( const RNode &node1,const RNode &node2,const RNode &node3 );

        //! Constructor.
        explicit RTriangle ( const RR3Vector &node1,const RR3Vector &node2,const RR3Vector &node3 );

        //! Copy constructor.
        RTriangle ( const RTriangle &triangle );

        //! Destructor.
        ~RTriangle ();

        //! Assignment operator.
        RTriangle & operator = ( const RTriangle &triangle );

        //! Set triangles node IDs.
        void setNodeIDs ( uint node1ID, uint node2ID, uint node3ID );

        //! Return true if triangle contains nodeID.
        //! If nodeID is RConstants::eod false is returned.
        bool hasNodeID(uint nodeID) const;

        //! Set triangles nodes.
        void setNodes ( const RNode &node1,const RNode &node2,const RNode &node3 );

        //! Calculate rotation matrix from local to global coordinates.
        //! It is assumed that triangle is in global coordinates.
        //! To get matrix from global to local just invert R matrix.
        void findRotationMatrix ( RRMatrix &R ) const;

        //! Find line triangle intersection point.
        //! Return true if intersection was found.
        bool findLineIntersection ( const RR3Vector &position, const RR3Vector &direction, RR3Vector &x, double &u ) const;

        //! Find segment triangle intersection point.
        //! Return true if intersection was found.
        //! Segment must not be parallel to the triangle plane.
        bool findSegmentIntersection ( const RSegment &segment, RR3Vector &x, double &u ) const;

        //! Find triangle node intersection points.
        //! Return true if intersection was found.
        bool findPointIntersection ( const RNode &node, RR3Vector &x ) const;

        //! Find triangle node intersection points.
        //! Return true if intersection was found.
        bool findPointIntersection ( const RNode &node, std::set<RR3Vector> &x ) const;

        //! Find triangle segment intersection points.
        //! Return true if intersection was found.
        bool findSegmentIntersection ( const RSegment &segment, std::set<RR3Vector> &x ) const;

        //! Find triangle triangle intersection points.
        //! Return true if intersection was found.
        bool findTriangleIntersection ( const RTriangle &triangle, std::set<RR3Vector> &x ) const;

        //! Return true if point is inside the triangle.
        bool isPointInside ( const RR3Vector &point, bool includeNodes = true ) const;

        //! Return node number if point is one of the nodes.
        //! If point is not at node RConstants::eod is returned.
        uint isPointAtNode ( const RR3Vector &point ) const;

        //! Calculate length of the longest edge.
        double findLongestEdgeLength(void) const;

        //! Calculate triangle area.
        double findArea ( void ) const;

        //! Calculate triangle area.
        static double findArea ( const RNode &node1, const RNode &node2, const RNode &node3 );

        //! Calculate triangle area.
        static double findArea ( double xi, double yi, double zi,
                                 double xj, double yj, double zj,
                                 double xk, double yk, double zk );

        //! Calculate triangle center.
        void findCenter(RR3Vector &center) const;

        //! Return triangles first node ID.
        inline uint getNode1ID ( void ) const
        {
            return this->node1ID;
        }

        //! Return triangles second node ID.
        inline uint getNode2ID ( void ) const
        {
            return this->node2ID;
        }

        //! Return triangles third node ID.
        inline uint getNode3ID ( void ) const
        {
            return this->node3ID;
        }

        //! Return triangles first node.
        inline const RNode & getNode1 ( void ) const
        {
            return this->node1;
        }

        //! Return triangles first node.
        inline RNode & getNode1 ( void )
        {
            return this->node1;
        }

        //! Return triangles second node.
        inline const RNode & getNode2 ( void ) const
        {
            return this->node2;
        }

        //! Return triangles second node.
        inline RNode & getNode2 ( void )
        {
            return this->node2;
        }

        //! Return triangles third node.
        inline const RNode & getNode3 ( void ) const
        {
            return this->node3;
        }

        //! Return triangles third node.
        inline RNode & getNode3 ( void )
        {
            return this->node3;
        }

        //! Return triangle normal.
        inline const RR3Vector & getNormal ( void ) const
        {
            return this->normal;
        }

        //! Calculate normal of a triangle represented by three nodes.
        static RR3Vector computeNormal(const RNode &node1, const RNode &node2, const RNode &node3, bool normalize = true);

        //! Calculate normal of a triangle represented by three vectors.
        static RR3Vector computeNormal(const RR3Vector &node1, const RR3Vector &node2, const RR3Vector &node3, bool normalize = true);

    protected:

        //! Compute triangle normal.
        void computeNormal (void);

        //! Find parallel segment triangle intersection points.
        bool findParallelSegmentIntersection ( const RSegment &segment, std::set<RR3Vector> &x ) const;

        //! Find skewed segment triangle intersection points.
        bool findSkewedSegmentIntersection ( const RSegment &segment, std::set<RR3Vector> &x ) const;

        //! Find parallel triangle triangle intersection points.
        bool findParallelTriangleIntersection ( const RTriangle &triangle, std::set<RR3Vector> &x ) const;

        //! Find skewed triangle triangle intersection points.
        bool findSkewedTriangleIntersection ( const RTriangle &triangle, std::set<RR3Vector> &x ) const;

};

#endif /* RML_TRIANGLE_H */
