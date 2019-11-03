/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   main_settings.h                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   12-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Main settings class declaration                     *
 *********************************************************************/

#ifndef MAIN_SETTINGS_H
#define MAIN_SETTINGS_H

#include <QSettings>
#include <QString>

#include <rblib.h>

#include "application_settings.h"

class MainSettings : public QSettings
{

    Q_OBJECT

    protected:

        //! Log file name without path.
        static const QString logFileName;
        //! License file name without path.
        static const QString licenseFileName;
        //! Release notes file name without path.
        static const QString releaseNotesFileName;

        //! Application path.
        QString applicationDirPath;
        //! Application settings.
        ApplicationSettings *applicationSettings;
        //! Stored version.
        RVersion storedVersion;

    public:
        //! Constructor.
        explicit MainSettings(const QString &organization,
                              const QString &application = QString(),
                              QObject       *parent = nullptr);

        //! Return pointer to main settings static instance.
        static MainSettings & getInstance(void);

        //! Return pointer to main settings static instance.
        static MainSettings * getInstancePtr(void);

        //! Set application path.
        void setDirApplicationPath(const QString &applicationDirPath);

        //! Return users home directory path.
        QString getHomeDir(void) const;

        //! Return users range directory path.
        QString getRangeDir(void) const;

        //! Return users data directory path.
        QString getDataDir(void) const;

        //! Return users materials directory path.
        QString getMaterialsDir(void) const;

        //! Return users document directory path.
        QString getDocDir(void) const;

        //! Return users log directory path.
        QString getLogDir(void) const;

        //! Return users temporary directory path.
        QString getTmpDir(void) const;

        //! Return users session directory path.
        QString getSessionDir(void) const;

        //! Return session file name.
        QString getSessionFileName(void) const;

        //! Return log file name.
        QString getLogFileName(void) const;

        //! Return license file name.
        QString findLicenseFileName(void) const;

        //! Return release notes file name.
        QString findReleaseNotesFileName(void) const;

        //! Return help directory path.
        QString findHelpDir(void) const;

        //! Return pixmaps directory path.
        QString findPixmapsDir(void) const;

        //! Return const pointer to application settings.
        const ApplicationSettings * getApplicationSettings(void) const;

        //! Return pointer to application settings.
        ApplicationSettings * getApplicationSettings(void);

        //! Return const reference to stored version.
        const RVersion &getStoredVersion(void) const;

        //! Return supported image formats (extensions).
        static QList<QString> getSupportedImageFormats(void);

        //! Return supported video formats (extensions).
        static QList<QString> getSupportedVideoFormats(void);

        //! Return default video format (extensions).
        static QString getDefaultVideoFormat(void);

        //! Return supported document formats (extensions).
        static QList<QString> getSupportedDocumentFormats(void);

        //! Return and construct directory path.
        static QString getDir(QString topDir, QString subDir);

};

#endif /* MAIN_SETTINGS_H */
