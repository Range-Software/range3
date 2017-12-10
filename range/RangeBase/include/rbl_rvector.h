/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_rvector.h                                            *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   21-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Real vector class declaration                       *
 *********************************************************************/

#ifndef __RBL_RVECTOR_H__
#define __RBL_RVECTOR_H__

#include <QString>
#include <vector>

//! Vector class.
class RRVector : public std::vector<double>
{

    private:

        //! Internal initialization function.
        void _init ( const RRVector *pArray = 0 );

    public:

        //! Constructor.
        RRVector ( unsigned int nRows = 0, double value = double() );

        //! Copy constructor.
        RRVector ( const RRVector &array );

        //! Copy constructor.
        RRVector ( const std::vector<double> &array );

        //! Position vector constructor.
        RRVector ( double x, double y, double z );

        //! Destructor.
        ~RRVector ();

        //! Assignment operator.
        RRVector & operator = ( const RRVector &array );

        //! Return number of rows (size of the vector).
        inline unsigned int getNRows ( void ) const
        {
            return (unsigned int)this->size();
        }

        //! Fill vector with specified value.
        void fill ( double value );

        //! Append vector.
        void append ( const RRVector &array );

        //! Access element operator.
        double & operator[] ( unsigned int n );

        //! Access element operator.
        const double & operator[] ( unsigned int n ) const;

        //! Multiplication / scale operator.
        void operator *= ( double scaleValue );

        //! Return vector length (Euclidean norm).
        double length (void) const;

        //! Normalize vector and return its previous length.
        double normalize ( void );

        //! Scale vector.
        void scale ( double scale );

        //! Print content to string.
        QString toString ( bool oneLine = false ) const;

        //! Print content.
        void print ( bool oneLine = false, bool newLine = false ) const;

        //! Cross product of two vectors.
        static void cross ( const RRVector &v1,
                            const RRVector &v2,
                            RRVector       &vp );

        //! Dot product of two vectors.
        static double dot ( const RRVector &v1,
                            const RRVector &v2 );

        //! Add vector to vector.
        static void add ( const RRVector &v1,
                          const RRVector &v2,
                          RRVector &x );

        //! Subtract vector v2 from vector v1.
        static void subtract ( const RRVector &v1,
                               const RRVector &v2,
                               RRVector &x );

        //! Angle between two vectors (in radians).
        static double angle ( const RRVector &v1,
                              const RRVector &v2 );

        //! Euclidean norm.
        static double norm ( const RRVector &v );

        //! Check if vectors are parallel and return number of parallel dimensions.
        static bool areParallel ( const RRVector &v1,
                                  const RRVector &v2 );

};

#endif /* __RBL_RVECTOR_H__ */
