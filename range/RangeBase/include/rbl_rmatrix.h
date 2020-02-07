/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_rmatrix.h                                            *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   21-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Real matrix class declaration                       *
 *********************************************************************/

#ifndef RBL_RMATRIX_H
#define RBL_RMATRIX_H

#include <QString>
#include <vector>

#include "rbl_rvector.h"

//! Real matrix class.
class RRMatrix
{

    protected:

        //! Vector of real vectors values.
        std::vector<RRVector> array;

    private:

        //! Internal initialization function.
        void _init(const RRMatrix *pMatrix = nullptr);

    public:

        //! Constructor.
        RRMatrix ();

        //! Constructor.
        RRMatrix(uint nRows,
                 uint nColumns,
                 double value = double());

        //! Copy constructor.
        RRMatrix(const RRMatrix &matrix);

        //! Destructor.
        ~RRMatrix ();

        //! Assignment operator.
        RRMatrix & operator =(const RRMatrix &matrix);

        //! Return number of rows.
        uint getNRows() const;

        //! Return number of coluns.
        uint getNColumns() const;

        //! Resize matrix array.
        void resize(uint nRows,
                    uint nColumns,
                    double value = double());

        //! Create identity matrix.
        //! This will change current matrix to square identity matrix.
        void setIdentity(uint nRows);

        //! Fill matrix with specified value.
        void fill(double value);

        //! Return value at given position.
        double getValue(uint row,
                        uint column) const;

        //! Return vector of RRVectors.
        std::vector<RRVector> getVectors() const;

        //! Set value at given position.
        void setValue(uint row,
                      uint column,
                      double value);

        //! Return true if matrix is square.
        bool isSquare() const;

        //! Transpose matrix.
        void transpose();

        //! Produce transposed matrix from A.
        void transpose(const RRMatrix &A);

        //! Invert matrix.
        void invert();

        //! LU decomposition.
        void decomposeToLU();

        //! Return matrix determinant.
        double getDeterminant() const;

        //! Return vector containing summed (lumped) rows.
        //! Equals to [A] * {1.0}.
        RRVector getSummedRows() const;

        //! Return sum of given row.
        double getSummedRow(uint row) const;

        //! Access operator - return const reference to RRVector.
        const RRVector & operator [](uint row) const;

        //! Access operator - return const reference to RRVector.
        RRVector & operator [](uint row);

        //! Multiplication / scale operator.
        void operator *=(double scaleValue);

        //! Equals operator.
        bool operator ==(const RRMatrix &array) const;

        //! Not-equals operator.
        bool operator !=(const RRMatrix &array) const;

        //! Return block defined by i(begin/end) and j(begin/end).
        RRMatrix getBlock(uint iBegin, uint iEnd, uint jBegin, uint jEnd) const;

        //! Set block starting at given position.
        void setBlock(const RRMatrix &matrix, uint iBegin, uint jBegin);

        //! Print content to string.
        QString toString() const;

        //! Print.
        void print() const;

        //! Clear matrix.
        void clear();

        //! Solve small equation system using LU decomposition.
        static void solveLU(const RRMatrix &A, const RRVector &x, RRVector &y);

        //! Euclidean norm.
        static double norm(const RRMatrix &M);

        //! Euclidean norm.
        static double norm(const RRMatrix &M, const RRVector &b);

        //! Trace (sum of diagonal elements).
        static double trace(const RRMatrix &M);

        //! Matrix vector multiplication - y=A*x.
        //! If add is true y won't be cleared.
        static void mlt(const RRMatrix &A, const RRVector &x, RRVector &y, bool add = false);

        //! Matrix matrix multiplication - Y=A*X.
        //! If add is true Y won't be cleared.
        static void mlt(const RRMatrix &A, const RRMatrix &X, RRMatrix &Y, bool add = false);

        //! Generate rotation matrix from three angles in radians.
        static RRMatrix generateRotationMatrix(double xAngle,double yAngle,double zAngle);

};

#endif /* RBL_RMATRIX_H */
