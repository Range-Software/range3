/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   http_request_worker.cpp                                  *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   26-th May 2016                                           *
 *                                                                   *
 *  DESCRIPTION: HTTP request worker class definition                *
 *********************************************************************/

#include <QDateTime>
#include <QUrl>
#include <QFileInfo>
#include <QBuffer>
#include <QAuthenticator>
#include <QNetworkProxyFactory>

#include <rblib.h>

#include "http_request_worker.h"

HttpRequestWorker::HttpRequestWorker(QNetworkAccessManager *networkAccessManager, QObject *parent)
    : QObject(parent)
    , manager(networkAccessManager)
{
    qsrand(QDateTime::currentDateTime().toTime_t());

    QNetworkProxyFactory *pProxyFactory = this->manager->proxyFactory();
    pProxyFactory->setUseSystemConfiguration(true);

    QObject::connect(this->manager,
                     &QNetworkAccessManager::authenticationRequired,
                     this,
                     &HttpRequestWorker::onManagerAuthenticationRequired);

    QObject::connect(this->manager,
                     &QNetworkAccessManager::encrypted,
                     this,
                     &HttpRequestWorker::onManagerEncrypted);

    QObject::connect(this->manager,
                     &QNetworkAccessManager::finished,
                     this,
                     &HttpRequestWorker::onManagerFinished);

    QObject::connect(this->manager,
                     &QNetworkAccessManager::networkAccessibleChanged,
                     this,
                     &HttpRequestWorker::onManagerNetworkAccessibleChanged);

    QObject::connect(this->manager,
                     &QNetworkAccessManager::preSharedKeyAuthenticationRequired,
                     this,
                     &HttpRequestWorker::onManagerPreSharedKeyAuthenticationRequired);

    QObject::connect(this->manager,
                     &QNetworkAccessManager::proxyAuthenticationRequired,
                     this,
                     &HttpRequestWorker::onManagerProxyAuthenticationRequired);

#ifndef QT_NO_SSL
    QObject::connect(this->manager,
                     &QNetworkAccessManager::sslErrors,
                     this,
                     &HttpRequestWorker::onSslErrors);
#endif
}

const QByteArray &HttpRequestWorker::getResponse(void) const
{
    return this->response;
}

QNetworkReply::NetworkError HttpRequestWorker::getErrorType() const
{
    return this->errorType;
}

const QString &HttpRequestWorker::getError(void) const
{
    return this->errorStr;
}

