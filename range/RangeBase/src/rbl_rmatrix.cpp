/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_rmatrix.cpp                                          *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   21-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Real matrix class definition                        *
 *********************************************************************/

#include <cmath>

#include "rbl_rmatrix.h"
#include "rbl_error.h"
#include "rbl_utils.h"
#include "rbl_logger.h"


RRMatrix::RRMatrix ()
{
    this->_init();
} /* RRMatrix::RRMatrix */


RRMatrix::RRMatrix (unsigned int nRows,
                    unsigned int nColumns,
                    double value)
{
    this->_init();
    this->resize(nRows,nColumns,value);
} /* RRMatrix::RRMatrix */


RRMatrix::RRMatrix (const RRMatrix &matrix)
{
    this->_init(&matrix);
} /* RRMatrix::RRMatrix (copy) */


RRMatrix::~RRMatrix ()
{
} /* RRMatrix::~RRMatrix */


RRMatrix & RRMatrix::operator = (const RRMatrix &matrix)
{
    this->_init(&matrix);
    return (*this);
} /* RRMatrix::operator = */


void RRMatrix::_init (const RRMatrix *pMatrix)
{
    if (pMatrix)
    {
        this->array = pMatrix->array;
    }
} /* RRMatrix::_init */


unsigned int RRMatrix::getNRows (void) const
{
    return (unsigned int)this->array.size();
} /* RRMatrix::getNRows */


unsigned int RRMatrix::getNColumns (void) const
{
    if (this->getNRows() > 0)
    {
        return (unsigned int)this->array[0].size();
    }
    else
    {
        return 0;
    }
} /* RRMatrix::getNColumns */


void RRMatrix::resize (unsigned int nRows, unsigned int nColumns, double value)
{
    this->array.resize(nRows);
    for (unsigned int i=0;i<nRows;i++)
    {
        this->array[i].resize(nColumns,value);
    }
} /* RRMatrix::resize */


void RRMatrix::setIdentity(unsigned int nRows)
{
    this->resize(nRows,nRows,0);
    this->fill(0.0);
    for (unsigned int i=0;i<nRows;i++)
    {
        this->setValue(i,i,1.0);
    }
} /* RRMatrix::setIdentity */


void RRMatrix::fill(double value)
{
    for (unsigned int i=0;i<this->getNRows();i++)
    {
        this->operator [](i).fill(value);
    }
} /* RRMatrix::fill */


double RRMatrix::getValue(unsigned int row, unsigned int column) const
{
    R_ERROR_ASSERT(row < this->getNRows());
    R_ERROR_ASSERT(column < this->getNColumns());

    return this->array[row][column];
} /* RRMatrix::getValue */


std::vector<RRVector> RRMatrix::getVectors(void) const
{
    std::vector<RRVector> v;

    v.resize(this->getNRows());

    for (unsigned int i=0;i<this->getNRows();i++)
    {
        v[i].resize(this->getNColumns());
        for (unsigned int j=0;j<v[i].size();j++)
        {
            v[i][j] = this->getValue(i,j);
        }
    }

    return v;
} /* RRMatrix::getVectors */


void RRMatrix::setValue(unsigned int row, unsigned int column, double value)
{
    R_ERROR_ASSERT(row < this->getNRows());
    R_ERROR_ASSERT(column < this->getNColumns());

    this->array[row][column] = value;
} /* RRMatrix::setValue */


bool RRMatrix::isSquare(void) const
{
    return (this->getNRows() > 0 && this->getNRows() == this->getNColumns());
} /* RRMatrix::isSquare */


