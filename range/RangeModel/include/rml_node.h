/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_node.h                                               *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   8-th December 2011                                       *
 *                                                                   *
 *  DESCRIPTION: Node class declaration                              *
 *********************************************************************/

#ifndef RML_NODE_H
#define RML_NODE_H

#include <rblib.h>

//! Node class
class RNode
{

    private:

        void _init ( const RNode *pNode = nullptr );

    protected:

        //! X coordinate
        double x;
        //! y coordinate
        double y;
        //! Z coordinate
        double z;

    public:

        //! Constructor
        RNode ();

        //! Constructor
        RNode ( double x,
                double y,
                double z );

        //! Constructor
        RNode ( const RR3Vector &vector );

        //! Copy constructor
        RNode ( const RNode &node );

        //! Destructor
        ~RNode ();

        //! Return x coordinate
        inline double getX ( void ) const
        {
            return this->x;
        }

        //! Return y coordinate
        inline double getY ( void ) const
        {
            return this->y;
        }

        //! Return z coordinate
        inline double getZ ( void ) const
        {
            return this->z;
        }

        //! Set x coordinate
        inline void setX ( double val )
        {
            this->x = val;
        }

        //! Set y coordinate
        inline void setY ( double val )
        {
            this->y = val;
        }

        //! Set z coordinate
        inline void setZ ( double val )
        {
            this->z = val;
        }

        //! Set all coordinate values
        inline void set ( double x,
                          double y,
                          double z )
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        //! Move node.
        inline void move ( const RR3Vector &delta )
        {
            this->x += delta[0];
            this->y += delta[1];
            this->z += delta[2];
        }

        //! Return distance to given node
        double getDistance ( const RNode &node ) const;

        //! Transform vector.
        void transform ( const RRMatrix &R, const RR3Vector &t = RR3Vector(0.0,0.0,0.0), bool translateFirst = false );

        //! Translate vector.
        void translate ( const RR3Vector &t );

        //! Scale node.
        void scale ( double scale );

        //! Scale node.
        void scale ( const RR3Vector &scaleVector );

        //! Assignment operator
        RNode & operator = ( const RNode &node );

        //! Equal operator.
        bool operator == ( const RNode &node ) const;

        //! Not-Equal operator.
        bool operator != ( const RNode &node ) const;

        //! Less than operator.
        bool operator < ( const RNode &node ) const;

        //! Convert coordinates to vector.
        RR3Vector toVector ( void ) const;

        //! Convert coordinates to vector.
        void toVector ( RR3Vector &vector ) const;

        //! Print node to standard output.
        void print ( void ) const;

        //! Allow RFileIO to access private members.
        friend class RFileIO;

};

#endif /* RML_NODE_H */
