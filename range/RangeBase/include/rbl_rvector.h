/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_rvector.h                                            *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   21-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Real vector class declaration                       *
 *********************************************************************/

#ifndef RBL_RVECTOR_H
#define RBL_RVECTOR_H

#include <QString>
#include <vector>

//! Vector class.
class RRVector : public std::vector<double>
{

    private:

        //! Internal initialization function.
        void _init(const RRVector *pArray = nullptr);

    public:

        //! Constructor.
        RRVector(uint nRows = 0, double value = double());

        //! Copy constructor.
        RRVector(const RRVector &array);

        //! Copy constructor.
        RRVector(const std::vector<double> &array);

        //! Position vector constructor.
        RRVector(double x, double y, double z);

        //! Destructor.
        ~RRVector ();

        //! Assignment operator.
        RRVector & operator =(const RRVector &array);

        //! Return number of rows (size of the vector).
        inline uint getNRows() const
        {
            return uint(this->size());
        }

        //! Fill vector with specified value.
        void fill(double value);

        //! Append vector.
        void append(const RRVector &array);

        //! Access element operator.
        double & operator[](uint n);

        //! Access element operator.
        const double & operator[](uint n) const;

        //! Multiplication / scale operator.
        void operator *=(double scaleValue);

        //! Equals operator.
        bool operator ==(const RRVector &array) const;

        //! Not-equals operator.
        bool operator !=(const RRVector &array) const;

        //! Return vector length (Euclidean norm).
        double length() const;

        //! Normalize vector and return its previous length.
        double normalize();

        //! Scale vector.
        void scale(double scale);

        //! Print content to string.
        QString toString(bool oneLine = false) const;

        //! Print content.
        void print(bool oneLine = false, bool newLine = false) const;

        //! Dot product of two vectors.
        static double dot(const RRVector &v1,
                          const RRVector &v2);

        //! Add vector to vector.
        static void add(const RRVector &v1,
                        const RRVector &v2,
                        RRVector &x);

        //! Subtract vector v2 from vector v1.
        static void subtract(const RRVector &v1,
                             const RRVector &v2,
                             RRVector &x);

        //! Euclidean norm.
        static double norm(const RRVector &v);

};

#endif /* RBL_RVECTOR_H */
