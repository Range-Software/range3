/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_sparse_matrix.h                                      *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   11-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Range sparse matrix class declaration               *
 *********************************************************************/

#ifndef RML_SPARSE_MATRIX_H
#define RML_SPARSE_MATRIX_H

#include <vector>

#include <rblib.h>

#include "rml_sparse_vector.h"

class RSparseMatrix
{

    protected:

        //! Vector of matrix rows.
        std::vector< RSparseVector<double> > data;

    private:

        //! Internal initialization function.
        void _init(const RSparseMatrix *pMatrix = nullptr);

    public:

        //! Constructor.
        RSparseMatrix();

        //! Copy constructor.
        RSparseMatrix(const RSparseMatrix &matrix);

        //! Destructor.
        ~RSparseMatrix();

        //! Assignment operator.
        RSparseMatrix & operator =(const RSparseMatrix &matrix);

        //! Return number of rows.
        uint getNRows(void) const;

        //! Set number of rows.
        void setNRows(uint nRows);

        //! Return number of columns for given row index.
        uint getNColumns(uint rowIndex) const;

        //! Set number of rows.
        void reserveNColumns(uint nColumns);

        //! Return value at given row index and column position.
        double getValue(uint rowIndex, uint columnPosition) const;

        //! Return vector of position indexes for given row index.
        std::vector<uint> getRowIndexes(uint rowIndex) const;

        //! Return const reference to sparse vector at given position.
        const RSparseVector<double> & getVector(uint rowIndex) const;

        //! Return reference to sparse vector at given position.
        RSparseVector<double> & getVector(uint rowIndex);

        //! Add value.
        //! If value with given row and column indexes already exist given value will be added to its current value.
        void addValue(uint rowIndex, uint columnIndex, double value);

        //! Find value at given row and column index. If value is not found 0.0 is returned.
        double findValue(uint rowIndex, uint columnIndex) const;

        //! Add matrix operation.
        void addMatrix(const RSparseMatrix &A);

        //! Clear all values.
        void clear(void);

        //! Find column position.
        bool findColumnPosition(uint rowIndex, uint columnIndex, uint &rowPosition) const;

        //! Find maximum column index.
        uint findMaxColumnIndex(void) const;

        //! Return euclidean norm of the matrix.
        double findNorm(void) const;

        //! Return trace (sum of diagonal elements) of the matrix.
        double findTrace(void) const;

        //! Print.
        void print(void) const;

        //! Matrix vector multiplication - y=A*x.
        static void mlt(const RSparseMatrix &A, const RRVector &x, RRVector &y);

};

#endif // RML_SPARSE_MATRIX_H
