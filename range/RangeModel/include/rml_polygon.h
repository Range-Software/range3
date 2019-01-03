/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_polygon.h                                            *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   24-th October 2014                                       *
 *                                                                   *
 *  DESCRIPTION: Polygon class declaration                           *
 *********************************************************************/

#ifndef RML_POLYGON_H
#define RML_POLYGON_H

#include <list>

#include <rblib.h>

#include "rml_node.h"
#include "rml_element.h"

class RPolygon
{

    protected:

        RR3Vector normal;
        std::vector<RNode> nodes;

    private:

        //! Internal initialization function.
        void _init ( const RPolygon *pPolygon = nullptr );

    public:

        //! Constructor.
        explicit RPolygon ( const std::vector<RNode> &nodes );

        //! Constructor.
        explicit RPolygon ( const std::vector<RR3Vector> &nodes );

        //! Copy constructor.
        RPolygon ( const RPolygon &polygon );

        //! Destructor.
        ~RPolygon ();

        //! Assignment operator.
        RPolygon & operator = ( const RPolygon &polygon );

        //! Set polygon nodes.
        void setNodes ( const std::vector<RNode> &nodes );

        //! Remove node.
        void removeNode(uint nodePosition, bool updateNormal = true);

        //! Calculate polygon area.
        double findArea ( void ) const;

        //! Return number of nodes.
        uint getNNodes(void) const;

        //! Return polygon first node.
        inline const RNode & getNode ( uint nodePosition ) const
        {
            return this->nodes[nodePosition];
        }

        //! Return polygon normal.
        inline const RR3Vector & getNormal ( void ) const
        {
            return this->normal;
        }

        //! Sort polygon nodes and return vector of original node positions.
        static std::vector<uint> sortNodes(std::vector<RNode> &nodes);

        //! Triangulate polygon nodes and return vector of triangle elements.
        //! If nodesSorted is set to true it is expected that nodes do not need sorting.
        static std::vector<RElement> triangulate(const std::vector<RNode> &nodes, bool nodesSorted = false);

    protected:

        //! Compute polygon normal.
        void computeNormal (void);

        //! Find ear (sharpest triangle) node.
        uint findEarNode(void) const;

};

#endif // RML_POLYGON_H
