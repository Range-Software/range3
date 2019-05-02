/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_element.h                                            *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   8-th December 2011                                       *
 *                                                                   *
 *  DESCRIPTION: Element class declaration                           *
 *********************************************************************/

#ifndef RML_ELEMENT_H
#define RML_ELEMENT_H

#include <vector>
#include <string>

#include "rml_node.h"
#include "rml_element_shape_function.h"
#include "rml_triangle.h"
#include "rml_variable.h"

class RInterpolatedElement;

#define R_ELEMENT_TYPE_IS_VALID(_type) \
( \
    _type >= R_ELEMENT_NONE && _type < R_ELEMENT_N_TYPES \
)

#define R_ELEMENT_TYPE_IS_POINT(_type) \
( \
    _type == R_ELEMENT_POINT \
)

#define R_ELEMENT_TYPE_IS_LINE(_type) \
( \
    _type == R_ELEMENT_TRUSS1 || \
    _type == R_ELEMENT_TRUSS2 || \
    _type == R_ELEMENT_BEAM1  || \
    _type == R_ELEMENT_BEAM2     \
)

#define R_ELEMENT_TYPE_IS_SURFACE(_type) \
( \
    _type == R_ELEMENT_TRI1  || \
    _type == R_ELEMENT_TRI2  || \
    _type == R_ELEMENT_QUAD1 || \
    _type == R_ELEMENT_QUAD2    \
)

#define R_ELEMENT_TYPE_IS_VOLUME(_type) \
( \
    _type == R_ELEMENT_TETRA1 || \
    _type == R_ELEMENT_WEDGE1 || \
    _type == R_ELEMENT_HEXA1     \
)

#define R_ELEMENT_GROUP_TYPE_EQUALS(_type1,_type2) \
( \
    (R_ELEMENT_TYPE_IS_POINT(_type1)   && R_ELEMENT_TYPE_IS_POINT(_type2))   || \
    (R_ELEMENT_TYPE_IS_LINE(_type1)    && R_ELEMENT_TYPE_IS_LINE(_type2))    || \
    (R_ELEMENT_TYPE_IS_SURFACE(_type1) && R_ELEMENT_TYPE_IS_SURFACE(_type2)) || \
    (R_ELEMENT_TYPE_IS_VOLUME(_type1)  && R_ELEMENT_TYPE_IS_VOLUME(_type2))     \
)

//! Element types.
typedef enum _RElementType
{
    R_ELEMENT_NONE = 0,
    R_ELEMENT_POINT,
    R_ELEMENT_TRUSS1,
    R_ELEMENT_TRUSS2, // Not implemented
    R_ELEMENT_BEAM1, // Not implemented
    R_ELEMENT_BEAM2, // Not implemented
    R_ELEMENT_TRI1,
    R_ELEMENT_TRI2, // Not implemented
    R_ELEMENT_QUAD1,
    R_ELEMENT_QUAD2, // Not implemented
    R_ELEMENT_TETRA1,
    R_ELEMENT_WEDGE1, // Not implemented
    R_ELEMENT_HEXA1, // Not implemented
    R_ELEMENT_N_TYPES
} RElementType;

//! Element class.
class RElement
{

    private:

        //! Internal initialization function.
        void _init ( const RElement *element = nullptr );

    protected:

        //! Element type.
        RElementType type;
        //! List of node IDs (positions) in global node vecotr.
        std::vector <unsigned int> nodeIDs;

    public:

        //! Constructor.
        RElement (RElementType type = R_ELEMENT_NONE);

        //! Copy constructor.
        RElement (const RElement &element);

        //! Destructor.
        ~RElement ();

        //! Assignment operator.
        RElement & operator = ( const RElement &element );

        //! Equals operator.
        bool operator == ( const RElement &element ) const;

        //! Less than operator.
        bool operator < ( const RElement &element ) const;

        //! Get element type.
        RElementType getType ( void ) const;

        //! Set element type.
        void setType ( RElementType type );

        //! Return size of the element (number of nodes).
        unsigned int size ( void ) const;

        //! Return node ID at given position.
        unsigned int getNodeId ( unsigned int position ) const;

