/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   download_manager.cpp                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   9-th May 2016                                            *
 *                                                                   *
 *  DESCRIPTION: Download manager class definition                   *
 *********************************************************************/

#include <QUrlQuery>
#include <QFileInfo>
#include <QDir>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QStringList>
#include <QTimer>

#include <rblib.h>
#include <rmlib.h>

#include "download_manager.h"
#include "main_settings.h"

DownloadManager::DownloadManager(QObject *parent)
    : QObject(parent)
    , downloadID(0)
    , downloadedCount(0)
    , totalCount(0)
{
}

uint DownloadManager::append(const QUrl &url)
{
    if (this->downloadQueue.isEmpty())
    {
        QTimer::singleShot(0, this, SLOT(startNextDownload()));
    }

    this->downloadQueue.enqueue(url);
    this->totalCount++;
    return ++this->downloadID;
}

QString DownloadManager::saveFileNameFromUrl(const QUrl &url)
{
    QString basename = QFileInfo(url.path()).fileName();

    if (basename.isEmpty())
    {
        basename = QUrlQuery(url).queryItemValue("file");
    }

    if (basename.isEmpty())
    {        
        basename = "download";
    }

    basename = QDir(MainSettings::getInstance().getTmpDir()).filePath(basename);

    if (QFile::exists(basename))
    {
        // already exists, don't overwrite
        int i = 1;
        while (QFile::exists(RFileManager::getFileNameWithTimeStep(basename,i)))
        {
            ++i;
        }

        basename = RFileManager::getFileNameWithTimeStep(basename,i);
    }

    return basename;
}

void DownloadManager::startNextDownload(void)
{
    emit this->started(this->downloadID);
    if (this->downloadQueue.isEmpty())
    {
        RLogger::info("%d/%d files downloaded successfully\n", this->downloadedCount, this->totalCount);
        return;
    }

    QUrl url = this->downloadQueue.dequeue();

    QString filename = DownloadManager::saveFileNameFromUrl(url);
    RLogger::info("Writing to file \'%s\'\n",filename.toUtf8().constData());
    this->output.setFileName(filename);
    if (!this->output.open(QIODevice::WriteOnly))
    {
        RLogger::warning("Problem opening save file '%s' for download '%s': %s\n",
                         qPrintable(filename),
                         url.toEncoded().constData(),
                         qPrintable(this->output.errorString()));
        emit this->failed(this->downloadID);

        this->startNextDownload();
        return;                 // skip this download
    }

    QNetworkRequest request(url);
    this->currentDownload = this->manager.get(request);
    connect(this->currentDownload, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64,qint64)));
    connect(this->currentDownload, SIGNAL(finished()), SLOT(downloadFinished()));
    connect(this->currentDownload, SIGNAL(readyRead()), SLOT(downloadReadyRead()));

    // prepare the output
    RLogger::info("Downloading %s...\n", url.toEncoded().constData());
    this->downloadTime.start();
}

void DownloadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    // calculate the download speed
    double speed = bytesReceived * 1000.0 / this->downloadTime.elapsed();
    emit this->progress(this->downloadID,bytesReceived, bytesTotal,speed);
}

void DownloadManager::downloadFinished(void)
{
    QString fileName(this->output.fileName());

    //progressBar.clear();
    this->output.close();

    if (this->currentDownload->error())
    {
        // download failed
        RLogger::warning("Failed: %s\n", qPrintable(this->currentDownload->errorString()));
        emit this->failed(this->downloadID);
    }
    else
    {
        RLogger::info("Downloaded file saved as \'%s\'.\n",fileName.toUtf8().constData());
        this->downloadedCount++;
        emit this->fileSaved(this->downloadID,fileName);
    }
    emit this->finished(this->downloadID);

    this->currentDownload->deleteLater();
    this->startNextDownload();
}

void DownloadManager::downloadReadyRead(void)
{
    output.write(this->currentDownload->readAll());
}