void HttpRequestWorker::execute(HttpRequestInput *pInput)
{
    this->pRequestInput = pInput;

    // reset variables

    QByteArray requestContent = "";
    this->response = "";
    this->errorType = QNetworkReply::NoError;
    this->errorStr = "";

    QMap<QString, QString> &rVariables = this->pRequestInput->getVariableMap();
    QList<HttpRequestInput::FileElement> &rFileList = this->pRequestInput->getFileList();

    // decide on the variable layout

    if (rFileList.length() > 0)
    {
        this->pRequestInput->setVariableLayout(HttpRequestInput::MULTIPART);
    }
    if (this->pRequestInput->getVariableLayout() == HttpRequestInput::NOT_SET)
    {
        this->pRequestInput->setVariableLayout(this->pRequestInput->getHttpMethod() == "GET" || this->pRequestInput->getHttpMethod() == "HEAD" ? HttpRequestInput::ADDRESS : HttpRequestInput::URL_ENCODED);
    }


    // prepare request content

    QString boundary = "";

    if (this->pRequestInput->getVariableLayout() == HttpRequestInput::ADDRESS || this->pRequestInput->getVariableLayout() == HttpRequestInput::URL_ENCODED)
    {
        // variable layout is ADDRESS or URL_ENCODED

        if (rVariables.count() > 0)
        {
            bool first = true;
            foreach (QString key, rVariables.keys())
            {
                if (!first)
                {
                    requestContent.append("&");
                }
                first = false;

                requestContent.append(QUrl::toPercentEncoding(key));
                requestContent.append("=");
                requestContent.append(QUrl::toPercentEncoding(rVariables.value(key)));
            }

            if (this->pRequestInput->getVariableLayout() == HttpRequestInput::ADDRESS)
            {
                this->pRequestInput->getUrlStr() += "?" + requestContent;
                requestContent = "";
            }
        }
    }
    else
    {
        // variable layout is MULTIPART

        boundary = "__-----------------------"
            + QString::number(QDateTime::currentDateTime().toTime_t())
            + QString::number(qrand());
        QString boundary_delimiter = "--";
        QString new_line = "\r\n";

        // add variables
        foreach (QString key, rVariables.keys())
        {
            // add boundary
            requestContent.append(boundary_delimiter);
            requestContent.append(boundary);
            requestContent.append(new_line);

            // add header
            requestContent.append("Content-Disposition: form-data; ");
            requestContent.append(HttpRequestWorker::httpAttributeEncode("name", key));
            requestContent.append(new_line);
            requestContent.append("Content-Type: text/plain");
            requestContent.append(new_line);

            // add header to body splitter
            requestContent.append(new_line);

            // add variable content
            requestContent.append(rVariables.value(key));
            requestContent.append(new_line);
        }

        // add files
        for (QList<HttpRequestInput::FileElement>::iterator fileIterator = rFileList.begin(); fileIterator != rFileList.end(); fileIterator++)
        {
            QFileInfo fi(fileIterator->localFilename);

            // ensure necessary variables are available
            if (
                fileIterator->localFilename == nullptr || fileIterator->localFilename.isEmpty()
                || fileIterator->variableName == nullptr || fileIterator->variableName.isEmpty()
                || !fi.exists() || !fi.isFile() || !fi.isReadable()
            )
            {
                // silent abort for the current file
                continue;
            }

            QFile file(fileIterator->localFilename);
            if (!file.open(QIODevice::ReadOnly))
            {
                // silent abort for the current file
                continue;
            }

            // ensure filename for the request
            if (fileIterator->requestFilename == nullptr || fileIterator->requestFilename.isEmpty())
            {
                fileIterator->requestFilename = fi.fileName();
                if (fileIterator->requestFilename.isEmpty())
                {
                    fileIterator->requestFilename = "file";
                }
            }

            // add boundary
            requestContent.append(boundary_delimiter);
            requestContent.append(boundary);
            requestContent.append(new_line);

            // add header
            requestContent.append(QString("Content-Disposition: form-data; %1; %2").arg(
                HttpRequestWorker::httpAttributeEncode("name", fileIterator->variableName),
                HttpRequestWorker::httpAttributeEncode("filename", fileIterator->requestFilename)
            ));
            requestContent.append(new_line);

            if (fileIterator->mimeType != nullptr && !fileIterator->mimeType.isEmpty())
            {
                requestContent.append("Content-Type: ");
                requestContent.append(fileIterator->mimeType);
                requestContent.append(new_line);
            }

            requestContent.append("Content-Transfer-Encoding: binary");
            requestContent.append(new_line);

            // add header to body splitter
            requestContent.append(new_line);

            // add file content
            requestContent.append(file.readAll());
            requestContent.append(new_line);

            file.close();
        }

        // add end of body
        requestContent.append(boundary_delimiter);
        requestContent.append(boundary);
        requestContent.append(boundary_delimiter);
    }

    // prepare connection
    QNetworkRequest request = QNetworkRequest(QUrl(this->pRequestInput->getUrlStr()));
    request.setRawHeader("User-Agent", "Agent name goes here");

    if (this->pRequestInput->getVariableLayout() == HttpRequestInput::URL_ENCODED)
    {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    }
    else if (this->pRequestInput->getVariableLayout() == HttpRequestInput::MULTIPART)
    {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + boundary);
    }

    if (this->pRequestInput->getHttpMethod() == "GET")
    {
        this->manager->get(request);
    }
    else if (this->pRequestInput->getHttpMethod() == "POST")
    {
        this->manager->post(request, requestContent);
    }
    else if (this->pRequestInput->getHttpMethod() == "PUT")
    {
        this->manager->put(request, requestContent);
    }
    else if (this->pRequestInput->getHttpMethod() == "HEAD")
    {
        this->manager->head(request);
    }
    else if (this->pRequestInput->getHttpMethod() == "DELETE")
    {
        this->manager->deleteResource(request);
    }
    else
    {
        QBuffer buff(&requestContent);
        this->manager->sendCustomRequest(request, this->pRequestInput->getHttpMethod().toLatin1(), &buff);
    }
}

