/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_version.h                                            *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   16-th August 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Version class declaration                           *
 *********************************************************************/

#ifndef RBL_VERSION_H
#define RBL_VERSION_H

#include <QString>

class RVersion
{

    protected:

        //! Release type separator string.
        static const QChar releaseTypeSeparator;

    public:

        //! Release type.
        enum ReleaseType
        {
            Alpha = 0,
            Beta,
            ReleaseCandidate,
            Release
        };

    protected:

        //! Major number.
        unsigned int mMajor;
        //! Minor number.
        unsigned int mMinor;
        //! Release number.
        unsigned int mRelease;
        //! Release type.
        ReleaseType mReleaseType;

    private:

        //! Internal initialization function.
        void _init ( const RVersion *pVersion = nullptr );

    public:

        //! Constructor.
        RVersion ();

        //! Constructor.
        RVersion (unsigned int mMajor, unsigned int mMinor, unsigned int mRelease, ReleaseType mReleaseType = Release);

        //! Constructor.
        RVersion (const QString &versionStr, char delimiter = '.');

        //! Copy constructor.
        RVersion ( const RVersion &version );

        //! Destructor.
        ~RVersion ();

        //! Return true is version is valid (0.0.0 will return false)
        inline bool isValid(void) const
        {
            return (this->mMajor != 0 || this->mMinor != 0 || this->mRelease != 0);
        }

        //! Convert version to string.
        QString toString(char delimiter = '.') const;

        //! Assignment operator.
        RVersion & operator = ( const RVersion &version );

        //! Equal to operator.
        bool operator ==(const RVersion& version) const;

        //! Not equal to operator.
        bool operator !=(const RVersion& version) const;

        //! Greater than operator.
        bool operator >(const RVersion& version) const;

        //! Less than operator.
        bool operator <(const RVersion& version) const;

        //! Greater than or equal to operator.
        bool operator >=(const RVersion& version) const;

        //! Less than or equal to operator.
        bool operator <=(const RVersion& version) const;

        //! Return major number.
        unsigned int getMajor(void) const;

        //! Return minor number.
        unsigned int getMinor(void) const;

        //! Return release number.
        unsigned int getRelease(void) const;

        //! Return release number.
        ReleaseType getReleaseType(void) const;

        //! Convert release type to string.
        static QString getReleaseStringFromType(ReleaseType releaseType);

        //! Convert release string to type.
        static ReleaseType getReleaseTypeFromString(const QString &releaseTypeString);

};

#endif /* RBL_VERSION_H */
