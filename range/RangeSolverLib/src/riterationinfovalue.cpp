/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   riterationinfovalue.cpp                                  *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   6-th January 2016                                        *
 *                                                                   *
 *  DESCRIPTION: Iteration info value class definition               *
 *********************************************************************/

#include "riterationinfovalue.h"

void RIterationInfoValue::_init(const RIterationInfoValue *pIterationInfoValue)
{
    if (pIterationInfoValue)
    {
        this->name = pIterationInfoValue->name;
        this->value = pIterationInfoValue->value;
    }
}

RIterationInfoValue::RIterationInfoValue()
    : name(QString())
    , value(0.0)
{
    this->_init();
}

RIterationInfoValue::RIterationInfoValue(const QString &name, double value)
    : name(name)
    , value(value)
{
    this->_init();
}

RIterationInfoValue::RIterationInfoValue(const RIterationInfoValue &iterationInfoValue)
{
    this->_init(&iterationInfoValue);
}

RIterationInfoValue::~RIterationInfoValue()
{

}

RIterationInfoValue &RIterationInfoValue::operator =(const RIterationInfoValue &iterationInfoValue)
{
    this->_init(&iterationInfoValue);
    return (*this);
}

const QString &RIterationInfoValue::getName(void) const
{
    return this->name;
}

double RIterationInfoValue::getValue(void) const
{
    return this->value;
}
