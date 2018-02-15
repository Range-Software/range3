/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rlicense.cpp                                             *
 *  GROUP:  RAL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   6-th September 2016                                      *
 *                                                                   *
 *  DESCRIPTION: License class definition                            *
 *********************************************************************/

#include <QFile>
#include <QTextStream>

#include <rblib.h>

#include "rlicense.h"

void RLicense::_init(const RLicense *pLicense)
{
    if (pLicense)
    {
        this->records = pLicense->records;
    }
}

RLicense::RLicense()
{
    this->_init();
}

RLicense::RLicense(const RLicense &license)
{
    this->_init(&license);
}

RLicense::RLicense(const QString &licenseTxt)
{
    QString licenseTxtTmp(licenseTxt);
    QTextStream textStream(&licenseTxtTmp,QIODevice::ReadOnly);
    this->read(textStream);
}

RLicense::~RLicense()
{

}

RLicense &RLicense::operator =(const RLicense &license)
{
    this->_init(&license);
    return (*this);
}

bool RLicense::operator ==(const RLicense &license) const
{
    return this->records == license.records;
}

bool RLicense::operator !=(const RLicense &license) const
{
    return this->records != license.records;
}

void RLicense::addRecord(const RLicenseRecord &licenseRecord)
{
    this->records.insert(licenseRecord.getName(),licenseRecord);
}

void RLicense::read(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to open the file for reading.");
    }

    QTextStream textStream(&file);
    this->read(textStream);

    file.close();
}

void RLicense::write(const QString &fileName) const
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to open the file for writing.");
    }

    QTextStream textStream(&file);

    foreach (const RLicenseRecord &record, this->records)
    {
        textStream << record.toString() << "\n";
    }

    file.close();
}

bool RLicense::validateRecord(const QString &recordName, const QString &account, const QString &password) const
{
    if (this->records.contains(recordName))
    {
        return this->records.value(recordName).validate(account,password);
    }

    return false;
}

bool RLicense::checkRecordExpiry(const QString &recordName, uint nMonthsToExpire) const
{
    if (this->records.contains(recordName))
    {
        return this->records.value(recordName).checkExpiry(nMonthsToExpire);
    }
    return false;
}

QDate RLicense::findRecordExpirationDate(const QString &recordName) const
{
    if (this->records.contains(recordName))
    {
        return this->records.value(recordName).getExpirationDate();
    }
    else
    {
        return QDate();
    }
}

QString RLicense::toString(void) const
{
    QString licenseTxt;
    bool isFirst = true;
    foreach (const RLicenseRecord &licenseRecord, this->records)
    {
        if (!isFirst)
        {
            licenseTxt += "\n";
        }
        licenseTxt += licenseRecord.toString();
        isFirst = false;
    }
    return licenseTxt;
}

void RLicense::read(QTextStream &textStream)
{
    while (!textStream.atEnd())
    {
        QString line(textStream.readLine());
        try
        {
            RLicenseRecord record(line);
            this->records.insert(record.getName(),record);
        }
        catch (const RError &rError)
        {
            RLogger::warning("Invalid license record \'%s\'. %s\n",line.toUtf8().constData(),rError.getMessage().toUtf8().constData());
        }
        catch (const std::exception &error)
        {
            RLogger::warning("Error while reading license file. %s\n",error.what());
        }
    }
}
