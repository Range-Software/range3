/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_sparse_matrix.h                                      *
 *  GROUP:  RML                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   11-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Range sparse matrix class declaration               *
 *********************************************************************/

#ifndef __RML_SPARSE_MATRIX_H__
#define __RML_SPARSE_MATRIX_H__

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
        void _init(const RSparseMatrix *pMatrix = 0);

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
        unsigned int getNRows(void) const;

        //! Set number of rows.
        void setNRows(unsigned int nRows);

        //! Return number of columns for given row index.
        unsigned int getNColumns(unsigned int rowIndex) const;

        //! Return value at given row index and column position.
        double getValue(unsigned int rowIndex, unsigned int columnPosition) const;

        //! Return vector of position indexes for given row index.
        std::vector<unsigned int> getRowIndexes(unsigned int rowIndex) const;

        //! Add value.
        //! If value with given row and column indexes already exist given value will be added to its current value.
        void addValue(unsigned int rowIndex, unsigned int columnIndex, double value);

        //! Find value at given row and column index. If value is not found 0.0 is returned.
        double findValue(unsigned int rowIndex, unsigned int columnIndex) const;

        //! Clear all values.
        void clear(void);

        //! Find column position.
        bool findColumnPosition(unsigned int rowIndex, unsigned int columnIndex, unsigned int &rowPosition) const;

        //! Find maximum column index.
        unsigned int findMaxColumnIndex(void) const;

        //! Return euclidean norm of the matrix.
        double findNorm(void) const;

        //! Return trace (sum of diagonal elements) of the matrix.
        double findTrace(void) const;

        //! Print.
        void print(void) const;

        //! Matrix vector multiplication - y=A*x.
        static void mlt(const RSparseMatrix &A, const RRVector &x, RRVector &y);

};

#endif // __RML_SPARSE_MATRIX_H__