QString HttpRequestWorker::httpAttributeEncode(QString attributeName, QString input)
{
    // result structure follows RFC 5987
    bool needUtfEncoding = false;
    QString result = "";
    QByteArray input_c = input.toLocal8Bit();
    char c;
    for (int i = 0; i < input_c.length(); i++)
    {
        c = input_c.at(i);
        if (c == '\\' || c == '/' || c == '\0' || c < ' ' || c > '~')
        {
            // ignore and request utf-8 version
            needUtfEncoding = true;
        }
        else if (c == '"')
        {
            result += "\\\"";
        }
        else
        {
            result += c;
        }
    }

    if (result.length() == 0)
    {
        needUtfEncoding = true;
    }

    if (!needUtfEncoding)
    {
        // return simple version
        return QString("%1=\"%2\"").arg(attributeName, result);
    }

    QString resultUtf8 = "";
    for (int i = 0; i < input_c.length(); i++)
    {
        c = input_c.at(i);
        if (
            (c >= '0' && c <= '9')
            || (c >= 'A' && c <= 'Z')
            || (c >= 'a' && c <= 'z')
        )
        {
            resultUtf8 += c;
        }
        else
        {
            resultUtf8 += "%" + QString::number(static_cast<unsigned char>(input_c.at(i)), 16).toUpper();
        }
    }

    // return enhanced version with UTF-8 support
    return QString("%1=\"%2\"; %1*=utf-8''%3").arg(attributeName, result, resultUtf8);
}

void HttpRequestWorker::onManagerAuthenticationRequired(QNetworkReply *, QAuthenticator *)
{
    RLogger::warning("Authentication is required.\n");
}

void HttpRequestWorker::onManagerEncrypted(QNetworkReply *)
{
    RLogger::info("Encrypted connection.\n");
}

void HttpRequestWorker::onManagerFinished(QNetworkReply *reply)
{
    if (!reply)
    {
        // No reply provided
        RLogger::error("HTTP request without a reply.\n");
        this->errorType = QNetworkReply::UnknownServerError;
        delete this->pRequestInput;
        this->pRequestInput = nullptr;
        emit this->finished();
        return;
    }

    this->errorType = reply->error();

    if (this->errorType)
    {
        // Error occured
        this->errorStr = reply->errorString();

        reply->deleteLater();
        reply = nullptr;
        delete this->pRequestInput;
        this->pRequestInput = nullptr;
        emit this->finished();
        return;
    }

    const QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (!redirectionTarget.isNull())
    {
        // Redirection
        RLogger::info("HTTP request was redirected to: \'%s\'\n",redirectionTarget.toUrl().toString().toUtf8().constData());

        QUrl url(this->pRequestInput->getUrlStr());
        this->pRequestInput->setUrlStr(url.resolved(redirectionTarget.toUrl()).toString());

        this->execute(this->pRequestInput);

        reply->deleteLater();
        reply = nullptr;
        return;
    }

    // Success
    this->response = reply->readAll();
    reply->deleteLater();
    reply = nullptr;
    delete this->pRequestInput;
    this->pRequestInput = nullptr;
    emit this->finished();
}

void HttpRequestWorker::onManagerNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible)
{
    RLogger::info("Network eccessibility has changed to .");
    switch (accessible)
    {
        case QNetworkAccessManager::UnknownAccessibility:
        {
            RLogger::warning("The network accessibility cannot be determined.\n");
            break;
        }
        case QNetworkAccessManager::NotAccessible:
        {
            RLogger::warning("The network is not currently accessible.\n");
            break;
        }
        case QNetworkAccessManager::Accessible:
        {
            RLogger::info("The network is accessible.\n");
            break;
        }
    }
}

void HttpRequestWorker::onManagerPreSharedKeyAuthenticationRequired(QNetworkReply *, QSslPreSharedKeyAuthenticator *)
{
#ifdef DEBUG
    RLogger::info("SSL/TLS handshake negotiates a PSK ciphersuite, and therefore a PSK authentication is then required.\n");
#endif
}

void HttpRequestWorker::onManagerProxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)
{
    RLogger::warning("Proxy authentication is required.\n");
}

#ifndef QT_NO_SSL
void HttpRequestWorker::onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    if (!reply)
    {
        RLogger::error("HTTP request without a reply.\n");
        this->errorType = QNetworkReply::UnknownServerError;
    }
    else
    {
        QString errorString;
        foreach (const QSslError &error, errors) {
            RLogger::warning("SSL error has occurred: %s\n",error.errorString().toUtf8().constData());
        }

        reply->ignoreSslErrors();
    }
}
#endif

