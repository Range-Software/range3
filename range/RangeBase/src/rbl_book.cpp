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

RBook::RBook(unsigned int size)
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

void RBook::resize(unsigned int size)
{
    this->book.resize(size);
    this->initialize();
}

unsigned int RBook::size(void) const
{
    return this->book.size();
}

void RBook::initialize(void)
{
    for (unsigned int i=0;i<this->book.size();i++)
    {
        this->book[i] = i;
    }
}

bool RBook::getValue(unsigned int position, unsigned int &value) const
{
    value = this->book[position];
    if (value != RConstants::eod)
    {
        return true;
    }
    return false;
}

void RBook::setValue(unsigned int position, unsigned int value)
{
    this->book[position] = value;
}

void RBook::enable(unsigned int position, bool consolidate)
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
        for (unsigned int i=position;i>0;i--)
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
        for (unsigned int i=position+1;i<this->book.size();i++)
        {
            if (this->book[i] != RConstants::eod)
            {
                this->book[i]++;
            }
        }
    }
}

void RBook::disable(unsigned int position, bool consolidate)
{
    if (this->book[position] == RConstants::eod)
    {
        return;
    }

    this->book[position] = RConstants::eod;

    if (consolidate)
    {
        for (unsigned int i=position+1;i<this->book.size();i++)
        {
            if (this->book[i] != RConstants::eod)
            {
                this->book[i]--;
            }
        }
    }
}

unsigned int RBook::getNEnabled(void) const
{
    return this->book.size() - this->getNDisabled();
}

unsigned int RBook::getNDisabled(void) const
{
    unsigned int nDisabled = 0;
    for (unsigned int i=0;i<this->book.size();i++)
    {
        if (this->book[i] == RConstants::eod)
        {
            nDisabled++;
        }
    }
    return nDisabled;
}

RUVector RBook::getEnabled(void) const
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

void RBook::print(void) const
{
    RLogger::info("Node book: [%u]\n",this->size());
    for (unsigned int i=0;i<this->size();i++)
    {
        RLogger::info("%9u: %9u\n",i,this->book[i]);
    }
}