void RRMatrix::transpose (void)
{
    uint nr = this->getNRows();
    uint nc = this->getNColumns();

    if (nr == nc)
    {
        double rtmp;
        for (uint i=0;i<nr;i++)
        {
            for (uint j=i+1;j<nc;j++)
            {
                rtmp = this->getValue(j,i);
                this->setValue(j,i,this->getValue(i,j));
                this->setValue(i,j,rtmp);
            }
        }
    }
    else
    {
        RRMatrix rtmp(*this);

        this->resize(nc,nr);
        for (unsigned int i=0;i<nr;i++)
        {
            for (unsigned int j=0;j<nc;j++)
            {
                this->setValue(j,i,rtmp.getValue(i,j));
            }
        }
    }
} /* RRMatrix::transpose */


void RRMatrix::transpose(const RRMatrix &A)
{
    uint nr = A.getNRows();
    uint nc = A.getNColumns();

    this->resize(nc,nr);

    for (uint i=0;i<nr;i++)
    {
        for (uint j=0;j<nc;j++)
        {
            (*this)[j][i] = A[i][j];
        }
    }
} /* RRMatrix::transpose */


void RRMatrix::invert(void)
{
    R_ERROR_ASSERT (this->isSquare());

    unsigned int n = this->getNRows();

    if (n == 0)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Can not invert zero sized matrix.");
    }
    else if (n == 1)
    {
        if (this->getValue(0,0) == 0.0)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Can not invert zero value matrix.");
        }
        this->setValue(0,0,1.0/this->getValue(0,0));
    }
    else if (n == 2)
    {
        double det = this->getDeterminant();
        if (det == 0.0)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Can not invert matrix. Matrix determinant = 0");
        }

        RRMatrix inv(n,n);

        inv[0][0] =    this->getValue(1,1) / det;
        inv[0][1] = -1*this->getValue(0,1) / det;
        inv[1][0] = -1*this->getValue(1,0) / det;
        inv[1][1] =    this->getValue(0,0) / det;

        this->operator =(inv);
    }
    else if (n == 3)
    {
        double det = this->getDeterminant();
        if (det == 0.0)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Can not invert matrix. Singular matrix.");
        }

        RRMatrix inv(n,n);

        inv[0][0] =    (this->getValue(1,1)*this->getValue(2,2) - this->getValue(1,2)*this->getValue(2,1)) / det;
        inv[0][1] = -1*(this->getValue(0,1)*this->getValue(2,2) - this->getValue(0,2)*this->getValue(2,1)) / det;
        inv[0][2] =    (this->getValue(0,1)*this->getValue(1,2) - this->getValue(0,2)*this->getValue(1,1)) / det;

        inv[1][0] = -1*(this->getValue(1,0)*this->getValue(2,2) - this->getValue(1,2)*this->getValue(2,0)) / det;
        inv[1][1] =    (this->getValue(0,0)*this->getValue(2,2) - this->getValue(0,2)*this->getValue(2,0)) / det;
        inv[1][2] = -1*(this->getValue(0,0)*this->getValue(1,2) - this->getValue(0,2)*this->getValue(1,0)) / det;

        inv[2][0] =    (this->getValue(1,0)*this->getValue(2,1) - this->getValue(1,1)*this->getValue(2,0)) / det;
        inv[2][1] = -1*(this->getValue(0,0)*this->getValue(2,1) - this->getValue(0,1)*this->getValue(2,0)) / det;
        inv[2][2] =    (this->getValue(0,0)*this->getValue(1,1) - this->getValue(0,1)*this->getValue(1,0)) / det;

        this->operator =(inv);
    }
    else
    {
        std::vector<unsigned int> indxc(n);
        std::vector<unsigned int> indxr(n);
        std::vector<unsigned int> ipiv(n,0);;

        for (unsigned int i=0;i<n;i++)
        {
            double big = 0.0;
            unsigned int  irow = 0;
            unsigned int  icol = 0;
            for (unsigned int j=0;j<n;j++)
            {
                if (ipiv[j] != 1)
                {
                    for (unsigned int k=0;k<n;k++)
                    {
                        if (ipiv[k] == 0)
                        {
                            if (fabs((*this)[j][k]) >= big) {
                                big=fabs((*this)[j][k]);
                                irow=j;
                                icol=k;
                            }
                        }
                    }
                }
            }
            ++(ipiv[icol]);
            if (irow != icol)
            {
                for (unsigned int l=0;l<n;l++)
                {
                    std::swap((*this)[irow][l],(*this)[icol][l]);
                }
            }
            indxr[i]=irow;
            indxc[i]=icol;
            if ((*this)[icol][icol] == 0.0)
            {
                throw RError(R_ERROR_APPLICATION,R_ERROR_REF,"Can not invert matrix. Singular matrix.");
            }
            double pivinv=1.0/(*this)[icol][icol];
            (*this)[icol][icol]=1.0;
            for (unsigned int l=0;l<n;l++)
            {
                (*this)[icol][l] *= pivinv;
            }
            for (unsigned int ll=0;ll<n;ll++)
            {
                if (ll != icol) {
                    double dum=(*this)[ll][icol];
                    (*this)[ll][icol]=0.0;
                    for (unsigned int l=0;l<n;l++)
                    {
                        (*this)[ll][l] -= (*this)[icol][l]*dum;
                    }
                }
            }
        }
        for (unsigned int l=n;l>0;l--)
        {
            if (indxr[l-1] != indxc[l-1])
            {
                for (unsigned int k=0;k<n;k++)
                {
                    std::swap((*this)[k][indxr[l-1]],(*this)[k][indxc[l-1]]);
                }
            }
        }
    }
} /* RRMatrix::invert */


