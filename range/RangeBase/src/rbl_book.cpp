/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_book.cpp                                             *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   4-th August 2014                                         *
 *                                                                   *
 *  DESCRIPTION: Book class definition                               *
 *********************************************************************/

#include <limits.h>

#include "rbl_book.h"
#include "rbl_logger.h"
#include "rbl_utils.h"

void RBook::_init(const RBook *pBook)
{
    if (pBook)
    {
        this->book = pBook->book;
    }
}

RBook::RBook(uint size)
{
    this->_init();
    this->resize(size);
}

RBook::RBook(const RBook &book)
{
    this->_init(&book);
}

RBook::~RBook()
{
}

RBook &RBook::operator =(const RBook &book)
{
    this->_init(&book);
    return (*this);
}

void RBook::resize(uint size)
{
    this->book.resize(size);
    this->initialize();
}

uint RBook::size() const
{
    return (uint)this->book.size();
}

void RBook::initialize()
{
    for (uint i=0;i<this->book.size();i++)
    {
        this->book[i] = i;
    }
}

bool RBook::getValue(uint position, uint &value) const
{
    value = this->book[position];
    if (value != RConstants::eod)
    {
        return true;
    }
    return false;
}

void RBook::setValue(uint position, uint value)
{
    this->book[position] = value;
}

void RBook::enable(uint position, bool consolidate)
{
    if (this->book[position] != RConstants::eod)
    {
        return;
    }

    if (position == 0)
    {
        this->book[position] = 0;
    }
    else
    {
        for (uint i=position;i>0;i--)
        {
            if (this->book[i-1] != RConstants::eod)
            {
                this->book[position] = this->book[i-1] + 1;
                break;
            }
        }
        this->book[position] = 0;
    }

    if (consolidate)
    {
        for (uint i=position+1;i<this->book.size();i++)
        {
            if (this->book[i] != RConstants::eod)
            {
                this->book[i]++;
            }
        }
    }
}

void RBook::disable(uint position, bool consolidate)
{
    if (this->book[position] == RConstants::eod)
    {
        return;
    }

    this->book[position] = RConstants::eod;

    if (consolidate)
    {
        for (uint i=position+1;i<this->book.size();i++)
        {
            if (this->book[i] != RConstants::eod)
            {
                this->book[i]--;
            }
        }
    }
}

uint RBook::getNEnabled() const
{
    return (uint)this->book.size() - this->getNDisabled();
}

uint RBook::getNDisabled() const
{
    uint nDisabled = 0;
    for (uint i=0;i<this->book.size();i++)
    {
        if (this->book[i] == RConstants::eod)
        {
            nDisabled++;
        }
    }
    return nDisabled;
}

RUVector RBook::getEnabled() const
{
    uint nEnabled = 0;
    RUVector enabled;

    enabled.resize(this->size());

    for (uint i=0;i<this->size();i++)
    {
        if (this->book[i] != RConstants::eod)
        {
            enabled[this->book[i]] = i;
            nEnabled++;
        }
    }

    enabled.resize(nEnabled);

    return enabled;
}

void RBook::print() const
{
    RLogger::info("Node book: [%u]\n",this->size());
    for (uint i=0;i<this->size();i++)
    {
        RLogger::info("%9u: %9u\n",i,this->book[i]);
    }
}
