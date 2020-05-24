/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_uvector.cpp                                          *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   14-th November 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Bool vector class definition                        *
 *********************************************************************/

#include <algorithm>

#include "rbl_bvector.h"
#include "rbl_error.h"
#include "rbl_logger.h"


RBVector::RBVector (uint nRows, bool value)
{
    this->resize(nRows,value);
    this->_init();
} /* RBVector::RBVector */


RBVector::RBVector (const RBVector &array) : std::vector<bool>(array)
{
    this->_init(&array);
} /* RBVector::RBVector (copy) */


RBVector::~RBVector ()
{
} /* RBVector::~RBVector */


RBVector & RBVector::operator = (const RBVector &array)
{
    this->std::vector<bool>::operator = (array);
    this->_init(&array);
    return (*this);
} /* RBVector::operator = */


void RBVector::_init(const RBVector *pArray)
{
    if (pArray)
    {
    }
} /* RBVector::_init */


//bool & RBVector::operator [] (uint n)
//{
//    R_ERROR_ASSERT(n<this->getNRows());

//    return this->at(n);
//} /* RBVector::operator [] */


//const bool & RBVector::operator [] (uint n) const
//{
//    R_ERROR_ASSERT(n<this->getNRows());

//    return this->at(n);
//} /* RBVector::operator [] */


void RBVector::fill(bool value)
{
    std::fill(this->begin(),this->end(),value);
} /* RBVector::fill */


void RBVector::print(bool oneLine, bool newLine) const
{
    if (!oneLine)
    {
        RLogger::info("Vector - bool: [%u]\n",this->size());
    }
    for (uint i=0;i<this->size();i++)
    {
        RLogger::info("%7s",this->at(i)?"true":"false");
        if (!oneLine)
        {
            RLogger::info("\n");
        }
    }
    if (newLine)
    {
        RLogger::info("\n");
    }
} /* RBVector::print */
