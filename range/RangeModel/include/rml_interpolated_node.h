/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_interpolated_node.h                                  *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   31-st May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Interpolated node class declaration                 *
 *********************************************************************/

#ifndef RML_INTERPOLATED_NODE_H
#define RML_INTERPOLATED_NODE_H

#include <vector>
#include <rblib.h>

#include "rml_node.h"

class RInterpolatedNode : public RNode
{

    protected:

        //! Element ID.
        unsigned int elementID;

    private:

        //! Internal initialization function.
        void _init ( const RInterpolatedNode *pInterpolatedNode = nullptr );

    public:

        //! Constructor.
        RInterpolatedNode ( unsigned int elementID = 0 );

        //! Constructor.
        RInterpolatedNode ( unsigned int elementID,
                            double x,
                            double y,
                            double z );

        //! Constructor.
        RInterpolatedNode ( unsigned int elementID,
                            const RR3Vector &vector );

        //! Copy constructor.
        RInterpolatedNode ( const RInterpolatedNode &interpolatedNode );

        //! Destructor.
        ~RInterpolatedNode ();

        //! Assignment operator.
        RInterpolatedNode & operator = ( const RInterpolatedNode &interpolatedNode );

        //! Move node.
        inline void move ( const RR3Vector &delta )
        {
            this->x += delta[0];
            this->y += delta[1];
            this->z += delta[2];
        }

        //! Return element ID.
        unsigned int getElementID ( void ) const;

        //! Set element ID.
        void setElementID ( unsigned int elementID );

};

#endif /* RML_INTERPOLATED_NODE_H */
