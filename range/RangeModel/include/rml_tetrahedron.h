/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_tetrahedron.h                                        *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   11-th June 2013                                          *
 *                                                                   *
 *  DESCRIPTION: Tetrahedron class declaration                       *
 *********************************************************************/


#ifndef RML_TETRAHEDRON_H
#define RML_TETRAHEDRON_H

#include <rblib.h>

#include "rml_node.h"

class RTetrahedron
{

    protected:

        RNode node1;
        RNode node2;
        RNode node3;
        RNode node4;

    private:

        //! Internal initialization function.
        void _init ( const RTetrahedron *pTetrahedron = nullptr );

    public:

        //! Constructor.
        explicit RTetrahedron ( const RNode &node1, const RNode &node2, const RNode &node3, const RNode &node4 );

        //! Copy constructor.
        RTetrahedron ( const RTetrahedron &tetrahedron );

        //! Destructor.
        ~RTetrahedron ();

        //! Assignment operator.
        RTetrahedron & operator = ( const RTetrahedron &tetrahedron );

        //! Set tetrahedrons nodes.
        void setNodes ( const RNode &node1, const RNode &node2, const RNode &node3, const RNode &node4 );

        //! Calculate tetrahedron volume.
        double findVolume ( void ) const;

        //! Calculate tetrahedron volume.
        static double findVolume ( const RNode &node1, const RNode &node2, const RNode &node3, const RNode &node4 );

        //! Calculate tetrahedron volume.
        static double findVolume ( double xi, double yi, double zi,
                                   double xj, double yj, double zj,
                                   double xk, double yk, double zk,
                                   double xl, double yl, double zl );

        //! Return tetrahedrons first node.
        inline const RNode & getNode1 ( void ) const
        {
            return this->node1;
        }

        //! Return tetrahedrons second node.
        inline const RNode & getNode2 ( void ) const
        {
            return this->node2;
        }

        //! Return tetrahedrons third node.
        inline const RNode & getNode3 ( void ) const
        {
            return this->node3;
        }

        //! Return tetrahedrons fourth node.
        inline const RNode & getNode4 ( void ) const
        {
            return this->node4;
        }

};

#endif /* RML_TETRAHEDRON_H */
