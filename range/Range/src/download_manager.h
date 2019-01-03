/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   download_manager.h                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   9-th May 2016                                            *
 *                                                                   *
 *  DESCRIPTION: Download manager class declaration                  *
 *********************************************************************/

#ifndef DOWNLOAD_MANAGER_H
#define DOWNLOAD_MANAGER_H

#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>

class DownloadManager: public QObject
{

    Q_OBJECT

    private:

        QNetworkAccessManager manager;
        QQueue<QUrl> downloadQueue;
        QNetworkReply *currentDownload;
        QFile output;
        QTime downloadTime;

        uint downloadID;
        uint downloadedCount;
        uint totalCount;

    public:

        DownloadManager(QObject *parent = nullptr);

        uint append(const QUrl &url);
        static QString saveFileNameFromUrl(const QUrl &url);

    signals:

        void progress(uint downloadID, qint64 bytesReceived, qint64 bytesTotal, double speed);
        void started(uint downloadID);
        void finished(uint downloadID);
        void failed(uint downloadID);
        void fileSaved(uint downloadID, const QString &fileName);

    private slots:

        void startNextDownload(void);
        void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
        void downloadFinished(void);
        void downloadReadyRead(void);

};

#endif // DOWNLOAD_MANAGER_H
