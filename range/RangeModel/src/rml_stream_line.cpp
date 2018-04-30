/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_stream_line.cpp                                      *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   20-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: Stream line class definition                        *
 *********************************************************************/

#include "rml_stream_line.h"

const QString RStreamLine::defaultName("Stream line");

void RStreamLine::_init(const RStreamLine *pStreamLine)
{
    if (pStreamLine)
    {
        this->variableType = pStreamLine->variableType;
        this->position = pStreamLine->position;
    }
}

RStreamLine::RStreamLine()
    : variableType(R_VARIABLE_NONE)
{
    this->name = RStreamLine::defaultName;
    this->_init();
}

RStreamLine::RStreamLine(const RStreamLine &streamLine) :
    RInterpolatedEntity(streamLine)
{
    this->_init(&streamLine);
}

RStreamLine::~RStreamLine()
{
}

RStreamLine &RStreamLine::operator =(const RStreamLine &streamLine)
{
    this->RInterpolatedEntity::operator = (streamLine);
    this->_init(&streamLine);
    return (*this);
}

RVariableType RStreamLine::getVariableType(void) const
{
    return this->variableType;
}

void RStreamLine::setVariableType(RVariableType variableType)
{
    this->variableType = variableType;
}

const RR3Vector &RStreamLine::getPosition(void) const
{
    return this->position;
}

void RStreamLine::setPosition(const RR3Vector &position)
{
    this->position = position;
}
