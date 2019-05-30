/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   graph_object.cpp                                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   6-th March 2014                                          *
 *                                                                   *
 *  DESCRIPTION: Graph object class definition                       *
 *********************************************************************/

#include <rblib.h>

#include "graph_object.h"

GraphObject::GraphObject(QObject *parent) :
    QObject(parent)
{
    this->sourceReadTimer = new QTimer(this);
    this->sourceReadTimer->setSingleShot(false);
    QObject::connect(this->sourceReadTimer,
                     &QTimer::timeout,
                     this,
                     &GraphObject::loadData);
}

const GraphData &GraphObject::getData(void) const
{
    return this->graphData;
}

GraphData &GraphObject::getData(void)
{
    return this->graphData;
}

void GraphObject::setData(const GraphData &graphData)
{
    this->graphData = graphData;
    this->setDataChanged();
}

void GraphObject::setDataChanged(void)
{
    emit this->dataChanged();
}

uint GraphObject::getSourceReadInterval(void) const
{
    return this->sourceReadInterval;
}

void GraphObject::setSourceReadInterval(uint sourceReadInterval)
{
    this->sourceReadInterval = sourceReadInterval;
    this->sourceReadTimer->stop();
    if (this->sourceReadInterval > 0)
    {
        this->sourceReadTimer->start(int(this->sourceReadInterval));
    }
}

void GraphObject::readSource(void)
{

}

void GraphObject::loadData(void)
{
    try
    {
        this->readSource();
    }
    catch (RError &error)
    {
        RLogger::error("Failed to read graph source. %s\n",error.getMessage().toUtf8().constData());
        return;
    }

    this->setDataChanged();
}
