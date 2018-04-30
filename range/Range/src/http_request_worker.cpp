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

#include <rblib.h>

#include "http_request_worker.h"

HttpRequestWorker::HttpRequestWorker(QNetworkAccessManager *networkAccessManager, QObject *parent)
    : QObject(parent)
    , manager(networkAccessManager)
{
    qsrand(QDateTime::currentDateTime().toTime_t());

    connect(this->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onManagerFinished(QNetworkReply*)));
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
    // reset variables

    QByteArray requestContent = "";
    this->response = "";
    this->errorType = QNetworkReply::NoError;
    this->errorStr = "";

    QMap<QString, QString> &rVariables = pInput->getVariableMap();
    QList<HttpRequestInput::FileElement> &rFileList = pInput->getFileList();

    // decide on the variable layout

    if (rFileList.length() > 0)
    {
        pInput->setVariableLayout(HttpRequestInput::MULTIPART);
    }
    if (pInput->getVariableLayout() == HttpRequestInput::NOT_SET)
    {
        pInput->setVariableLayout(pInput->getHttpMethod() == "GET" || pInput->getHttpMethod() == "HEAD" ? HttpRequestInput::ADDRESS : HttpRequestInput::URL_ENCODED);
    }


    // prepare request content

    QString boundary = "";

    if (pInput->getVariableLayout() == HttpRequestInput::ADDRESS || pInput->getVariableLayout() == HttpRequestInput::URL_ENCODED)
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

            if (pInput->getVariableLayout() == HttpRequestInput::ADDRESS)
            {
                pInput->getUrlStr() += "?" + requestContent;
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
                fileIterator->localFilename == NULL || fileIterator->localFilename.isEmpty()
                || fileIterator->variableName == NULL || fileIterator->variableName.isEmpty()
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
            if (fileIterator->requestFilename == NULL || fileIterator->requestFilename.isEmpty())
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

            if (fileIterator->mimeType != NULL && !fileIterator->mimeType.isEmpty())
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

    QNetworkRequest request = QNetworkRequest(QUrl(pInput->getUrlStr()));
    request.setRawHeader("User-Agent", "Agent name goes here");

    if (pInput->getVariableLayout() == HttpRequestInput::URL_ENCODED)
    {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    }
    else if (pInput->getVariableLayout() == HttpRequestInput::MULTIPART)
    {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + boundary);
    }

    if (pInput->getHttpMethod() == "GET")
    {
        this->manager->get(request);
    }
    else if (pInput->getHttpMethod() == "POST")
    {
        this->manager->post(request, requestContent);
    }
    else if (pInput->getHttpMethod() == "PUT")
    {
        this->manager->put(request, requestContent);
    }
    else if (pInput->getHttpMethod() == "HEAD")
    {
        this->manager->head(request);
    }
    else if (pInput->getHttpMethod() == "DELETE")
    {
        this->manager->deleteResource(request);
    }
    else
    {
        QBuffer buff(&requestContent);
        this->manager->sendCustomRequest(request, pInput->getHttpMethod().toLatin1(), &buff);
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

void HttpRequestWorker::onManagerFinished(QNetworkReply *reply)
{
    if (!reply)
    {
        RLogger::error("HTTP request without a reply.\n");
        this->errorType = QNetworkReply::UnknownServerError;
    }
    else
    {
        this->errorType = reply->error();
        if (this->errorType == QNetworkReply::NoError)
        {
            this->response = reply->readAll();
        }
        else
        {
            this->errorStr = reply->errorString();
        }
        reply->deleteLater();
    }

    emit this->finished();
}

