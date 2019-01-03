/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_model_raw.h                                          *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   10-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: RAW model class declaration                         *
 *********************************************************************/

#ifndef RML_MODEL_RAW_H
#define RML_MODEL_RAW_H

#include <QTextStream>

#include <string>
#include <vector>

#include "rml_node.h"
#include "rml_element.h"

//! Raw triangulated surface class.
class RModelRaw
{

    private:

        //! Internal initialization function.
        void _init ( const RModelRaw *pModelRaw = nullptr );

    protected:

        //! List of nodes.
        std::vector<RNode> nodes;

        //! List of elements.
        std::vector<RElement> elements;

        //! Find near node to the given node.
        //! If no node was found a RConstants::eod is returned.
        unsigned int findNearNode ( const RNode &node,
                                    double       tolerance = 0.0 ) const;

        //! Merge all near nodes which distance is eaqual or smaller
        //! than 'tolerance'.
        //! Return number of merged nodes.
        unsigned int mergeNearNodes ( double tolerance = 0.0 );

    public:

        //! Constructor.
        RModelRaw ();

        //! Copy constructor.
        RModelRaw ( const RModelRaw &modelRaw );

        //! Destructor.
        ~RModelRaw ();

        //! Assignment operator.
        RModelRaw & operator = ( const RModelRaw &modelRaw );

        //! Return number of nodes.
        unsigned int getNNodes ( void ) const;

        //! Return const reference to node.
        const RNode & getNode ( unsigned int position ) const;

        //! Return reference to node.
        RNode & getNode ( unsigned int position );

        //! Return number of elements.
        unsigned int getNElements ( void ) const;

        //! Return const reference to element.
        const RElement & getElement ( unsigned int position ) const;

        //! Return reference to element.
        RElement & getElement ( unsigned int position );

        //! Add point.
        //! Specified tolerance determines how the nodes should be merged.
        void addPoint ( const RNode &node,
                        bool         mergeNodes = true,
                        double       tolerance = 0.0 );

        //! Add segment.
        //! Specified tolerance determines how the nodes should be merged.
        void addSegment ( const RNode &node1,
                          const RNode &node2,
                          bool         mergeNodes = true,
                          double       tolerance = 0.0 );

        //! Add triangle.
        //! Specified tolerance determines how the nodes should be merged.
        void addTriangle ( const RNode &node1,
                           const RNode &node2,
                           const RNode &node3,
                           bool         mergeNodes = true,
                           double       tolerance = 0.0 );

        //! Add quadrilateral.
        //! Specified tolerance determines how the nodes should be merged.
        void addQuadrilateral ( const RNode &node1,
                                const RNode &node2,
                                const RNode &node3,
                                const RNode &node4,
                                bool         mergeNodes = true,
                                double       tolerance = 0.0 );

        //! Remove all nodes and elements from the model.
        void clear (void);

        //! Read surface mesh from file.
        void read ( const QString &fileName,
                    double             tolerance );

        //! Write surface mesh to file.
        void write ( const QString &fileName ) const;

        //! Read from text stream.
        void readTextStream(QTextStream &textSTream, double tolerance);

    protected:

        //! Compute element normal.
        //! If possible true is returned otherwise false.
        bool getNormal ( unsigned int  elementID,
                         double       &nx,
                         double       &ny,
                         double       &nz) const;
};

#endif /* RML_MODEL_RAW_H */
