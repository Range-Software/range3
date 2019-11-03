/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rra_session.h                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   4-th July 2016                                           *
 *                                                                   *
 *  DESCRIPTION: Remote Range API session class declaration          *
 *********************************************************************/

#ifndef RRA_SESSION_H
#define RRA_SESSION_H

#include <QString>
#include <QObject>
#include <QNetworkAccessManager>

#include <rblib.h>

//#include "rra_client.h"
#include "rra_request_worker.h"

class RRASession : public QObject
{

    Q_OBJECT

    protected:

        static const int relaxInterval;
        static const int timeoutInterval;

        //! Session running flag.
        bool isRunning;
        //! Request running flag.
        bool requestIsRunning;
        //! Available version.
        RVersion availableSoftwareVersion;
        //! Available version.
        QString availableSoftwareLink;
        //! Logged in.
        bool loggedIn;

        //! Next request type.
        RRARequestInput::Type nextRequest;

        //! Network access manager.
        QNetworkAccessManager *networkAccessManager;
        //! RRA Request Worker.
        RRARequestWorker *rraRequestWorker;

    private:

        explicit RRASession(QObject *parent = nullptr);

        static RRARequestInput::Type getNextType(RRARequestInput::Type type);

    public:

        //! Return pointer to RRA Session static instance.
        static RRASession &getInstance(void);

        //! Start session.
        void start(void);

        //! Stop session.
        void stop(void);

        //! Return available software vesion.
        const RVersion &getAvailableSoftwareVersion(void) const;

        //! Return available software link.
        const QString &getAvailableSoftwareLink(void) const;

        //! Submit crash report.
        void submitCrashReport(const QString &crashReport);

    private slots:

        //! Submit next request.
        void submitNextRequest(void);

        //! Catch latest software version.
        void onRraAvailableSoftware(const RVersion &version, const QString &link, const QString &responseMessage);

        //! Catch login status.
        void onRraLoginStatus(const QString &status, bool loggedIn, const QString &responseMessage);

        //! Catch RRA failed signal.
        void onRraFailed(RRARequestInput::Type type, const QString &errorMessage);

    signals:

        //! RRA Session has started.
        void started(void);

        //! RRA Session has stopped.
        void stopped(void);

        //! RRA Request failed.
        void failed(const QString &errorMessage);

        //! software version available.
        void availableSoftware(const RVersion &version, const QString &link);

        //! User signed in.
        void signedIn(void);

        //! User signed out.
        void signedOut(void);

};

#endif // RRA_SESSION_H
