/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_variable_data.cpp                                    *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   18-th February 2013                                      *
 *                                                                   *
 *  DESCRIPTION: Variable data class definition                      *
 *********************************************************************/

#include <rblib.h>

#include "rml_variable_data.h"


void RVariableData::_init(const RVariableData *pVariableData)
{
    if (pVariableData)
    {
        this->minDisplayValue = pVariableData->minDisplayValue;
        this->maxDisplayValue = pVariableData->maxDisplayValue;
        this->displayValueRange = pVariableData->displayValueRange;
        this->valueRangeName = pVariableData->valueRangeName;
        this->scale = pVariableData->scale;
    }
} /* RVariableData::_init */

RVariableData::RVariableData()
    : minDisplayValue(0.0)
    , maxDisplayValue(0.0)
    , displayValueRange(false)
    , scale(1.0)
{
    this->_init();
} /* RVariableData::RVariableData */


RVariableData::RVariableData(const RVariableData &variableData)
{
    this->_init(&variableData);
} /* RVariableData::RVariableData (copy) */


RVariableData::~RVariableData()
{
} /* RVariableData::~RVariableData */


RVariableData &RVariableData::operator =(const RVariableData &variableData)
{
    this->_init(&variableData);
    return (*this);
} /* RVariableData::operator = */


double RVariableData::getMinDisplayValue(void) const
{
    return this->minDisplayValue;
} /* RVariableData::getMinDisplayValue */


double RVariableData::getMaxDisplayValue(void) const
{
    return this->maxDisplayValue;
} /* RVariableData::getMaxDisplayValue */


void RVariableData::setMinMaxDisplayValue(double minDisplayValue, double maxDisplayValue)
{
    R_ERROR_ASSERT(minDisplayValue <= maxDisplayValue);
    this->minDisplayValue = minDisplayValue;
    this->maxDisplayValue = maxDisplayValue;
} /* RVariableData::setMinMaxDisplayValue */


bool RVariableData::getDisplayValueRange(void) const
{
    return this->displayValueRange;
} /* RVariableData::getDisplayValueRange */


void RVariableData::setDisplayValueRange(bool displayValueRange)
{
    this->displayValueRange = displayValueRange;
} /* RVariableData::setDisplayValueRange */


const QString &RVariableData::getValueRangeName(void) const
{
    return this->valueRangeName;
} /* RVariableData::getValueRangeName */


void RVariableData::setValueRangeName(const QString &valueRangeName)
{
    this->valueRangeName = valueRangeName;
} /* RVariableData::setValueRangeName */


double RVariableData::getScale(void) const
{
    return this->scale;
} /* RVariableData::getScale */


void RVariableData::setScale(double scale)
{
    this->scale = scale;
} /* RVariableData::setScale */
