/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rmatrixpreconditioner.h                                  *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   23-rd January 2013                                       *
 *                                                                   *
 *  DESCRIPTION: Matrix preconditioner class declaration             *
 *********************************************************************/

#ifndef RMATRIXPRECONDITIONER_H
#define RMATRIXPRECONDITIONER_H

#include <rblib.h>
#include <rmlib.h>

typedef enum _RMatrixPreconditionerType
{
    R_MATRIX_PRECONDITIONER_NONE = 0,
    R_MATRIX_PRECONDITIONER_JACOBI,
    R_MATRIX_PRECONDITIONER_BLOCK_JACOBI,
//    R_MATRIX_PRECONDITIONER_SSOR,
//    R_MATRIX_PRECONDITIONER_ILU,
//    R_MATRIX_PRECONDITIONER_DILU,
    R_MATRIX_PRECONDITIONER_N_TYPES
} RMatrixPreconditionerType;

class RMatrixPreconditioner
{

    protected:

        //! Matrix preconditioner type.
        RMatrixPreconditionerType matrixPreconditionerType;
        //! Preconditioner values.
        RRMatrix data;

    private:

        //! Internal initialization function.
        void _init(const RMatrixPreconditioner *pMatrixPreconditioner = nullptr);

    public:

        //! Constructor.
        RMatrixPreconditioner(const RSparseMatrix &matrix, RMatrixPreconditionerType matrixPreconditionerType = R_MATRIX_PRECONDITIONER_NONE, unsigned int blockSize = 1 );

        //! Copy constructor.
        RMatrixPreconditioner(const RMatrixPreconditioner &matrixPreconditioner);

        //! Destructor.
        ~RMatrixPreconditioner();

        //! Assignment operator.
        RMatrixPreconditioner & operator =(const RMatrixPreconditioner &matrixPreconditioner);

        //! Compute preconditioner equation system.
        void compute(const RRVector &x, RRVector &y) const;

    protected:

        //! Construct Jacobi preconditioner.
        void constructJacobi(const RSparseMatrix &matrix);

        //! Construct Block Jacobi preconditioner.
        void constructBlockJacobi(const RSparseMatrix &matrix, unsigned int blockSize);

        //! Compute Jacobi equation system.
        void computeJacobi(const RRVector &x, RRVector &y) const;

        //! Construct Block Jacobi equation system.
        void computeBlockJacobi(const RRVector &x, RRVector &y) const;

};

#endif // RMATRIXPRECONDITIONER_H
