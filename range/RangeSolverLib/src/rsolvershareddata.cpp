/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rsolvershareddata.cpp                                    *
 *  GROUP:  RSolverLib                                               *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   7-th November 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Range solver shared data class definition           *
 *********************************************************************/

#include "rsolvershareddata.h"

void RSolverSharedData::_init(const RSolverSharedData *pSolverSharedData)
{
    if (pSolverSharedData)
    {
        this->data = pSolverSharedData->data;
    }
}

RSolverSharedData::RSolverSharedData()
{
    this->_init();
}

RSolverSharedData::RSolverSharedData(const RSolverSharedData &solverSharedData)
{
    this->_init(&solverSharedData);
}

RSolverSharedData::~RSolverSharedData()
{

}

RSolverSharedData &RSolverSharedData::operator =(const RSolverSharedData &solverSharedData)
{
    this->_init(&solverSharedData);
    return (*this);
}

bool RSolverSharedData::hasData(const QString &name, uint dataSize) const
{
    if (this->data.contains(name))
    {
        return (this->data[name].size() == dataSize);
    }
    return false;
}

void RSolverSharedData::addData(const QString &name, const RRVector &data)
{
    this->data[name] = data;
}

RRVector &RSolverSharedData::findData(const QString &name)
{
    return this->data[name];
}

void RSolverSharedData::clearData(void)
{
    this->data.clear();
}

