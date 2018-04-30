/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_imatrix.cpp                                          *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   21-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Integer matrix class definition                     *
 *********************************************************************/

#include "rbl_imatrix.h"
#include "rbl_error.h"


RIMatrix::RIMatrix ()
{
    this->_init();
} /* RIMatrix::RIMatrix */


RIMatrix::RIMatrix (unsigned int nRows,
                    unsigned int nColumns, int value)
{
    this->_init();
    this->resize(nRows,nColumns,value);
} /* RIMatrix::RIMatrix */


RIMatrix::RIMatrix (const RIMatrix &matrix)
{
    this->_init(&matrix);
} /* RIMatrix::RIMatrix (copy) */


RIMatrix::~RIMatrix ()
{
} /* RIMatrix::~RIMatrix */


RIMatrix & RIMatrix::operator = (const RIMatrix &matrix)
{
    this->_init(&matrix);
    return (*this);
} /* RIMatrix::operator = */


void RIMatrix::_init (const RIMatrix *pMatrix)
{
    if (pMatrix)
    {
        this->resize(pMatrix->getNRows(),pMatrix->getNColumns());
        for (unsigned int i=0;i<pMatrix->getNRows();i++)
        {
            for (unsigned int j=0;j<pMatrix->getNColumns();j++)
            {
                this->setValue(i,j,pMatrix->getValue(i,j));
            }
        }
    }
} /* RIMatrix::_init */


unsigned int RIMatrix::getNRows (void) const
{
    return (unsigned int)this->array.size();
} /* RIMatrix::getNRows */


unsigned int RIMatrix::getNColumns (void) const
{
    if (this->getNRows() > 0)
    {
        return (unsigned int)this->array[0].size();
    }
    else
    {
        return 0;
    }
} /* RIMatrix::getNColumns */


void RIMatrix::resize (unsigned int nRows, unsigned int nColumns, int value)
{
    this->array.resize(nRows);
    for (unsigned int i=0;i<nRows;i++)
    {
        this->array[i].resize(nColumns,value);
    }
} /* RIMatrix::resize */


int RIMatrix::getValue(unsigned int row, unsigned int column) const
{
    R_ERROR_ASSERT(row < this->getNRows());
    R_ERROR_ASSERT(column < this->getNColumns());

    return this->array[row][column];
} /* RIMatrix::getValue */


void RIMatrix::setValue(unsigned int row, unsigned int column, int value)
{
    R_ERROR_ASSERT(row < this->getNRows());
    R_ERROR_ASSERT(column < this->getNColumns());

    this->array[row][column] = value;
} /* RIMatrix::setValue */


void RIMatrix::transpose (void)
{
    unsigned int nr = this->getNRows();
    unsigned int nc = this->getNColumns();

    if (nr == nc)
    {
        int itmp;
        for (unsigned int i=0;i<nr;i++)
        {
            for (unsigned int j=0;j<nc;j++)
            {
                if (i == j)
                {
                    continue;
                }
                itmp = this->getValue(j,i);
                this->setValue(j,i,this->getValue(i,j));
                this->setValue(i,j,itmp);
            }
        }
    }
    else
    {
        RIMatrix itmp(*this);

        this->resize(nc,nr);
        for (unsigned int i=0;i<nr;i++)
        {
            for (unsigned int j=0;j<nc;j++)
            {
                if (i == j)
                {
                    continue;
                }
                this->setValue(j,i,itmp.getValue(i,j));
            }
        }
    }
} /* RIMatrix::transpose */


const RIVector & RIMatrix::operator [] (unsigned int row) const
{
    R_ERROR_ASSERT (row < this->getNRows());

    return this->array[row];
} /* RIMatrix::operator [] */


RIVector & RIMatrix::operator [] (unsigned int row)
{
    R_ERROR_ASSERT (row < this->getNRows());

    return this->array[row];
} /* RIMatrix::operator [] */


void RIMatrix::clear (void)
{
    for (unsigned int i=0;i<this->getNRows();i++)
    {
        this->array[i].clear();
    }
    this->array.clear();
} /* RIMatrix::clear */
