/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_monitoring_point.cpp                                 *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   12-th March 2014                                         *
 *                                                                   *
 *  DESCRIPTION: Monitoring point class definition                   *
 *********************************************************************/

#include <cmath>

#include "rml_monitoring_point.h"
#include "rml_file_io.h"
#include "rml_save_file.h"

void RMonitoringPoint::_init(const RMonitoringPoint *pMonitoringPoint)
{
    if (pMonitoringPoint)
    {
        this->variableType = pMonitoringPoint->variableType;
        this->position = pMonitoringPoint->position;
        this->outputFileName = pMonitoringPoint->outputFileName;
    }
}

RMonitoringPoint::RMonitoringPoint()
    : variableType(R_VARIABLE_NONE)
{
    this->_init();
}

RMonitoringPoint::RMonitoringPoint(RVariableType variableType, const RR3Vector &position)
    : variableType(variableType)
    , position(position)
{
}

RMonitoringPoint::RMonitoringPoint(const RMonitoringPoint &monitoringPoint)
{
    this->_init(&monitoringPoint);
}

RMonitoringPoint::~RMonitoringPoint()
{

}

RMonitoringPoint &RMonitoringPoint::operator =(const RMonitoringPoint &monitoringPoint)
{
    this->_init(&monitoringPoint);
    return (*this);
}

bool RMonitoringPoint::operator ==(const RMonitoringPoint &monitoringPoint) const
{
    if (this->variableType != monitoringPoint.variableType)
    {
        return false;
    }
    if (std::fabs(this->position[0]-monitoringPoint.position[0]) > RConstants::eps)
    {
        return false;
    }
    if (std::fabs(this->position[1]-monitoringPoint.position[1]) > RConstants::eps)
    {
        return false;
    }
    if (std::fabs(this->position[2]-monitoringPoint.position[2]) > RConstants::eps)
    {
        return false;
    }
    return true;
}

bool RMonitoringPoint::operator <(const RMonitoringPoint &monitoringPoint) const
{
    if (this->variableType < monitoringPoint.variableType)
    {
        return true;
    }
    if (this->position[0] < monitoringPoint.position[0] - RConstants::eps)
    {
        return true;
    }
    if (this->position[1] < monitoringPoint.position[1] - RConstants::eps)
    {
        return true;
    }
    if (this->position[2] < monitoringPoint.position[2] - RConstants::eps)
    {
        return true;
    }
    return false;
}

RVariableType RMonitoringPoint::getVariableType(void) const
{
    return this->variableType;
}

void RMonitoringPoint::setVariableType(RVariableType variableType)
{
    this->variableType = variableType;
}

const RR3Vector &RMonitoringPoint::getPosition(void) const
{
    return this->position;
}

void RMonitoringPoint::setPosition(const RR3Vector &position)
{
    this->position = position;
}

const QString &RMonitoringPoint::getOutputFileName(void) const
{
    return this->outputFileName;
}

void RMonitoringPoint::setOutputFileName(const QString &outputFileName)
{
    this->outputFileName = outputFileName;
}

void RMonitoringPoint::writeValueVectorToFile(unsigned int iteration, const RValueVector &valueVector) const
{
    if (this->outputFileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"Empty file name was provided");
    }

    RFile file(this->outputFileName,RFile::ASCII);

    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",this->outputFileName.toUtf8().constData());
    }

    RFileIO::writeAscii(file,(*this),false);
    RFileIO::writeAscii(file,' ',false);
    RFileIO::writeAscii(file,iteration+1,false);
    RFileIO::writeAscii(file,' ',false);
    RFileIO::writeAscii(file,valueVector.getDataVector(),true,false);
    RFileIO::writeNewLineAscii(file);
}

void RMonitoringPoint::readRecordFromFile(RFile &in, unsigned int &iteration, RRVector &valueVector)
{
    RFileIO::readAscii(in,(*this));
    RFileIO::readAscii(in,iteration);
    RFileIO::readAscii(in,valueVector,true);
}
