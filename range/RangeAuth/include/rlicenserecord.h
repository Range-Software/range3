/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rlicenserecord.h                                         *
 *  GROUP:  RAL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   2-nd September 2016                                      *
 *                                                                   *
 *  DESCRIPTION: License record class declaration                    *
 *********************************************************************/

#ifndef __RLICENSERECORD_H__
#define __RLICENSERECORD_H__

#include <QString>
#include <QDate>

class RLicenseRecord
{

    protected:

        //! Delimiter.
        static const char delimiter;
        //! Date format.
        static const QString dateFormat;

    protected:

        //! License name.
        QString name;
        //! License expiration key.
        QDate expirationDate;
        //! License code.
        QString code;

    private:

        //! Internal initialization function.
        void _init(const RLicenseRecord *pLicenseRecord = 0);

    public:

        //! Constructor.
        RLicenseRecord();

        //! Constructor.
        RLicenseRecord(const QString &name, const QDate &expirationDate, const QString &code);

        //! Constructor.
        RLicenseRecord(const QString &record);

        //! Copy constructor.
        RLicenseRecord(const RLicenseRecord &licenseRecord);

        //! Destructor.
        ~RLicenseRecord();

        //! Copy operator.
        RLicenseRecord &operator =(const RLicenseRecord &licenseRecord);

        //! Equal operator.
        bool operator ==(const RLicenseRecord &licenseRecord) const;

        //! Not equal operator.
        bool operator !=(const RLicenseRecord &licenseRecord) const;

        //! Return const reference to license record name.
        const QString &getName(void) const;

        //! Return const reference to expiration date.
        const QDate &getExpirationDate(void) const;

        //! Set license code.
        void setCode(const QString &code);

        //! Return record string.
        QString toString(void) const;

        //! Generate authentication code.
        QString generateAuthCode(const QString &account, const QString &password) const;

        //! Validate record.
        bool validate(const QString &account, const QString &password) const;

        //! Check record for expiry.
        bool checkExpiry(uint nMonthsToExpire) const;

        //! Generate software key.
        static QString generateSoftwareKey(void);

};

#endif // __RLICENSERECORD_H__
