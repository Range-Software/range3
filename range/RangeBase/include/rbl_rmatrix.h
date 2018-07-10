/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_rmatrix.h                                            *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   21-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Real matrix class declaration                       *
 *********************************************************************/

#ifndef __RBL_RMATRIX_H__
#define __RBL_RMATRIX_H__

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
        void _init ( const RRMatrix *pMatrix = 0 );

    public:

        //! Constructor.
        RRMatrix ();

        //! Constructor.
        RRMatrix ( unsigned int nRows,
                   unsigned int nColumns,
                   double value = double() );

        //! Copy constructor.
        RRMatrix ( const RRMatrix &matrix );

        //! Destructor.
        ~RRMatrix ();

        //! Assignment operator.
        RRMatrix & operator = ( const RRMatrix &matrix );

        //! Return number of rows.
        unsigned int getNRows (void) const;

        //! Return number of coluns.
        unsigned int getNColumns (void) const;

        //! Resize matrix array.
        void resize ( unsigned int nRows,
                      unsigned int nColumns,
                      double       value = double() );

        //! Create identity matrix.
        //! This will change current matrix to square identity matrix.
        void setIdentity ( unsigned int nRows );

        //! Fill matrix with specified value.
        void fill ( double value );

        //! Return value at given position.
        double getValue ( unsigned int row,
                          unsigned int column ) const;

        //! Return vector of RRVectors.
        std::vector<RRVector> getVectors(void) const;

        //! Set value at given position.
        void setValue ( unsigned int row,
                        unsigned int column,
                        double       value );

        //! Return true if matrix is square.
        bool isSquare( void ) const;

        //! Transpose matrix.
        void transpose ( void );

        //! Produce transposed matrix from A.
        void transpose ( const RRMatrix &A );

        //! Invert matrix.
        void invert ( void );

        //! LU decomposition.
        void decomposeToLU ( void );

        //! Return matrix determinant.
        double getDeterminant( void ) const;

        //! Return vector containing summed (lumped) rows.
        //! Equals to [A] * {1.0}.
        RRVector getSummedRows(void) const;

        //! Return sum of given row.
        double getSummedRow(unsigned int row) const;

        //! Access operator - return const reference to RRVector.
        const RRVector & operator [] (unsigned int row) const;

        //! Access operator - return const reference to RRVector.
        RRVector & operator [] (unsigned int row);

        //! Multiplication / scale operator.
        void operator *= ( double scaleValue );

        //! Return block defined by i(begin/end) and j(begin/end).
        RRMatrix getBlock(uint iBegin, uint iEnd, uint jBegin, uint jEnd) const;

        //! Set block starting at given position.
        void setBlock(const RRMatrix &matrix, uint iBegin, uint jBegin);

        //! Print content to string.
        QString toString ( void ) const;

        //! Print.
        void print(void) const;

        //! Clear matrix.
        void clear (void);

        //! Solve small equation system using LU decomposition.
        static void solveLU ( const RRMatrix &A, const RRVector &x, RRVector &y );

        //! Euclidean norm.
        static double norm ( const RRMatrix &M );

        //! Euclidean norm.
        static double norm ( const RRMatrix &M, const RRVector &b );

        //! Trace (sum of diagonal elements).
        static double trace ( const RRMatrix &M );

        //! Matrix vector multiplication - y=A*x.
        //! If add is true y won't be cleared.
        static void mlt(const RRMatrix &A, const RRVector &x, RRVector &y, bool add = false);

        //! Matrix matrix multiplication - Y=A*X.
        //! If add is true Y won't be cleared.
        static void mlt(const RRMatrix &A, const RRMatrix &X, RRMatrix &Y, bool add = false);

        //! Generate rotation matrix from three angles in radians.
        static RRMatrix generateRotationMatrix(double xAngle,double yAngle,double zAngle);

};

#endif /* __RBL_RMATRIX_H__ */