        //! Set new node ID at given position.
        void setNodeId ( unsigned int position,
                         unsigned int nodeID   );

        //! Swap node IDs.
        void swapNodeIds ( unsigned int position1,
                           unsigned int position2 );

        //! Check whether elemement has given node ID.
        bool hasNodeId ( unsigned int nodeID ) const;

        //! Check whether elemement has duplicate nodes (at least two nodes with same ID).
        bool hasDuplicateNodes ( void ) const;

        //! Swap element normal (nodes).
        //! If element is not a surface element, nothing will be done and false is returned.
        bool swapNormal ( void );

        //! Compute and return element's normal if possible.
        bool findNormal ( const std::vector <RNode> &nodes,
                          double &nx,
                          double &ny,
                          double &nz ) const;

        //! Compute and return element's center.
        void findCenter ( const std::vector <RNode> &nodes,
                          double &cx,
                          double &cy,
                          double &cz ) const;

        //! Compute and return element's area if possible.
        bool findShortestEdge ( const std::vector <RNode> &nodes,
                                double &length,
                                uint &node1,
                                uint &node2 ) const;

        //! Compute and return element's length if possible.
        bool findLength ( const std::vector <RNode> &nodes,
                          double &length ) const;

        //! Compute and return element's area if possible.
        bool findArea ( const std::vector <RNode> &nodes,
                        double &area ) const;

        //! Compute and return element's volume if possible.
        bool findVolume ( const std::vector <RNode> &nodes,
                          double &volume ) const;

        //! Compute and return element's size.
        double findSize ( const std::vector <RNode> &nodes ) const;

        //! Check if picking ray is intersecting given element and return intersection distance from the position.
        bool findPickDistance(const std::vector <RNode> &nodes,
                              const RR3Vector &position,
                              const RR3Vector &direction,
                              double tolerance,
                              double &distance) const;

        //! Create an interpolated element defined by plane.
        RInterpolatedElement createInterpolatedElement ( const RPlane &plane,
                                                         const std::vector <RNode> &nodes,
                                                         unsigned int elementID ) const;

        //! Create an interpolated element defined by value.
        RInterpolatedElement createInterpolatedElement ( double value,
                                                         const std::vector <double> &nodeValues,
                                                         const std::vector <RNode> &nodes,
                                                         unsigned int elementID ) const;

        //! Check if given node is inside the element.
        bool isInside( const std::vector <RNode> &nodes,
                       const RNode &node) const;

        //! Check if given node is inside the element.
        bool isInside( const std::vector <RNode> &nodes,
                       const RNode &node,
                       RRVector &volumes) const;

        //! Return interpolated value.
        //! Node values is a vector which size is equal to number of nodes in element.
        double interpolate( const std::vector <RNode> &nodes,
                            const RNode &interpolatedNode,
                            const RRVector &nodeValues ) const;

        //! Return interpolated value.
        //! Node values is a vector which size is equal to number of nodes in element.
        double interpolate( const std::vector <RNode> &nodes,
                            const RNode &interpolatedNode,
                            const RRVector &nodeValues,
                            const RRVector &volumes ) const;

        //! Find transformation matrix which provides rotation and translation from local to global coordinates.
        //! Return local (rotated) element coordinates.
        //! Rotate 2D elements into XY plane and 1D element into X axis.
        RRMatrix findTransformationMatrix( const std::vector <RNode> &nodes,
                                           RRMatrix &R,
                                           RRVector &t) const;

        //! Calculate jacobian matrix and its determinant.
        //! J - jacobian
        //! Rt - transformation matrix to global 3D coordinates.
        double findJacobian( const std::vector <RNode> &nodes,
                             unsigned int iPoint,
                             RRMatrix &J,
                             RRMatrix &Rt ) const;

        //! Find out on which side and where is given vector intersecting the element.
        unsigned int findIntersectedSide( const std::vector <RNode> &nodes,
                                          const RR3Vector &position,
                                          const RR3Vector &direction,
                                          RR3Vector &intersection) const;

        //! Find line segments.
        //! For line element this function will return list of segments which represent line of element.
        std::vector<RSegment> segmentize ( const std::vector <RNode> &nodes ) const;

