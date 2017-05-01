/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rauthenticator.cpp                                       *
 *  GROUP:  RAL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   30-th August 2016                                        *
 *                                                                   *
 *  DESCRIPTION: Authenticator class definition                      *
 *********************************************************************/

#include <rblib.h>

#include "rauthenticator.h"
#include "rlicense.h"

void RAuthenticator::_init(const RAuthenticator *pAuthenticator)
{
    if (pAuthenticator)
    {
        this->licenseFile = pAuthenticator->licenseFile;
        this->account = pAuthenticator->account;
        this->password = pAuthenticator->password;
    }
}

RAuthenticator::RAuthenticator(const QString &licenseFile, const QString &account, const QString &password)
    : licenseFile(licenseFile)
    , account(account)
    , password(password)
{
    this->_init();
}

RAuthenticator::RAuthenticator(const RAuthenticator &authenticator)
{
    this->_init(&authenticator);
}

RAuthenticator::~RAuthenticator()
{

}

RAuthenticator &RAuthenticator::operator =(const RAuthenticator &authenticator)
{
    this->_init(&authenticator);
    return (*this);
}

void RAuthenticator::setLicenseFile(const QString &licenseFile)
{
    this->licenseFile = licenseFile;
}

void RAuthenticator::setAccount(const QString &account)
{
    this->account = account;
}

void RAuthenticator::setPassword(const QString &password)
{
    this->password = password;
}

bool RAuthenticator::authenticate(RAuthenticator::Area area) const
{
    RLicense license;
    try
    {
        license.read(this->licenseFile);
        return license.validateRecord(RAuthenticator::getName(area),this->account,this->password);
    }
    catch (const RError &rError)
    {
        RLogger::warning("Area authentication failed. %s",rError.getMessage().toUtf8().constData());
    }
    return false;
}

QList<RAuthenticator::Area> RAuthenticator::getAreaList(void)
{
    QList<RAuthenticator::Area> areaList;
    areaList.append(Acoustics);
    areaList.append(Contaminant);
    areaList.append(Electrostatics);
    areaList.append(Magnetostatics);
    areaList.append(Fluid);
    areaList.append(FluidHeat);
    areaList.append(Heat);
    areaList.append(RadiativeHeat);
    areaList.append(Stress);
    areaList.append(StressModal);
    areaList.append(Potential);
    areaList.append(Wave);
    return areaList;
}

QList<RAuthenticator::Area> RAuthenticator::getAreaList(RAuthenticator::Area mask)
{
    QList<RAuthenticator::Area> areaList;
    foreach (Area area, RAuthenticator::getAreaList())
    {
        if (area & mask)
        {
            areaList.append(area);
        }
    }
    return areaList;
}

QString RAuthenticator::getName(RAuthenticator::Area area)
{
    switch (area)
    {
        case Acoustics:
        {
            return "Acoustics";
        }
        case Contaminant:
        {
            return "Contaminant";
        }
        case Electrostatics:
        {
            return "Electrostatics";
        }
        case Magnetostatics:
        {
            return "Magnetostatics";
        }
        case Fluid:
        {
            return "Fluid";
        }
        case FluidHeat:
        {
            return "FluidHeat";
        }
        case Heat:
        {
            return "Heat";
        }
        case RadiativeHeat:
        {
            return "RadiativeHeat";
        }
        case Stress:
        {
            return "Stress";
        }
        case StressModal:
        {
            return "StressModal";
        }
        case Potential:
        {
            return "Potential";
        }
        case Wave:
        {
            return "Wave";
        }
        default:
        {
            return QString();
        }
    }
}
