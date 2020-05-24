/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_r3vector.h                                           *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   15-th February 2013                                      *
 *                                                                   *
 *  DESCRIPTION: Real vector[3] class declaration                    *
 *********************************************************************/

#ifndef RBL_R3VECTOR_H
#define RBL_R3VECTOR_H

#include "rbl_rvector.h"
#include "rbl_rmatrix.h"

class RR3Vector : public RRVector
{

    private:

        //! Internal initialization function.
        void _init ( const RR3Vector *pR3Vector = nullptr );

    public:

        //! Constructor.
        RR3Vector ();

        //! Copy constructor.
        RR3Vector ( const RR3Vector &r3Vector );

        //! Position vector constructor.
        RR3Vector ( double x, double y, double z );

        //! Destructor.
        ~RR3Vector ();

        //! Assignment operator.
        RR3Vector & operator = ( const RR3Vector &r3Vector );

        //! Calculate rotation matrix from local to global coordinates.
        //! Vector is assumed to be in global coordinates.
        //! To get matrix from global to local just invert R matrix.
        void findRotationMatrix ( RRMatrix &R ) const;

        //! Return opposite vector.
        RR3Vector getOpposite() const;

        //! Find orthogonal vector.
        RR3Vector findOrthogonal() const;

        //! Clear vector.
        void clear();

        static double findDistance(const RR3Vector &v1,const RR3Vector &v2);

        //! Cross product of two vectors.
        static void cross(const RR3Vector &v1,
                          const RR3Vector &v2,
                          RR3Vector       &vp);

        //! Check if vectors are parallel and return number of parallel dimensions.
        static bool areParallel(const RR3Vector &v1,
                                const RR3Vector &v2);

        //! Angle between two vectors (in radians).
        static double angle(const RR3Vector &v1,
                            const RR3Vector &v2);

};

#endif // RBL_R3VECTOR_H
