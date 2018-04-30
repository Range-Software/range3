/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rmatrixpreconditioner.cpp                                *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   23-rd January 2013                                       *
 *                                                                   *
 *  DESCRIPTION: Matrix preconditioner class definition              *
 *********************************************************************/

#include <cmath>

#include <omp.h>

#include "rmatrixpreconditioner.h"

void RMatrixPreconditioner::_init(const RMatrixPreconditioner *pMatrixPreconditioner)
{
    if (pMatrixPreconditioner)
    {
        this->matrixPreconditionerType = pMatrixPreconditioner->matrixPreconditionerType;
        this->data = pMatrixPreconditioner->data;
    }
}

RMatrixPreconditioner::RMatrixPreconditioner(const RSparseMatrix &matrix, RMatrixPreconditionerType matrixPreconditionerType, unsigned int blockSize)
    : matrixPreconditionerType(matrixPreconditionerType)
{
    this->_init();

    switch (matrixPreconditionerType)
    {
        case R_MATRIX_PRECONDITIONER_JACOBI:
            this->constructJacobi(matrix);
            break;
        case R_MATRIX_PRECONDITIONER_BLOCK_JACOBI:
            this->constructBlockJacobi(matrix,blockSize);
            break;
        default:
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Invalid matrix preconditioner type \'%d\'",matrixPreconditionerType);
    }
}

RMatrixPreconditioner::RMatrixPreconditioner(const RMatrixPreconditioner &matrixPreconditioner)
{
    this->_init(&matrixPreconditioner);
}

RMatrixPreconditioner::~RMatrixPreconditioner()
{
}

RMatrixPreconditioner &RMatrixPreconditioner::operator =(const RMatrixPreconditioner &matrixPreconditioner)
{
    this->_init(&matrixPreconditioner);
    return (*this);
}

void RMatrixPreconditioner::compute(const RRVector &x, RRVector &y) const
{
    switch (matrixPreconditionerType)
    {
        case R_MATRIX_PRECONDITIONER_JACOBI:
            this->computeJacobi(x,y);
            break;
        case R_MATRIX_PRECONDITIONER_BLOCK_JACOBI:
            this->computeBlockJacobi(x,y);
            break;
        default:
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Invalid matrix preconditioner type \'%d\'",matrixPreconditionerType);
    }
}

void RMatrixPreconditioner::constructJacobi(const RSparseMatrix &matrix)
{
    unsigned int nRows = matrix.getNRows();

    this->data.resize(nRows,1);
    this->data.fill(0.0);

    for (unsigned int i=0;i<nRows;i++)
    {
        unsigned int columnPosition = 0;
        if (matrix.findColumnPosition(i,i,columnPosition))
        {
            this->data[i][0] = matrix.getValue(i,columnPosition);
        }
    }
}

void RMatrixPreconditioner::constructBlockJacobi(const RSparseMatrix &matrix, unsigned int blockSize)
{
    unsigned int width = blockSize;

    // Matrix mus be dividable by block size.
    unsigned int nRows = matrix.getNRows();
    while (nRows % width != 0)
    {
        width --;
    }

    this->data.resize(nRows,width,0.0);

    unsigned int nBlocks = nRows/width;

    for (unsigned int i=0;i<nBlocks;i++)
    {
        for (unsigned int k=0;k<width;k++)
        {
            unsigned int m = i*width + k;
            for (unsigned int l=0;l<width;l++)
            {
                unsigned int n = i*width + l;
                unsigned int columnPosition = 0;
                if (matrix.findColumnPosition(m,n,columnPosition))
                {
                    this->data[m][l] = matrix.getValue(m,columnPosition);
                }
            }
        }
    }
}

void RMatrixPreconditioner::computeJacobi(const RRVector &x, RRVector &y) const
{
    unsigned int nRows = this->data.getNRows();

    y.resize(nRows);

    for (unsigned int i=0;i<nRows;i++)
    {
        y[i] = (this->data[i][0] == 0.0) ? 0.0 : x[i] / this->data[i][0];
    }
}

void RMatrixPreconditioner::computeBlockJacobi(const RRVector &x, RRVector &y) const
{
    unsigned int nRows = this->data.getNRows();
    unsigned int width = this->data.getNColumns();

    y.resize(nRows);

    RRMatrix As(width,width);
    RRVector xs(width);
    RRVector ys(width);

    unsigned int nBlocks = nRows/width;

    for (unsigned int i=0;i<nBlocks;i++)
    {
        for (unsigned int k=0;k<width;k++)
        {
            unsigned int m = i*width + k;
            for (unsigned int l=0;l<width;l++)
            {
                As[k][l] = this->data[m][l];
            }
            xs[k] = x[m];
        }

        RRMatrix::solveLU(As,xs,ys);

        for (unsigned int k=0;k<width;k++)
        {
            unsigned int m = i*width + k;
            y[m] = ys[k];
        }
    }
}
