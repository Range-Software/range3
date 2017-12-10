/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_value_table.cpp                                      *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   15-th December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Value table class definition                        *
 *********************************************************************/

#include <string>
#include <map>

#include "rbl_value_table.h"
#include "rbl_error.h"
#include "rbl_utils.h"


RValueTable::RValueTable ()
{
    this->_init ();
} /* RValueTable::RValueTable */


RValueTable::RValueTable (const RValueTable &valueTable)
{
    this->_init (&valueTable);
} /* RValueTable::RValueTable (copy) */


RValueTable::~RValueTable ()
{
} /* RValueTable::~RValueTable */


void RValueTable::_init (const RValueTable *pValueTable)
{
    if (pValueTable)
    {
        this->setKeyName (pValueTable->getKeyName());
        this->setKeyUnits (pValueTable->getKeyUnits());
        this->setValueName (pValueTable->getValueName());
        this->setValueUnits (pValueTable->getValueUnits());
        this->clear();
        for (unsigned int i=0;i<pValueTable->size();i++)
        {
            this->add (pValueTable->getKey(i),
                       pValueTable->getValue(i));
        }
    }
} /* RValueTable::_init */


const QString &RValueTable::getKeyName(void) const
{
    return this->keyName;
} /* RValueTable::get_key_name */


void RValueTable::setKeyName (const QString &name)
{
    this->keyName = name;
} /* RValueTable::set_key_name */


const QString &RValueTable::getKeyUnits(void) const
{
    return this->keyUnits;
} /* RValueTable::get_key_units */


void RValueTable::setKeyUnits (const QString &units)
{
    this->keyUnits = units;
} /* RValueTable::set_key_units */


const QString & RValueTable::getValueName (void) const
{
    return this->valueName;
} /* RValueTable::get_value_name */


void RValueTable::setValueName (const QString &name)
{
    this->valueName = name;
} /* RValueTable::set_value_name */


const QString &RValueTable::getValueUnits(void) const
{
    return this->valueUnits;
} /* RValueTable::get_value_units */


void RValueTable::setValueUnits (const QString &units)
{
    this->valueUnits = units;
} /* RValueTable::set_value_units */


unsigned int RValueTable::size (void) const
{
    return (unsigned int)this->table.size();
} /* RValueTable::size */


void RValueTable::add (double key,
                       double value)
{
    this->table[key] = value;
} /* RValueTable::add */


void RValueTable::remove (double key)
{
    R_ERROR_ASSERT (this->table.find (key) != this->table.end());

    this->table.erase (key);
} /* RValueTable::remove */


void RValueTable::remove (unsigned int position)
{
    std::map<double,double>::iterator iter = this->table.begin();
    R_ERROR_ASSERT (position < this->size());

    std::advance (iter, position);
    this->table.erase (iter);
} /* RValueTable::remove */


double RValueTable::get (double key) const
{
    double value = 0.0;
    double ratio = 0.0;
    double dKey = 0.0;
    double k1, k2;
    double v1, v2;
    std::map<double,double>::const_iterator iter;

    iter = this->table.lower_bound (key);
    k2 = iter->first;
    v2 = iter->second;

    if (iter == this->table.begin())
    {
        k1 = k2;
        v1 = v2;
    }
    else
    {
        bool isLast = false;
        if (iter == this->table.end())
        {
           isLast = true; 
        }
        std::advance (iter, -1);
        k1 = iter->first;
        v1 = iter->second;
        if (isLast)
        {
            k2 = k1;
            v2 = v1;
        }
    }

    dKey = k2 - k1;
    if (dKey == 0.0)
    {
        ratio = 0.0;
    }
    else
    {
        ratio = (key  - k1) / dKey;
    }
    value = ratio * (v2 - v1) + v1;

    return value;
} /* RValueTable::get */


double RValueTable::getKey (unsigned int position) const
{
    R_ERROR_ASSERT(position < this->table.size());
    std::map<double,double>::const_iterator iter = this->table.begin();
    std::advance (iter, position);
    return iter->first;
} /* RValueTable::get_key */


double RValueTable::getValue (unsigned int position) const
{
    R_ERROR_ASSERT(position < this->table.size());
    std::map<double,double>::const_iterator iter = this->table.begin();
    std::advance (iter, position);
    return iter->second;
} /* RValueTable::get_key */


RValueTable & RValueTable::operator = (const RValueTable &valueTable)
{
    this->_init (&valueTable);
    return (*this);
} /* RValueTable::operator = */


bool RValueTable::operator ==(const RValueTable &valueTable) const
{
    if (this->table.size() != valueTable.size())
    {
        return false;
    }
    if (this->keyName != valueTable.keyName)
    {
        return false;
    }
    if (this->keyUnits != valueTable.keyUnits)
    {
        return false;
    }
    if (this->valueName != valueTable.valueName)
    {
        return false;
    }
    if (this->valueUnits != valueTable.valueUnits)
    {
        return false;
    }
    for (unsigned int i=0;i<this->table.size();i++)
    {
        if (std::abs((this->getKey(i) - valueTable.getKey(i)) / this->getKey(i)) > RConstants::eps)
        {
            return false;
        }
        if (std::abs((this->getValue(i) - valueTable.getValue(i)) / this->getValue(i)) > RConstants::eps)
        {
            return false;
        }
    }
    return true;
} /* RValueTable::operator == */


bool RValueTable::operator !=(const RValueTable &valueTable) const
{
    return !(this->operator ==(valueTable));
} /* RValueTable::operator != */
