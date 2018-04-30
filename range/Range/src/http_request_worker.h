/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   http_request_worker.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th May 2016                                           *
 *                                                                   *
 *  DESCRIPTION: HTTP request worker class declaration               *
 *********************************************************************/

#ifndef __HTTP_REQUEST_WORKER_H__
#define __HTTP_REQUEST_WORKER_H__

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

        void onManagerFinished(QNetworkReply *reply);

};

#endif // __HTTP_REQUEST_WORKER_H__
