/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_rvector.cpp                                          *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   21-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Real vector class declaration                       *
 *********************************************************************/

#include <QTextStream>

#include <cmath>

#include "rbl_logger.h"
#include "rbl_rvector.h"
#include "rbl_error.h"
#include "rbl_utils.h"


RRVector::RRVector(uint nRows, double value)
{
    this->resize(nRows,value);
    this->_init();
} /* RRVector::RRVector */


RRVector::RRVector(const RRVector &array)
    : std::vector<double>(array)
{
    this->_init(&array);
} /* RRVector::RRVector */

RRVector::RRVector(const std::vector<double> &array)
    : std::vector<double>(array)
{
    this->_init();
} /* RRVector::RRVector (copy) */



RRVector::RRVector(double x, double y, double z)
{
    this->resize(3);
    this->operator [](0) = x;
    this->operator [](1) = y;
    this->operator [](2) = z;
    this->_init();
} /* RRVector::RRVector */


RRVector::~RRVector()
{
} /* RRVector::~RRVector */


RRVector & RRVector::operator =(const RRVector &array)
{
    this->std::vector<double>::operator = (array);
    this->_init(&array);
    return (*this);
} /* RRVector::operator = */


void RRVector::fill(double value)
{
    std::fill(this->begin(),this->end(),value);
} /* RRVector::fill */


void RRVector::append(const RRVector &array)
{
    this->insert(this->end(), array.begin(), array.end());
} /* RRVector::append */


void RRVector::_init(const RRVector *pArray)
{
    if (pArray)
    {
    }
} /* RRVector::_init */


double & RRVector::operator [](uint n)
{
    R_ERROR_ASSERT(n<this->getNRows());

    return this->at(n);
} /* RRVector::operator [] */


const double & RRVector::operator [](uint n) const
{
    R_ERROR_ASSERT(n<this->getNRows());

    return this->at(n);
} /* RRVector::operator [] */


void RRVector::operator *=(double scaleValue)
{
    for (uint i=0;i<this->size();i++)
    {
        this->at(i) *= scaleValue;
    }
} /* RRVector::operator *= */

bool RRVector::operator ==(const RRVector &array) const
{
    if (this->size() != array.size())
    {
        return false;
    }
    for (uint i=0;i<this->size();i++)
    {
        if (!R_D_ARE_SAME(this->at(i),array.at(i)))
        {
            return false;
        }
    }
    return true;
} /* RRVector::operator == */

bool RRVector::operator !=(const RRVector &array) const
{
    return ! this->operator==(array);
} /* RRVector::operator != */


double RRVector::length() const
{
    double len = 0.0;
    for (uint i=0;i<this->size();i++)
    {
        len += std::pow(this->at(i),2);
    }
    return sqrt(len);
} /* RRVector::length */


double RRVector::normalize()
{
    double len = this->length();
    if (len != 0.0)
    {
        for (uint i=0;i<this->size();i++)
        {
            (*this)[i] /= len;
        }
    }
    return len;
} /* RRVector::normalize */


void RRVector::scale(double scale)
{
    for (uint i=0;i<this->size();i++)
    {
        (*this)[i] *= scale;
    }
} /* RRVector::scale */


QString RRVector::toString(bool oneLine) const
{
    QString string;
    QTextStream ss(&string);

    ss.setFieldWidth(15);
    ss.setRealNumberNotation(QTextStream::ScientificNotation);
    ss.setRealNumberPrecision(6);

    for (uint i=0;i<this->size();i++)
    {
        ss << this->operator [](i);
        if (!oneLine)
        {
            ss << "\n";
        }
    }

    return string;
} /* RRVector::toString */


void RRVector::print(bool oneLine, bool newLine) const
{
    if (!oneLine)
    {
        RLogger::info("Vector - real: [%u]\n",this->size());
    }
    for (uint i=0;i<this->size();i++)
    {
        RLogger::info("%15.6e",this->at(i));
        if (!oneLine)
        {
            RLogger::info("\n");
        }
    }
    if (newLine)
    {
        RLogger::info("\n");
    }
} /* RRVector::print */


double RRVector::dot(const RRVector &v1, const RRVector &v2)
{
    R_ERROR_ASSERT(v1.size() == v2.size());

    double dotProd = 0.0;
    for (uint i=0;i<v1.size();i++)
    {
        dotProd += v1[i]*v2[i];
    }
    return dotProd;
} /* RRVector::dot */


void RRVector::add(const RRVector &v1, const RRVector &v2, RRVector &x)
{
    R_ERROR_ASSERT(v1.size() == v2.size());

    x.resize(v1.size());

    for (uint i=0;i<v1.size();i++)
    {
        x[i] = v1[i] + v2[i];
    }
} /* RRVector::add */


void RRVector::subtract(const RRVector &v1, const RRVector &v2, RRVector &x)
{
    R_ERROR_ASSERT(v1.size() == v2.size());

    x.resize(v1.size());

    for (uint i=0;i<v1.size();i++)
    {
        x[i] = v1[i] - v2[i];
    }
} /* RRVector::subtract */


double RRVector::norm(const RRVector &v)
{
    return v.length();
} /* RRVector::norm */