void RRMatrix::decomposeToLU(void)
{
    unsigned int m = this->getNRows();

    RRMatrix lu(m,m);
    RRMatrix &A = (*this);

    for (unsigned int j=0;j<m;j++)
    {
        for (unsigned int i=0;i<m;i++)
        {
            if (i == 0)
            {
                lu[i][j] = A[i][j];
            }
            else if (i <= j && i != 0)
            {
                lu[i][j] = 0.0;
                for (unsigned int k=0;k<i;k++)
                {
                    lu[i][j] -= lu[i][k] * lu[k][j];
                }
                lu[i][j] += A[i][j];
            }
            else
            {
                double tmpValue = 0.0;

                if (j == 0)
                {
                    if (std::abs(lu[0][0]) < RConstants::eps)
                    {
                        tmpValue = A[i][j] / RConstants::eps;
                    }
                    else
                    {
                        tmpValue = A[i][j] / lu[0][0];
                    }
                }
                else
                {
                    for (unsigned int k=0;k<j;k++)
                    {
                        tmpValue += lu[i][k] * lu[k][j];
                    }
                    if (std::abs(lu[j][j]) < RConstants::eps)
                    {
                        tmpValue = (A[i][j] - tmpValue) / RConstants::eps;
                    }
                    else
                    {
                        tmpValue = (A[i][j] - tmpValue) / lu[j][j];
                    }
                }
                lu[i][j] = tmpValue;
            }
        }
    }
    this->operator =(lu);

//    std::vector<unsigned int> idx(m);
//    std::vector<double> vv(m);

//    for (unsigned int i=0;i<m;i++)
//    {
//        double big = 0.0;
//        for (unsigned int j=0;j<m;j++)
//        {
//            big = std::max(big,std::abs(this->getValue(i,j)));
//        }
//        if (big == 0.0)
//        {
//            throw RError(R_ERROR_APPLICATION,"LU decomposition failed. Singular matrix.");
//        }
//        if (std::abs(big) < RConstants::eps)
//        {
//            vv[i] = 1.0 / RConstants::eps;
//        }
//        else
//        {
//            vv[i] = 1.0 / big;
//        }
//    }

//    unsigned int iMax;

//    for (unsigned int j=0;j<m;j++)
//    {
//        for (unsigned int i=0;i<j;i++)
//        {
//            double sum = this->getValue(i,j);
//            for (unsigned int k=1;k<i;k++)
//            {
//                sum -= this->getValue(i,k)*this->getValue(k,j);
//            }
//            this->setValue(i,j,sum);
//        }

//        double big = 0.0;

//        for (unsigned int i=j;i<m;i++)
//        {
//            double sum = this->getValue(i,j);
//            if (j > 0)
//            {
//                for (unsigned int k=0;k<(j-1);k++)
//                {
//                    sum -= this->getValue(i,k)*this->getValue(k,j);
//                }
//            }
//            this->setValue(i,j,sum);
//            double dum = vv[i]*std::abs(sum);
//            if (dum > big)
//            {
//                big = dum;
//                iMax = i;
//            }
//        }

//        if (j != iMax)
//        {
//            for (unsigned int k=0;k<m;k++)
//            {
//                double temp = this->getValue(iMax,k);
//                this->setValue(iMax,k,this->getValue(j,k));
//                this->setValue(j,k,temp);
//            }
//            vv[iMax] = vv[j];
//        }

//        idx[j] = iMax;

//        if (this->getValue(j,j) == 0.0)
//        {
//            this->setValue(j,j,1.0e-19);
//        }

//        if (j != m)
//        {
//            double dum;
//            if (std::abs(this->getValue(j,j)) < RConstants::eps)
//            {
//                dum = 1.0 / RConstants::eps;
//            }
//            else
//            {
//                dum = 1.0 / this->getValue(j,j);
//            }
//            for (unsigned int i=j+1;i<m;i++)
//            {
//                this->setValue(i,j,this->getValue(i,j) * dum);
//            }
//        }
//    }

//    unsigned int i = 0;
//    while (i < m)
//    {
//        if (i != idx[i])
//        {
//            std::swap(this->array[i],this->array[idx[i]]);
//            std::swap(idx[i],idx[idx[i]]);
//        }
//        else
//        {
//            i++;
//        }
//    }
} /* RRMatrix::decomposeToLU */


