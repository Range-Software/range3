/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_radiation_setup.cpp                                  *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   4-th July 2014                                           *
 *                                                                   *
 *  DESCRIPTION: Radiation setup class definition                    *
 *********************************************************************/

#include <QFileInfo>

#include "rml_file_manager.h"
#include "rml_radiation_setup.h"


void RRadiationSetup::_init(const RRadiationSetup *pRadiationSetup)
{
    if (pRadiationSetup)
    {
        this->resolution = pRadiationSetup->resolution;
        this->viewFactorMatrixFile = pRadiationSetup->viewFactorMatrixFile;
    }
}

RRadiationSetup::RRadiationSetup()
    : resolution(R_RADIATION_RESOLUTION_MEDIUM)
{
    this->_init();
}

RRadiationSetup::RRadiationSetup(const RRadiationSetup &radiationSetup)
{
    this->_init(&radiationSetup);
}

RRadiationSetup::~RRadiationSetup()
{

}

RRadiationSetup &RRadiationSetup::operator =(const RRadiationSetup &radiationSetup)
{
    this->_init(&radiationSetup);
    return (*this);
}

RRadiationResolution RRadiationSetup::getResolution(void) const
{
    return this->resolution;
}

void RRadiationSetup::setResolution(const RRadiationResolution &value)
{
    this->resolution = value;
}

const QString &RRadiationSetup::getViewFactorMatrixFile(void) const
{
    return this->viewFactorMatrixFile;
}

void RRadiationSetup::setViewFactorMatrixFile(const QString &viewFactorMatrixFile)
{
    this->viewFactorMatrixFile = viewFactorMatrixFile;
}

QString RRadiationSetup::findRecentViewFactorMatrixFile(const QString &viewFactorMatrixFile, uint timeStep)
{
    if (viewFactorMatrixFile.isEmpty())
    {
        return QString();
    }

    for (uint i=0;i<=timeStep;i++)
    {
        QString fileNameWithTimeStamp = RFileManager::getFileNameWithTimeStep(viewFactorMatrixFile,timeStep-i);

        if (QFileInfo::exists(fileNameWithTimeStamp))
        {
            return fileNameWithTimeStamp;
        }
    }

    if (QFileInfo::exists(viewFactorMatrixFile))
    {
        return viewFactorMatrixFile;
    }

    return QString();
}

QString RRadiationSetup::getResolutionText(RRadiationResolution resolution)
{
    if (resolution <= R_RADIATION_RESOLUTION_LOW)
    {
        return "Low";
    }
    if (resolution > R_RADIATION_RESOLUTION_LOW && resolution < R_RADIATION_RESOLUTION_HIGH)
    {
        return "Medium";
    }
    if (resolution >= R_RADIATION_RESOLUTION_HIGH)
    {
        return "High";
    }
    return "Unknown";
}

