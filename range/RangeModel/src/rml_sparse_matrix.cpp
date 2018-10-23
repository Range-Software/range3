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

unsigned int RSparseMatrix::getNRows(void) const
{
    return (unsigned int)this->data.size();
}

void RSparseMatrix::setNRows(unsigned int nRows)
{
    this->data.resize(nRows);
}

unsigned int RSparseMatrix::getNColumns(unsigned int rowIndex) const
{
    return (unsigned int)this->data[rowIndex].size();
}

void RSparseMatrix::reserveNColumns(unsigned int nColumns)
{
    for (uint i=0;i<this->data.size();i++)
    {
        this->data[i].reserve(nColumns);
    }
}

double RSparseMatrix::getValue(unsigned int rowIndex, unsigned int columnPosition) const
{
    return this->data[rowIndex].getValue(columnPosition);
}

std::vector<unsigned int> RSparseMatrix::getRowIndexes(unsigned int rowIndex) const
{
    return this->data[rowIndex].getIndexes();
}

void RSparseMatrix::addValue(unsigned int rowIndex, unsigned int columnIndex, double value)
{
    if (rowIndex >= this->data.size())
    {
        this->data.resize(rowIndex + 1);
    }
    this->data[rowIndex].addValue(columnIndex,value);
}

double RSparseMatrix::findValue(unsigned int rowIndex, unsigned int columnIndex) const
{
    unsigned int pos = 0;
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

bool RSparseMatrix::findColumnPosition(unsigned int rowIndex, unsigned int columnIndex, unsigned int &rowPosition) const
{
    std::vector<unsigned int> columnIndexes = this->getRowIndexes(rowIndex);
    for (unsigned int i=0;i<columnIndexes.size();i++)
    {
        if (columnIndexes[i] == columnIndex)
        {
            rowPosition = i;
            return true;
        }
    }
    return false;
}

unsigned int RSparseMatrix::findMaxColumnIndex(void) const
{
    unsigned int maxColumnIndex = 0;

    for (unsigned int i=0;i<this->getNRows();i++)
    {
        for (unsigned int j=0;j<this->data[i].size();j++)
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

    for (unsigned int i=0;i<this->getNRows();i++)
    {
        for (unsigned int j=0;j<this->getNColumns(i);j++)
        {
            v[i] += this->getValue(i,j);
        }
    }

    return RRVector::norm(v);
}

double RSparseMatrix::findTrace(void) const
{
    double traceValue = 0.0;
    for (unsigned int i=0;i<this->getNRows();i++)
    {
        unsigned int position = 0;
        if (this->findColumnPosition(i,i,position))
        {
            traceValue += this->getValue(i,position);
        }
    }
    return traceValue;
}

void RSparseMatrix::print(void) const
{
    unsigned int nc = this->findMaxColumnIndex() + 1;

    RLogger::info("Matrix - sparse: [%ux%u]\n",this->getNRows(),nc);
    for (unsigned int i=0;i<this->getNRows();i++)
    {
        for (unsigned int j=0;j<nc;j++)
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
        std::vector<unsigned int> index = A.getRowIndexes(i);
        for (uint j=0;j<index.size();j++)
        {
            y[i] += A.getValue(i,j) * x[j];
        }
    }
}