double RRMatrix::getDeterminant(void) const
{
    R_ERROR_ASSERT (this->isSquare());

    double det = 0.0;

    unsigned int n = this->getNRows();

    if (n == 1)
    {
        return this->getValue(0,0);
    }
    if (n == 2)
    {
        return this->getValue(0,0) * this->getValue(1,1) - this->getValue(0,1) * this->getValue(1,0);
    }

    RRMatrix tmp(n-1,n-1);

    for (unsigned int i=0;i<n;i++)
    {
        unsigned int l = 0;
        for (unsigned int j=0;j<n;j++)
        {
            if (i != j)
            {
                for (unsigned int k=1;k<n;k++)
                {
                    tmp.setValue(l,k-1,this->getValue(j,k));
                }
                l++;
            }
        }
        int one = 1;
        if (i & 1)
        {
            one = -1;
        }
        det += one * this->getValue(i,0) * tmp.getDeterminant();
    }

    return det;
} /* RRMatrix::getDeterminant */


RRVector RRMatrix::getSummedRows(void) const
{
    RRVector b(this->getNRows(),0.0);

    for (uint i=0;i<this->getNRows();i++)
    {
        b[i] = this->getSummedRow(i);
    }

    return b;
} /* RRMatrix::getSummedRows */


double RRMatrix::getSummedRow(unsigned int row) const
{
    R_ERROR_ASSERT (row < this->getNRows());

    double b = 0.0;

    for (uint i=0;i<this->getNColumns();i++)
    {
        b += (*this)[row][i];
    }

    return b;
} /* RRMatrix::getSummedRow */


const RRVector & RRMatrix::operator [] (unsigned int row) const
{
    R_ERROR_ASSERT (row < this->getNRows());

    return this->array[row];
} /* RRMatrix::operator [] */


RRVector & RRMatrix::operator [] (unsigned int row)
{
    R_ERROR_ASSERT (row < this->getNRows());

    return this->array[row];
} /* RRMatrix::operator [] */


