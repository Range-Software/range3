/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rra_request_worker.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   14-th November 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Remote Range API request worker class declaration   *
 *********************************************************************/

#ifndef RRA_REQUEST_WORKER_H
#define RRA_REQUEST_WORKER_H

#include <QNetworkAccessManager>

#include <rblib.h>

#include "http_request_worker.h"
#include "rra_request_input.h"

class RRARequestWorker : public HttpRequestWorker
{

    Q_OBJECT

    protected:

        //! Request type.
        RRARequestInput::Type requestType;

    public:

        //! Constructor.
        explicit RRARequestWorker(QNetworkAccessManager *networkAccessManager, QObject *parent = nullptr);

        //! Request available software.
        void availableSoftware(void);

        //! Log in.
        void logIn(const QString &account, const QString &password);

        //! Log out.
        void logOut(const QString &account);

        //! Send usage info.
        void sendUsageInfo(const QString &usageInfo);

        //! Send crash report.
        void sendCrashReport(const QString &crashReport);

    private slots:

        //! Handle HTTP Request finished signal.
        void onHttpRequestFinished(void);

    private:

        //! Process JSon output.
        static bool processJSon(const QByteArray &byteArray,
                                QString &html,
                                QString &status,
                                QStringList &messages,
                                QStringList &warnings,
                                QStringList &errors);

    signals:

        //! Request failed.
        void failed(RRARequestInput::Type requestType, const QString &errorMessage);

        //! Available software version request is complete.
        void availableSoftware(const RVersion &version, const QString &link, const QString &responseMessage);

        //! Login status.
        void loginStatus(const QString &status, bool loggedIn, const QString &responseMessage);

        //! Usage request is complete.
        void usage(const QString &data, const QString &responseMessage);

        //! Crash report is complete.
        void crashReport(const QString &data, const QString &responseMessage);

};

#endif // RRA_REQUEST_WORKER_H
