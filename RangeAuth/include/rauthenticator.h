/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rauthenticator.h                                         *
 *  GROUP:  RAL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   30-th August 2016                                        *
 *                                                                   *
 *  DESCRIPTION: Authenticator class declaration                     *
 *********************************************************************/

#ifndef __RAUTHENTICATOR_H__
#define __RAUTHENTICATOR_H__

#include <QString>
#include <QList>

class RAuthenticator
{

    public:

        enum Area
        {
            Acoustics        = 1 << 0,
            Contaminant      = 1 << 1,
            Electrostatics   = 1 << 2,
            Magnetostatics   = 1 << 3,
            Fluid            = 1 << 4,
            FluidHeat        = 1 << 5,
            Heat             = 1 << 6,
            RadiativeHeat    = 1 << 7,
            Stress           = 1 << 8,
            StressModal      = 1 << 9,
            Potential        = 1 << 10,
            Wave             = 1 << 11
//            All              = Acoustics
//                             | Contaminant
//                             | Electrostatics
//                             | Magnetostatics
//                             | Fluid
//                             | FluidHeat
//                             | Heat
//                             | RadiativeHeat
//                             | Stress
//                             | StressModal
//                             | Potential
//                             | Wave
        };

    protected:

        //! License file.
        QString licenseFile;
        //! Account.
        QString account;
        //! Password.
        QString password;

    private:

        //! Internal initialization function.
        void _init(const RAuthenticator *pAuthenticator = 0);

    public:

        //! Constructor.
        RAuthenticator(const QString &licenseFile, const QString &account, const QString &password);

        //! Copy constructor.
        RAuthenticator(const RAuthenticator &authenticator);

        //! Destructor.
        ~RAuthenticator();

        //! Copy operator.
        RAuthenticator &operator =(const RAuthenticator &authenticator);

        //! Set license file.
        void setLicenseFile(const QString &licenseFile);

        //! Set account.
        void setAccount(const QString &account);

        //! Set password.
        void setPassword(const QString &password);

        //! Authenticate area.
        bool authenticate(Area area) const;

        //! Return list of all authentication areas.
        static QList<Area> getAreaList(void);

        //! Return list of authentication areas in mask.
        static QList<Area> getAreaList(Area mask);

        //! Return name for given area.
        static QString getName(Area area);

};

#endif // __RAUTHENTICATOR_H__