void RRMatrix::operator *=(double scaleValue)
{
    for (unsigned int i=0;i<this->getNRows();i++)
    {
        this->operator [](i) *= scaleValue;
    }
} /* RRMatrix::operator *= */


RRMatrix RRMatrix::getBlock(uint iBegin, uint iEnd, uint jBegin, uint jEnd) const
{
    R_ERROR_ASSERT (iBegin <= iEnd);
    R_ERROR_ASSERT (jBegin <= jEnd);
    R_ERROR_ASSERT (this->getNRows() > iEnd);
    R_ERROR_ASSERT (this->getNColumns() > jEnd);

    uint iSize = iEnd - iBegin + 1;
    uint jSize = jEnd - jBegin + 1;

    RRMatrix m(iSize,jSize);

    for (uint i=0;i<iSize;i++)
    {
        for (uint j=0;j<jSize;j++)
        {
            m[i][j] = (*this)[i+iBegin][j+jBegin];
        }
    }

    return m;
}

void RRMatrix::setBlock(const RRMatrix &matrix, uint iBegin, uint jBegin)
{
    R_ERROR_ASSERT (this->getNRows() > iBegin);
    R_ERROR_ASSERT (this->getNColumns() > jBegin);
    R_ERROR_ASSERT (this->getNRows() >= iBegin + matrix.getNRows());
    R_ERROR_ASSERT (this->getNColumns() >= jBegin + matrix.getNColumns());

    for (uint i=0;i<matrix.getNRows();i++)
    {
        for (uint j=0;j<matrix.getNColumns();j++)
        {
            (*this)[iBegin+i][jBegin+j] = matrix[i][j];
        }
    }
} /* RRMatrix::getBlock */


QString RRMatrix::toString(void) const
{
    QString text;

    for (unsigned int i=0;i<this->getNRows();i++)
    {
        text += this->operator [](i).toString(true) + "\n";
    }

    return text;
} /* RRMatrix::toString */


void RRMatrix::print(void) const
{
    RLogger::info("Matrix - real: [%ux%u]\n",this->getNRows(),this->getNColumns());
    for (unsigned int i=0;i<this->getNRows();i++)
    {
        this->operator [](i).print(true,true);
    }
} /* RRMatrix::print */


void RRMatrix::clear (void)
{
    for (unsigned int i=0;i<this->getNRows();i++)
    {
        this->array[i].clear();
    }
    this->array.clear();
} /* RRMatrix::clear */


void RRMatrix::solveLU(const RRMatrix &A, const RRVector &x, RRVector &y)
{
    RRMatrix LU(A);

    LU.decomposeToLU();

    unsigned int m = LU.getNRows();

    y = x;

    // Solve L*b=x (where b=U*y)
    for (unsigned int i=0;i<m;i++)
    {
        double sum = y[i];
        for (unsigned int j=0;j<i;j++)
        {
            sum -= LU[i][j]*y[j];
        }
        y[i] = sum;
    }

    // Solve U*y=b
    for (unsigned int i=0;i<m;i++)
    {
        unsigned int k = m-i-1;
        double sum = y[k];
        for (unsigned int j=k+1;j<m;j++)
        {
            sum -= LU[k][j]*y[j];
        }
        if (std::abs(LU[k][k]) < RConstants::eps)
        {
            y[k] = sum / RConstants::eps;
        }
        else
        {
            y[k] = sum / LU[k][k];
        }
    }
} /* RRMatrix::solveLU */


double RRMatrix::norm(const RRMatrix &M)
{
    RRVector v(M.getNRows());

    v.fill(0.0);

    for (unsigned int i=0;i<M.getNRows();i++)
    {
        for (unsigned int j=0;j<M.getNColumns();j++)
        {
            v[i] += M[i][j];
        }
    }

    return RRVector::norm(v);
} /* RRMatrix::norm */


