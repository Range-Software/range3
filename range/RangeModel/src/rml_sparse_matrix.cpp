/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_sparse_matrix.cpp                                    *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   11-th December 2012                                      *
 *                                                                   *
 *  DESCRIPTION: Range sparse matrix class definition                *
 *********************************************************************/

#include <rblib.h>

#include "rml_sparse_matrix.h"

void RSparseMatrix::_init(const RSparseMatrix *pMatrix)
{
    if (pMatrix)
    {
        this->data = pMatrix->data;
    }
}

RSparseMatrix::RSparseMatrix()
{
    this->_init();
}

RSparseMatrix::RSparseMatrix(const RSparseMatrix &matrix)
{
    this->_init(&matrix);
}

RSparseMatrix::~RSparseMatrix()
{
}

RSparseMatrix &RSparseMatrix::operator =(const RSparseMatrix &matrix)
{
    this->_init(&matrix);
    return (*this);
}

const RSparseVector<double> &RSparseMatrix::getVector(uint rowIndex) const
{
    return this->data.at(rowIndex);
}

RSparseVector<double> &RSparseMatrix::getVector(uint rowIndex)
{
    return this->data.at(rowIndex);
}

void RSparseMatrix::addMatrix(const RSparseMatrix &A)
{
    R_ERROR_ASSERT(this->getNRows() == A.getNRows());

    for (uint i=0;i<this->getNRows();i++)
    {
        this->data.at(i).addVector(A.getVector(i));
    }
}

uint RSparseMatrix::getNRows(void) const
{
    return uint(this->data.size());
}

void RSparseMatrix::setNRows(uint nRows)
{
    this->data.resize(nRows);
}

uint RSparseMatrix::getNColumns(uint rowIndex) const
{
    return uint(this->data[rowIndex].size());
}

void RSparseMatrix::reserveNColumns(uint nColumns)
{
    for (uint i=0;i<this->data.size();i++)
    {
        this->data[i].reserve(nColumns);
    }
}

double RSparseMatrix::getValue(uint rowIndex, uint columnPosition) const
{
    return this->data[rowIndex].getValue(columnPosition);
}

std::vector<uint> RSparseMatrix::getRowIndexes(uint rowIndex) const
{
    return this->data[rowIndex].getIndexes();
}

void RSparseMatrix::addValue(uint rowIndex, uint columnIndex, double value)
{
    if (rowIndex >= this->data.size())
    {
        this->data.resize(rowIndex + 1);
    }
    this->data[rowIndex].addValue(columnIndex,value);
}

double RSparseMatrix::findValue(uint rowIndex, uint columnIndex) const
{
    uint pos = 0;
    if (this->findColumnPosition(rowIndex,columnIndex,pos))
    {
        return this->getValue(rowIndex,pos);
    }
    return 0.0;
}

void RSparseMatrix::clear(void)
{
    for (uint i=0;i<this->data.size();i++)
    {
        this->data[i].clear();
    }
}

bool RSparseMatrix::findColumnPosition(uint rowIndex, uint columnIndex, uint &rowPosition) const
{
    std::vector<uint> columnIndexes = this->getRowIndexes(rowIndex);
    for (uint i=0;i<columnIndexes.size();i++)
    {
        if (columnIndexes[i] == columnIndex)
        {
            rowPosition = i;
            return true;
        }
    }
    return false;
}

uint RSparseMatrix::findMaxColumnIndex(void) const
{
    uint maxColumnIndex = 0;

    for (uint i=0;i<this->getNRows();i++)
    {
        for (uint j=0;j<this->data[i].size();j++)
        {
            maxColumnIndex = std::max(maxColumnIndex,this->data[i].getIndex(j));
        }
    }

    return maxColumnIndex;
}

double RSparseMatrix::findNorm(void) const
{
    RRVector v(this->getNRows());
    v.fill(0.0);

    for (uint i=0;i<this->getNRows();i++)
    {
        for (uint j=0;j<this->getNColumns(i);j++)
        {
            v[i] += this->getValue(i,j);
        }
    }

    return RRVector::norm(v);
}

double RSparseMatrix::findTrace(void) const
{
    double traceValue = 0.0;
    for (uint i=0;i<this->getNRows();i++)
    {
        uint position = 0;
        if (this->findColumnPosition(i,i,position))
        {
            traceValue += this->getValue(i,position);
        }
    }
    return traceValue;
}

void RSparseMatrix::print(void) const
{
    uint nc = this->findMaxColumnIndex() + 1;

    RLogger::info("Matrix - sparse: [%ux%u]\n",this->getNRows(),nc);
    for (uint i=0;i<this->getNRows();i++)
    {
        for (uint j=0;j<nc;j++)
        {
            double value = 0.0;
            unsigned rp = 0;
            if (this->findColumnPosition(i,j,rp))
            {
                value = this->getValue(i,rp);
            }
            RLogger::info("%15.6e",value);
        }
        RLogger::info("\n");
    }
}

void RSparseMatrix::mlt(const RSparseMatrix &A, const RRVector &x, RRVector &y)
{
    y.resize(A.getNRows(),0.0);

    for (uint i=0;i<A.getNRows();i++)
    {
        std::vector<uint> index = A.getRowIndexes(i);
        for (uint j=0;j<index.size();j++)
        {
            y[i] += A.getValue(i,j) * x[j];
        }
    }
}
