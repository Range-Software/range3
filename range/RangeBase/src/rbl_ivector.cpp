/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_ivector.cpp                                          *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   21-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Integer vector class definition                     *
 *********************************************************************/

#include <algorithm>

#include "rbl_ivector.h"
#include "rbl_error.h"
#include "rbl_logger.h"


RIVector::RIVector (unsigned int nRows, int value)
{
    this->resize(nRows,value);
    this->_init();
} /* RIVector::RIVector */


RIVector::RIVector (const RIVector &array) : std::vector<int>(array)
{
    this->_init(&array);
} /* RIVector::RIVector (copy) */


RIVector::~RIVector ()
{
} /* RIVector::~RIVector */


RIVector & RIVector::operator = (const RIVector &array)
{
    this->std::vector<int>::operator = (array);
    this->_init(&array);
    return (*this);
} /* RIVector::operator = */


void RIVector::_init(const RIVector *pArray)
{
    if (pArray)
    {
    }
} /* RIVector::_init */


int & RIVector::operator [] (unsigned int n)
{
    R_ERROR_ASSERT(n<this->getNRows());

    return this->at(n);
} /* RIVector::operator [] */


const int & RIVector::operator [] (unsigned int n) const
{
    R_ERROR_ASSERT(n<this->getNRows());

    return this->at(n);
} /* RIVector::operator [] */

void RIVector::fill(int value)
{
    std::fill(this->begin(),this->end(),value);
}

void RIVector::print(bool oneLine, bool newLine) const
{
    if (!oneLine)
    {
        RLogger::info("Vector - int: [%u]\n",this->size());
    }
    for (unsigned int i=0;i<this->size();i++)
    {
        RLogger::info("%15d",this->at(i));
        if (!oneLine)
        {
            RLogger::info("\n");
        }
    }
    if (newLine)
    {
        RLogger::info("\n");
    }
} /* RIVector::fill */