double RRMatrix::norm(const RRMatrix &M, const RRVector &b)
{
    RRVector v(M.getNRows());

    v.fill(0.0);

    for (unsigned int i=0;i<M.getNRows();i++)
    {
        for (unsigned int j=0;j<M.getNColumns();j++)
        {
            v[i] += M[i][j]*b[j];
        }
    }

    double norm1 = RRVector::norm(v);
    double norm2 = RRVector::norm(b);

    if (norm2 < RConstants::eps)
    {
        return 0.0;
    }
    return norm1 / norm2;
} /* RRMatrix::norm */


double RRMatrix::trace(const RRMatrix &M)
{
    unsigned int m = M.getNRows();
    if (m > M.getNColumns())
    {
        m = M.getNColumns();
    }

    double traceValue = 0.0;
    for (unsigned int i=0;i<m;i++)
    {
        traceValue += M[i][i];
    }
    return traceValue;
} /* RRMatrix::trace */


void RRMatrix::mlt(const RRMatrix &A, const RRVector &x, RRVector &y, bool add)
{
    R_ERROR_ASSERT (A.getNColumns() == x.getNRows());

    y.resize(A.getNRows(),0.0);
    if (!add)
    {
        y.fill(0.0);
    }

    for (uint i=0;i<A.getNRows();i++)
    {
        for (uint j=0;j<A.getNColumns();j++)
        {
            y[i] += A[i][j]*x[j];
        }
    }
} /* RRMatrix::mlt */


void RRMatrix::mlt(const RRMatrix &A, const RRMatrix &X, RRMatrix &Y, bool add)
{
    R_ERROR_ASSERT (A.getNColumns() == X.getNRows());

    Y.resize(A.getNRows(),X.getNColumns(),0.0);
    if (!add)
    {
        Y.fill(0.0);
    }

    for (uint i=0;i<X.getNColumns();i++)
    {
        for (uint j=0;j<A.getNRows();j++)
        {
            for (uint k=0;k<A.getNColumns();k++)
            {
                Y[j][i] += A[j][k]*X[k][i];
            }
        }
    }
} /* RRMatrix::mlt */


RRMatrix RRMatrix::generateRotationMatrix(double xAngle, double yAngle, double zAngle)
{
    RRMatrix R(3,3);
    RRMatrix Rx(3,3);
    RRMatrix Ry(3,3);
    RRMatrix Rz(3,3);

    double cxr = std::cos(xAngle);
    double sxr = std::sin(xAngle);
    double cyr = std::cos(yAngle);
    double syr = std::sin(yAngle);
    double czr = std::cos(zAngle);
    double szr = std::sin(zAngle);

    // Rx
    Rx[0][0] =  1.0; Rx[0][1] =  0.0; Rx[0][2] =  0.0;
    Rx[1][0] =  0.0; Rx[1][1] =  cxr; Rx[1][2] = -sxr;
    Rx[2][0] =  0.0; Rx[2][1] =  sxr; Rx[2][2] =  cxr;

    // Ry
    Ry[0][0] =  cyr; Ry[0][1] =  0.0; Ry[0][2] =  syr;
    Ry[1][0] =  0.0; Ry[1][1] =  1.0; Ry[1][2] =  0.0;
    Ry[2][0] = -syr; Ry[2][1] =  0.0; Ry[2][2] =  cyr;

    // Rz
    Rz[0][0] =  czr; Rz[0][1] = -szr; Rz[0][2] =  0.0;
    Rz[1][0] =  szr; Rz[1][1] =  czr; Rz[1][2] =  0.0;
    Rz[2][0] =  0.0; Rz[2][1] =  0.0; Rz[2][2] =  1.0;

    RRMatrix Rzy(3,3);

    RRMatrix::mlt(Rz,Ry,Rzy);
    RRMatrix::mlt(Rzy,Rx,R);

    return R;
} /* RRMatrix::generateRotationMatrix */
