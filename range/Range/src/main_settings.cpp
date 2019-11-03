/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   main_settings.cpp                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   12-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Main settings class definition                      *
 *********************************************************************/

#include <QDir>
#include <QDesktopServices>
#include <QImageWriter>

#include <rblib.h>

#include "main_settings.h"

const QString MainSettings::logFileName("range.log");
const QString MainSettings::licenseFileName("LICENSE.txt");
const QString MainSettings::releaseNotesFileName("RELEASE_NOTES.txt");

MainSettings::MainSettings(const QString &organization,
                           const QString &application,
                           QObject       *parent)
    : QSettings(organization,application,parent)
{
    this->applicationSettings = new ApplicationSettings(this);
    this->sync();
    this->storedVersion = RVersion(this->value("version").toString());
    this->setValue("version",QVariant(RVendor::version.toString()));
}

MainSettings &MainSettings::getInstance(void)
{
    static MainSettings mainSettings(RVendor::name, RVendor::shortName);
    return mainSettings;
}

MainSettings * MainSettings::getInstancePtr(void)
{
    return &MainSettings::getInstance();
}

void MainSettings::setDirApplicationPath(const QString &applicationPath)
{
    this->applicationDirPath = applicationPath;
}

QString MainSettings::getHomeDir(void) const
{
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
}

QString MainSettings::getRangeDir(void) const
{
    return this->getDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),RVendor::name);
}

QString MainSettings::getDataDir(void) const
{
    return this->getDir(this->getRangeDir(),"data");
}

QString MainSettings::getMaterialsDir(void) const
{
    return this->getDir(this->getRangeDir(),"materials");
}

QString MainSettings::getDocDir(void) const
{
    return this->getDir(this->getRangeDir(),"doc");
}

QString MainSettings::getLogDir(void) const
{
    return this->getDir(this->getRangeDir(),"log");
}

QString MainSettings::getTmpDir(void) const
{
    return this->getDir(this->getRangeDir(),"tmp");
}

QString MainSettings::getSessionDir(void) const
{
    return this->getDir(this->getRangeDir(),"session");
}

QString MainSettings::getSessionFileName(void) const
{
    return this->value("session/fileName").toString();
}

QString MainSettings::getLogFileName() const
{
    return this->getLogDir() + QDir::separator() + MainSettings::logFileName;
}

QString MainSettings::findLicenseFileName(void) const
{
    QDir dir(this->applicationDirPath);
    dir.cdUp();
    dir.cd("doc");
    return dir.filePath(MainSettings::licenseFileName);
}

QString MainSettings::findReleaseNotesFileName(void) const
{
    QDir dir(this->applicationDirPath);
    dir.cdUp();
    dir.cd("doc");
    return dir.filePath(MainSettings::releaseNotesFileName);
}

QString MainSettings::findHelpDir(void) const
{
    QDir dir(this->applicationDirPath);
    dir.cdUp();
    return dir.filePath("help");
}

QString MainSettings::findPixmapsDir(void) const
{
    QDir dir(this->applicationDirPath);
    dir.cdUp();
    return dir.filePath("pixmaps");
}

const ApplicationSettings *MainSettings::getApplicationSettings(void) const
{
    return this->applicationSettings;
}

ApplicationSettings *MainSettings::getApplicationSettings(void)
{
    return this->applicationSettings;
}

const RVersion &MainSettings::getStoredVersion(void) const
{
    return this->storedVersion;
}

QList<QString> MainSettings::getSupportedImageFormats(void)
{
    QList<QString> formats;

    QList<QByteArray> supportedFormats = QImageWriter::supportedImageFormats();
    foreach (QString format, supportedFormats)
    {
        formats.append(format.toLower());
    }

    return formats;
}

QList<QString> MainSettings::getSupportedVideoFormats(void)
{
    QList<QString> formats;

    formats.append("avi");
    formats.append("mpeg");
    formats.append("ogg");

    return formats;
}

QString MainSettings::getDefaultVideoFormat(void)
{
    return QString("avi");
}

QList<QString> MainSettings::getSupportedDocumentFormats(void)
{
    QList<QString> formats;

    formats.append("txt");
    formats.append("log");
    formats.append("pdf");
    formats.append("odf");
    formats.append("html");

    return formats;
}

QString MainSettings::getDir(QString topDir, QString subDir)
{
    QDir dir(topDir);

    if (!dir.exists(subDir))
    {
        QString fullPath(dir.filePath(subDir));
        RLogger::info("Creating directory \'%s\'\n",fullPath.toUtf8().constData());
        if (!dir.mkpath(fullPath))
        {
            RLogger::error("Failed to create a directory \'%s\'\n",fullPath.toUtf8().constData());
        }
    }
    if (!dir.cd(subDir))
    {
        RLogger::error("Failed to change to a sub-directory \'%s\' in \'%s\'\n",
                  subDir.toUtf8().constData(),
                  dir.path().toUtf8().constData());
    }

    return dir.path();
}
