/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_version.cpp                                          *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   16-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Version class definition                            *
 *********************************************************************/

#include <QStringList>

#include "rbl_version.h"

const QChar RVersion::releaseTypeSeparator('_');

void RVersion::_init(const RVersion *pVersion)
{
    if (pVersion)
    {
        this->mMajor = pVersion->mMajor;
        this->mMinor = pVersion->mMinor;
        this->mRelease = pVersion->mRelease;
        this->mReleaseType = pVersion->mReleaseType;
    }
}

RVersion::RVersion() :
    mMajor(0),
    mMinor(0),
    mRelease(0)
{
    this->_init();
}

RVersion::RVersion(unsigned int major, unsigned int minor, unsigned int release, ReleaseType mReleaseType) :
    mMajor(major),
    mMinor(minor),
    mRelease(release),
    mReleaseType(mReleaseType)
{
    this->_init();
}

RVersion::RVersion(const QString &versionStr, char delimiter) :
    mMajor(0),
    mMinor(0),
    mRelease(0),
    mReleaseType(Release)
{
    this->_init();

    QStringList query = versionStr.split(QChar(delimiter));

    if (query.size() > 0)
    {
        this->mMajor = query[0].toUInt();
    }
    if (query.size() > 1)
    {
        this->mMinor = query[1].toUInt();
    }
    if (query.size() > 2)
    {
        QStringList releaseQuery = query[2].split(RVersion::releaseTypeSeparator);
        if (releaseQuery.size() == 1)
        {
            this->mReleaseType = Release;
            this->mRelease = releaseQuery[0].toUInt();
        }
        if (releaseQuery.size() > 1)
        {
            this->mReleaseType = RVersion::getReleaseTypeFromString(releaseQuery[0]);
            this->mRelease = releaseQuery[1].toUInt();
        }
    }
}

RVersion::RVersion(const RVersion &version)
{
    this->_init(&version);
}

RVersion::~RVersion()
{
}

QString RVersion::toString(char delimiter) const
{
    QString versionStr;

    versionStr = QString::number(this->mMajor)
               + delimiter
               + QString::number(this->mMinor)
               + delimiter
               + (this->mReleaseType != RVersion::Release ? RVersion::getReleaseStringFromType(this->mReleaseType) + RVersion::releaseTypeSeparator : "")
               + QString::number(this->mRelease);
    return versionStr;
}

RVersion &RVersion::operator =(const RVersion &version)
{
    this->_init(&version);
    return (*this);
}

bool RVersion::operator ==(const RVersion &version) const
{
    if (this->mMajor == version.mMajor &&
        this->mMinor == version.mMinor &&
        this->mRelease == version.mRelease &&
        this->mReleaseType == version.mReleaseType)
    {
        return true;
    }
    return false;
}

bool RVersion::operator !=(const RVersion &version) const
{
    return !((*this)==version);
}

bool RVersion::operator >(const RVersion &version) const
{
    if (this->mMajor > version.mMajor)
    {
        return true;
    }
    else if (this->mMajor == version.mMajor)
    {
        if (this->mMinor > version.mMinor)
        {
            return true;
        }
        else if (this->mMinor == version.mMinor)
        {
            if (this->mReleaseType > version.mReleaseType)
            {
                return true;
            }
            else if (this->mReleaseType == version.mReleaseType)
            {
                if (this->mRelease > version.mRelease)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool RVersion::operator <(const RVersion &version) const
{
    if (this->mMajor < version.mMajor)
    {
        return true;
    }
    else if (this->mMajor == version.mMajor)
    {
        if (this->mMinor < version.mMinor)
        {
            return true;
        }
        else if (this->mMinor == version.mMinor)
        {
            if (this->mReleaseType < version.mReleaseType)
            {
                return true;
            }
            else if (this->mReleaseType == version.mReleaseType)
            {
                if (this->mRelease < version.mRelease)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool RVersion::operator >=(const RVersion &version) const
{
    if (this->mMajor > version.mMajor)
    {
        return true;
    }
    else if (this->mMajor == version.mMajor)
    {
        if (this->mMinor > version.mMinor)
        {
            return true;
        }
        else if (this->mMinor == version.mMinor)
        {
            if (this->mReleaseType > version.mReleaseType)
            {
                return true;
            }
            else if (this->mReleaseType == version.mReleaseType)
            {
                if (this->mRelease >= version.mRelease)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool RVersion::operator <=(const RVersion &version) const
{
    if (this->mMajor < version.mMajor)
    {
        return true;
    }
    else if (this->mMajor == version.mMajor)
    {
        if (this->mMinor < version.mMinor)
        {
            return true;
        }
        else if (this->mMinor == version.mMinor)
        {
            if (this->mReleaseType < version.mReleaseType)
            {
                return true;
            }
            else if (this->mReleaseType == version.mReleaseType)
            {
                if (this->mRelease <= version.mRelease)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

unsigned int RVersion::getMajor(void) const
{
    return this->mMajor;
}

unsigned int RVersion::getMinor(void) const
{
    return this->mMinor;
}

unsigned int RVersion::getRelease(void) const
{
    return this->mRelease;
}

RVersion::ReleaseType RVersion::getReleaseType(void) const
{
    return this->mReleaseType;
}

QString RVersion::getReleaseStringFromType(RVersion::ReleaseType releaseType)
{
    switch (releaseType)
    {
        case Alpha:
        {
            return QString("alpha");
        }
        case Beta:
        {
            return QString("beta");
        }
        case ReleaseCandidate:
        {
            return QString("rc");
        }
        case Release:
        default:
        {
            return QString("");
        }
    }
}

RVersion::ReleaseType RVersion::getReleaseTypeFromString(const QString &releaseTypeString)
{
    if (releaseTypeString == "alpha")
    {
        return Alpha;
    }
    if (releaseTypeString == "beta")
    {
        return Beta;
    }
    else if (releaseTypeString == "rc")
    {
        return ReleaseCandidate;
    }
    return Release;
}