        //! Find surface and volume triangles.
        //! For surface element this function will return list of triangles which represent surface of element.
        std::vector<RTriangle> triangulate ( const std::vector <RNode> &nodes ) const;

        //! Return number of edge elements.
        uint getNEdgeElements(void) const;

        //! Generate set of edge elements.
        std::vector<RElement> generateEdgeElements(void) const;

        //! Check if element is neighbor.
        bool isNeighbor( const RElement &rElement ) const;

        //! Find edge position for given neighbor.
        unsigned int findEdgePositionForNeighborElement( const RElement &rElement ) const;

        //! Check if neighbor orientation is in sync with element.
        bool isNeighborNormalSync( const RElement &rElement ) const;

        //! Return true if node at given position is on given edge position.
        bool nodeIsOnEdge(uint nodePosition, uint edgePosition) const;

        //! Merge nodes.
        //! If allowDowngrad = true, downgrade of dmensions is possible (tetrahedra => triangle).
        //! Return true if downgrade was performed.
        bool mergeNodes(uint nodeId1, uint nodeId2, bool allowDowngrade);

        //! Break element with given set of nodes.
        void breakWithNodes(const std::vector<RNode> &nodes, const std::vector<uint> breakNodeIDs, std::vector<RElement> &newElements);

        //! Break element with given node.
        void breakWithNode(const std::vector<RNode> &nodes, uint breakNodeID, std::vector<RElement> &newElements);

        //! Find limit box.
        void findLimitBox(const std::vector <RNode> &nodes, RLimitBox &limitBox) const;

        //! Get scalar vector.
        void findScalarNodeValues(uint elementID,
                                  const RVariable &rVariable,
                                  std::vector<double> &values) const;

        //! Get displacement vector.
        void findDisplacementNodeValues(uint elementID,
                                        const RVariable &rVariable,
                                        std::vector<RR3Vector> &values) const;

        //! Print element to standard output.
        void print ( const std::vector <RNode> &nodes ) const;

        //! Print element to standard output.
        void print ( void ) const;

        //! Allow RFileIO to access private members.
        friend class RFileIO;

        //! Return element type for given element ID.
        static RElementType getTypeFromId( const QString &elementId );

        //! Return element ID for specified element type.
        static QString getId ( RElementType type );

        //! Return name for given element type.
        static const QString & getName ( RElementType type );

        //! Return number of nodes for given element type.
        static unsigned int getNNodes ( RElementType type );

        //! Return number of possible neighbors for given element type.
        static unsigned int getNNeighbors ( RElementType type );

        //! Return newly generated shape function information for given element type.
        static std::vector<RElementShapeFunction> generateShapeFunctionValues ( RElementType type );

        //! Return newly generated Mass matrix for given element type.
        static RRMatrix generateMassMatrix ( RElementType type );

        //! Return newly generated Mass vector for given element type.
        static RRVector generateMassVector ( RElementType type );

        //! Return newly generated total weight factor for given element type.
        static double generateTotalWeightFactor ( RElementType type );

        //! Return number of integration points for given element type.
        static unsigned int getNIntegrationPoints ( RElementType type );

        //! Return shape function information for given element type.
        static const RElementShapeFunction & getShapeFunction ( RElementType type, unsigned int point );

        //! Return Mass matrix for given element type.
        static const RRMatrix & getMassMatrix ( RElementType type );

        //! Return Mass vector for given element type.
        static const RRVector & getMassVector ( RElementType type );

        //! Return total weight factor for given element type.
        static double getTotalWeightFactor ( RElementType type );

        //! Return true if shape derivative is constant for given element type.
        static bool hasConstantDerivative ( RElementType type );

        //! Find intersection points between two elements.
        static bool findIntersectionPoints(const RElement &e1, const RElement &e2, const std::vector<RNode> &nodes, std::set<RR3Vector> &x);

    private:

        //! Check if given node is inside the element.
        bool isInsideBoundingBox( const std::vector <RNode> &nodes,
                                  const RNode &node ) const;

        //! Test if plane is going through the element.
        bool testPlaneIntersect ( const RPlane &plane,
                                  const std::vector <RNode> &nodes ) const;

};

#endif /* RML_ELEMENT_H */
