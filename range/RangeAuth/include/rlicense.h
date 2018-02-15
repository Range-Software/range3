/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rlicense.h                                               *
 *  GROUP:  RAL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th September 2016                                      *
 *                                                                   *
 *  DESCRIPTION: License class declaration                           *
 *********************************************************************/

#ifndef __RLICENSE_H__
#define __RLICENSE_H__

#include <QMap>
#include <QTextStream>

#include "rlicenserecord.h"

class RLicense
{

    protected:

        //! License records.
        QMap<QString,RLicenseRecord> records;

    private:

        //! Internal initialization function.
        void _init(const RLicense *pLicense = 0);

    public:

        //! Constructor.
        RLicense();

        //! Copy constructor.
        RLicense(const RLicense &license);

        //! Convert constructor.
        RLicense(const QString &licenseTxt);

        //! Destructor.
        ~RLicense();

        //! Copy operator.
        RLicense &operator =(const RLicense &license);

        //! Equal operator.
        bool operator ==(const RLicense &license) const;

        //! Not equal operator.
        bool operator !=(const RLicense &license) const;

        //! Add license record.
        void addRecord(const RLicenseRecord &licenseRecord);

        //! Read from file.
        void read(const QString &fileName);

        //! Write to file.
        void write(const QString &fileName) const;

        //! Validate license record with given name.
        bool validateRecord(const QString &recordName, const QString &account, const QString &password) const;

        //! Check license record with given name whether its expiry fits in specified number of months from now.
        bool checkRecordExpiry(const QString &recordName, uint nMonthsToExpire) const;

        //! Find record expiry date.
        QDate findRecordExpirationDate(const QString &recordName) const;

        //! Return record string.
        QString toString(void) const;

    protected:

        //! Read from text stream.
        void read(QTextStream &textStream);

};

#endif // __RLICENSE_H__
