/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_uvector.cpp                                          *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   6-th January 2014                                        *
 *                                                                   *
 *  DESCRIPTION: Unsigned integer vector class definition            *
 *********************************************************************/

#include <algorithm>

#include "rbl_uvector.h"
#include "rbl_error.h"
#include "rbl_logger.h"


RUVector::RUVector (unsigned int nRows, unsigned int value)
{
    this->resize(nRows,value);
    this->_init();
} /* RUVector::RUVector */


RUVector::RUVector (const RUVector &array) : std::vector<unsigned int>(array)
{
    this->_init(&array);
} /* RUVector::RUVector (copy) */


RUVector::~RUVector ()
{
} /* RUVector::~RUVector */


RUVector & RUVector::operator = (const RUVector &array)
{
    this->std::vector<unsigned int>::operator = (array);
    this->_init(&array);
    return (*this);
} /* RUVector::operator = */


void RUVector::_init(const RUVector *pArray)
{
    if (pArray)
    {
    }
} /* RUVector::_init */


unsigned int & RUVector::operator [] (unsigned int n)
{
    R_ERROR_ASSERT(n<this->getNRows());

    return this->at(n);
} /* RUVector::operator [] */


const unsigned int & RUVector::operator [] (unsigned int n) const
{
    R_ERROR_ASSERT(n<this->getNRows());

    return this->at(n);
} /* RUVector::operator [] */

void RUVector::fill(unsigned int value)
{
    std::fill(this->begin(),this->end(),value);
}

void RUVector::print(bool oneLine, bool newLine) const
{
    if (!oneLine)
    {
        RLogger::info("Vector - unsigned int: [%u]\n",this->size());
    }
    for (unsigned int i=0;i<this->size();i++)
    {
        RLogger::info("%15u",this->at(i));
        if (!oneLine)
        {
            RLogger::info("\n");
        }
    }
    if (newLine)
    {
        RLogger::info("\n");
    }
} /* RUVector::fill */
