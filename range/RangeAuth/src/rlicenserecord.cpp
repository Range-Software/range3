/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rlicenserecord.cpp                                       *
 *  GROUP:  RAL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   2-nd September 2016                                      *
 *                                                                   *
 *  DESCRIPTION: License record class definition                     *
 *********************************************************************/

#include <QMessageAuthenticationCode>

#include <rblib.h>

#include "rlicenserecord.h"

const char RLicenseRecord::delimiter = ',';
const QString RLicenseRecord::dateFormat("yyyyMMdd");

void RLicenseRecord::_init(const RLicenseRecord *pLicenseRecord)
{
    if (pLicenseRecord)
    {
        this->name = pLicenseRecord->name;
        this->expirationDate = pLicenseRecord->expirationDate;
        this->code = pLicenseRecord->code;
    }
}

RLicenseRecord::RLicenseRecord()
{

}

RLicenseRecord::RLicenseRecord(const QString &name, const QDate &expirationDate, const QString &code)
    : name(name)
    , expirationDate(expirationDate)
    , code(code)
{
    this->_init();
}

RLicenseRecord::RLicenseRecord(const QString &record)
{
    QStringList items = record.split(RLicenseRecord::delimiter,QString::KeepEmptyParts);
    if (items.size() != 3)
    {
        throw RError(R_ERROR_INVALID_INPUT,R_ERROR_REF,"Invalid number of items \'%d\' in license record",items.size());
    }
    if (items.at(0).isEmpty())
    {
        throw RError(R_ERROR_INVALID_INPUT,R_ERROR_REF,"Missing license record name");
    }
    if (items.at(1).isEmpty())
    {
        throw RError(R_ERROR_INVALID_INPUT,R_ERROR_REF,"Missing license record expiry date");
    }
    if (items.at(1).size() != RLicenseRecord::dateFormat.size())
    {
        throw RError(R_ERROR_INVALID_INPUT,R_ERROR_REF,"Invalid license record expiry format \'%s\'",items.at(1).toUtf8().constData());
    }
    this->name = items.at(0);
    this->expirationDate = QDate::fromString(items.at(1),RLicenseRecord::dateFormat);
    this->code = items.at(2);
}

RLicenseRecord::RLicenseRecord(const RLicenseRecord &licenseRecord)
{
    this->_init(&licenseRecord);
}

RLicenseRecord::~RLicenseRecord()
{

}

RLicenseRecord &RLicenseRecord::operator =(const RLicenseRecord &licenseRecord)
{
    this->_init(&licenseRecord);
    return (*this);
}

bool RLicenseRecord::operator ==(const RLicenseRecord &licenseRecord) const
{
    return (this->name == licenseRecord.name &&
            this->expirationDate == licenseRecord.expirationDate &&
            this->code == licenseRecord.code);
}

bool RLicenseRecord::operator !=(const RLicenseRecord &licenseRecord) const
{
    return !(this->operator ==(licenseRecord));
}

const QString &RLicenseRecord::getName(void) const
{
    return this->name;
}

const QDate &RLicenseRecord::getExpirationDate(void) const
{
    return this->expirationDate;
}

void RLicenseRecord::setCode(const QString &code)
{
    this->code = code;
}

QString RLicenseRecord::toString(void) const
{
    QString record;

    record = this->name + RLicenseRecord::delimiter + this->expirationDate.toString(RLicenseRecord::dateFormat) + RLicenseRecord::delimiter + this->code;

    return record;
}

QString RLicenseRecord::generateAuthCode(const QString &account, const QString &password) const
{
    QString softwareKey(RLicenseRecord::generateSoftwareKey());

//    RLogger::info("SK: |%s|\n",softwareKey.toUtf8().constData());

    QByteArray message;
    message.append(this->name + this->expirationDate.toString(RLicenseRecord::dateFormat));

    QByteArray passwordData;
    passwordData.append(password);
    QString passwordHash(QCryptographicHash::hash(passwordData,QCryptographicHash::Md5).toHex());

    QByteArray key;
    key.append(account);
    key.append(passwordHash);
    key.append(softwareKey);

    return QString(QMessageAuthenticationCode::hash(message,key,QCryptographicHash::Sha256).toHex());
}

bool RLicenseRecord::validate(const QString &account, const QString &password) const
{
    return (this->code == this->generateAuthCode(account,password));
}

QString RLicenseRecord::generateSoftwareKey(void)
{
    // min 32
    // max 126
    char v01[16]  = {  32,  64, 106, 125, 114,  38,  45,  50, 111,  76,  42, 123,  82, 121,  45,  96 };
    char v02[16]  = {  33,  51,  68,  83,  49,  79,  83,  76, 122, 118,  59,  87,  33,  66,  81, 112 };
    char v03[16]  = {  59,  67, 112, 106, 120, 124,  42, 121, 114,  97, 118, 101,  72,  81,  98,  63 };
    char v04[16]  = {  68,  85, 124,  38,  42,  68,  95, 101,  43,  45,  90, 109,  92, 122,  55,  84 };
    char v05[16]  = {  73,  51,  70,  74,  65,  42, 125,  75,  55, 113,  36,  57,  96,  72, 111, 124 };
    char v06[16]  = {  46, 100, 103,  50,  67,  78,  83,  37,  80,  54,  38,  89,  75,  74,  66,  73 };
    char v07[16]  = {  76,  90,  43,  62,  96,  99,  56,  91,  66,  60,  78, 125,  65, 106,  64, 125 };
    char v08[16]  = {  60, 106,  83,  57,  93, 115,  33,  47,  80,  89,  97,  33,  58,  33,  60,  45 };
    char v09[16]  = { 120,  97,  63,  95, 116, 107,  99,  70, 105,  46,  60,  56,  40,  70,  64,  50 };
    char v10[16]  = { 122,  41,  49,  59,  69,  37,  69,  47,  59,  91,  74,  46,  41,  75,  65,  48 };
    char v11[16]  = {  61,  72,  99, 104,  80,  92,  86,  57,  55,  54,  37, 125,  34, 120,  79, 105 };
    char v12[16]  = {  98,  93, 113,  62,  49,  98,  53,  70, 115, 118,  82,  59,  60,  44, 101,  42 };
    char v13[16]  = {  90, 118,  94,  79,  92, 125, 102, 118, 112,  64, 106,  73,  60,  52,  42, 106 };
    char v14[16]  = {  60,  41,  72,  50,  41, 110,  94,  41,  89,  99,  73, 112, 115, 125,  97, 120 };
    char v15[16]  = {  51,  48,  59,  64,  34,  82,  83, 110,  37,  92, 102, 100,  77,  40, 112,  59 };
    char v16[16]  = {  55,  76,  65,  47,  53, 107,  55, 102,  41,  68,  35,  96,  43,  66, 111,  86 };
    short key[16] = {   2,   4,   9,  11,  13,  14,   3,  15,  10,   4,  13,   0,  11,   7,   9,  12 };

    QString sKey = QString(v01[key[0]])
                 + QString(v02[key[1]])
                 + QString(v03[key[2]])
                 + QString(v04[key[3]])
//                 + "-"
                 + QString(v05[key[4]])
                 + QString(v06[key[5]])
                 + QString(v07[key[6]])
                 + QString(v08[key[7]])
//                 + "-"
                 + QString(v09[key[8]])
                 + QString(v10[key[9]])
                 + QString(v11[key[10]])
                 + QString(v12[key[11]])
//                 + "-"
                 + QString(v13[key[12]])
                 + QString(v14[key[13]])
                 + QString(v15[key[14]])
                 + QString(v16[key[15]]);

    return sKey;
}
