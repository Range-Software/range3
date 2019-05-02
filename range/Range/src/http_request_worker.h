/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   http_request_worker.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th May 2016                                           *
 *                                                                   *
 *  DESCRIPTION: HTTP request worker class declaration               *
 *********************************************************************/

#ifndef HTTP_REQUEST_WORKER_H
#define HTTP_REQUEST_WORKER_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "http_request_input.h"

class HttpRequestWorker
    : public QObject
{

    Q_OBJECT

    protected:

        QByteArray response;
        QNetworkReply::NetworkError errorType;
        QString errorStr;
        QNetworkAccessManager *manager;
        HttpRequestInput *pRequestInput;

    public:

        //! Constructor.
        explicit HttpRequestWorker(QNetworkAccessManager *networkAccessManager, QObject *parent);

        //! Return const reference to response.
        const QByteArray &getResponse(void) const;

        //! Return error type.
        QNetworkReply::NetworkError getErrorType(void) const;

        //! Return const reference to error message.
        const QString &getError(void) const;

        //! Execute HTTP request.
        void execute(HttpRequestInput *pInput);

    private:

        //! Encode attribute.
        static QString httpAttributeEncode(QString attributeName, QString input);

    signals:

        void finished(void);

    private slots:

        //! Authentication is required.
        void onManagerAuthenticationRequired(QNetworkReply *, QAuthenticator *);

        //! Encrypted connection.
        void onManagerEncrypted(QNetworkReply *);

        //! Finished request.
        void onManagerFinished(QNetworkReply *reply);

        //! Network accessibility has changed.
        void onManagerNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible);

        //! Pre-shared key authentication is required.
        void onManagerPreSharedKeyAuthenticationRequired(QNetworkReply *, QSslPreSharedKeyAuthenticator *);

        //! Proxy authentication is required.
        void onManagerProxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *);

#ifndef QT_NO_SSL
        //! Handle SSL errors.
        void onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
#endif

};

#endif // HTTP_REQUEST_WORKER_H
