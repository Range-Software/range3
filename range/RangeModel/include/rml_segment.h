/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_segment.h                                            *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   17-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Segment class declaration                           *
 *********************************************************************/

#ifndef RML_SEGMENT_H
#define RML_SEGMENT_H

#include <rblib.h>

#include "rml_node.h"

class RSegment
{

    protected:

        uint node1ID;
        uint node2ID;
        RNode node1;
        RNode node2;

    private:

        //! Internal initialization function.
        void _init ( const RSegment *pSegment = nullptr );

    public:

        //! Constructor.
        explicit RSegment ( const RNode &node1, const RNode &node2 );

        //! Copy constructor.
        RSegment ( const RSegment &segment );

        //! Destructor.
        ~RSegment ();

        //! Assignment operator.
        RSegment & operator = ( const RSegment &segment );

        //! Less than operator.
        bool operator < ( const RSegment &segment ) const;

        //! Set segment node IDs.
        void setNodeIDs ( uint node1ID, uint node2ID );

        //! Return true if segment contains nodeID.
        //! If nodeID is RConstants::eod false is returned.
        bool hasNodeID(uint nodeID) const;

        //! Set segment nodes.
        void setNodes ( const RNode &node1, const RNode &node2 );

        //! Calculate rotation matrix from local to global coordinates.
        //! To get matrix from global to local just invert R matrix.
        void findRotationMatrix ( RRMatrix &R ) const;

        //! Find distance of line from line.
        double findLineDistance ( const RR3Vector &position, const RR3Vector &direction ) const;

        //! Find distance of segment from line.
        double findSegmentDistance ( const RR3Vector &position, const RR3Vector &direction ) const;

        //! Find find nearest point on skewed line (two skewed lines).
        //! If onSegment == true then nearest point on segment will be returned
        RR3Vector findLineNearestPoint ( const RR3Vector &position, const RR3Vector &direction, bool onSegment = false ) const;

        //! Find line segment intersection point.
        //! Return true if intersection was found.
        //! This function ignores parallel lines.
        bool findLineIntersection ( const RR3Vector &position, const RR3Vector &direction, RR3Vector &x, double &u ) const;

        //! Find segment node intersection points.
        //! Return true if intersection was found.
        bool findPointIntersection ( const RNode &node, std::set<RR3Vector> &x) const;

        //! Find segment segment intersection points.
        //! Return true if intersection was found.
        bool findSegmentIntersection ( const RSegment &segment, std::set<RR3Vector> &x) const;

        //! Find segment segment intersection point.
        //! Return true if intersection was found.
        bool findSegmentIntersection ( const RSegment &segment, RR3Vector &x, double &u ) const;

        //! Find distance of point from line.
        double findPointDistance ( const RR3Vector &point ) const;

        //! Return true if point is inside the segment.
        bool isPointInside ( const RR3Vector &point, bool includeNodes = true ) const;

        //! Return node number if point is one of the nodes.
        //! If point is not at node RConstants::eod is returned.
        uint isPointAtNode ( const RR3Vector &point ) const;

        //! Find direction vector (n2 - n1).
        RR3Vector findDirection() const;

        //! Find direction vector (n2 - n1).
        void findDirection(RR3Vector &direction) const;

        //! Calculate segment length.
        double findLength() const;

        //! Calculate segment length.
        static double findLength ( const RNode &node1, const RNode &node2 );

        //! Calculate perpendicular vectors where d1 is the normalized direction of the segment.
        void findPerpendicularVectors(RR3Vector &d1, RR3Vector &d2, RR3Vector &d3) const;

        //! Return RNode first node ID.
        inline uint getNode1ID() const
        {
            return this->node1ID;
        }

        //! Return RNode second node ID.
        inline uint getNode2ID() const
        {
            return this->node2ID;
        }

        //! Return segment first node.
        inline const RNode & getNode1() const
        {
            return this->node1;
        }

        //! Return segment first node.
        inline RNode & getNode1()
        {
            return this->node1;
        }

        //! Return segment second node.
        inline const RNode & getNode2() const
        {
            return this->node2;
        }

        //! Return segment second node.
        inline RNode & getNode2()
        {
            return this->node2;
        }

        //! Print segment to standard output.
        void print() const;

    private:

        //! Find line/line intersection defined by four nodes.
        //! Lines must not be parallel.
        static bool findLineLineIntersection(const RR3Vector &n1,
                                             const RR3Vector &n2,
                                             const RR3Vector &n3,
                                             const RR3Vector &n4,
                                             RR3Vector &nx,
                                             double &u1,
                                             double &u2);

};

#endif /* RML_SEGMENT_H */
