/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rra_request_worker.cpp                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   14-th November 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Remote Range API request worker class definition    *
 *********************************************************************/

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "rra_request_worker.h"

RRARequestWorker::RRARequestWorker(QNetworkAccessManager *networkAccessManager, QObject *parent)
    : HttpRequestWorker(networkAccessManager,parent)
{
    QObject::connect(this,&HttpRequestWorker::finished,this,&RRARequestWorker::onHttpRequestFinished);
}

void RRARequestWorker::availableSoftware(void)
{
    RRARequestInput *pInput = RRARequestInput::requestLatestSoftwareVersion();
    this->requestType = pInput->getType();
    this->execute(pInput);
}

void RRARequestWorker::logIn(const QString &account, const QString &password)
{
    RRARequestInput *pInput = RRARequestInput::logIn(account,password);
    this->requestType = pInput->getType();
    this->execute(pInput);
}

void RRARequestWorker::logOut(const QString &account)
{
    RRARequestInput *pInput = RRARequestInput::logOut(account);
    this->requestType = pInput->getType();
    this->execute(pInput);
}

void RRARequestWorker::sendUsageInfo(const QString &usageInfo)
{
    RRARequestInput *pInput = RRARequestInput::sendUsageInfo(usageInfo);
    this->requestType = pInput->getType();
    this->execute(pInput);
}

void RRARequestWorker::sendCrashReport(const QString &crashReport)
{
    RRARequestInput *pInput = RRARequestInput::sendCrashReport(crashReport);
    this->requestType = pInput->getType();
    this->execute(pInput);
}

void RRARequestWorker::onHttpRequestFinished(void)
{
    if (this->getErrorType() != QNetworkReply::NoError)
    {
        // an error occurred
        emit this->failed(this->requestType,"RRA communication error: " + this->getError());
        return;
    }

    // communication was successful
    QString html, status;
    QStringList messages;
    QStringList warnings;
    QStringList errors;

    if (!RRARequestWorker::processJSon(this->getResponse(),html,status,messages,warnings,errors))
    {
        // an error occurred
        int responseSize = 255;
        QString response(this->getResponse());
        if (response.size() > responseSize)
        {
            response.truncate(responseSize);
            response += "...";
        }
        emit this->failed(this->requestType,"RRA unexpected results: " + response);
        return;
    }

    QString responseMessages;
    if (messages.size() > 0)
    {
        if (!responseMessages.isEmpty())
        {
            responseMessages += " ";
        }
        responseMessages += "Messages: " + messages.join(' ');
    }
    if (warnings.size() > 0)
    {
        if (!responseMessages.isEmpty())
        {
            responseMessages += " ";
        }
        responseMessages += "Warnings: " + warnings.join(' ');
    }
    if (errors.size() > 0)
    {
        if (!responseMessages.isEmpty())
        {
            responseMessages += " ";
        }
        responseMessages += "Errors: " + errors.join(' ');
    }

    if (status != "success")
    {
        // an error occurred
        emit this->failed(this->requestType,"RRA error status: " + status + ". " + responseMessages);
        return;
    }

    switch (this->requestType)
    {
        case RRARequestInput::AVAILABLE_SOFTWARE:
        {
            QStringList elements = html.split('|');
            if (elements.size() == 2)
            {
                emit this->availableSoftware(RVersion(elements.at(0)),elements.at(1),responseMessages);
            }
            else
            {
                emit this->failed(this->requestType,tr("Unexpected response."));
            }
            return;
        }
        case RRARequestInput::LOGIN:
        {
            emit this->loginStatus(QString(html),true,responseMessages);
            return;
        }
        case RRARequestInput::LOGOUT:
        {
            emit this->loginStatus(QString(html),false,responseMessages);
            return;
        }
        case RRARequestInput::SEND_USAGE_INFO:
        {
            emit this->usage(QString(html),responseMessages);
            return;
        }
        case RRARequestInput::SEND_CRASH_REPORT:
        {
            emit this->crashReport(QString(html),responseMessages);
            return;
        }
    }
}

bool RRARequestWorker::processJSon(const QByteArray &byteArray, QString &html, QString &status, QStringList &messages, QStringList &warnings, QStringList &errors)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(byteArray));
    QJsonObject json = jdoc.object();

    QStringList dataKeys;

    if (json.contains("data") && json["data"].isObject())
    {
        dataKeys.append("data");
    }
    if (json.contains("action") && json["action"].isObject())
    {
        dataKeys.append("action");
    }

    if (dataKeys.isEmpty())
    {
        return false;
    }

    if (!json.contains("messages") ||
        !json.contains("warnings") ||
        !json.contains("errors"))
    {
        return false;
    }
    if (!json["messages"].isArray() ||
        !json["warnings"].isArray() ||
        !json["errors"].isArray())
    {
        return false;
    }

    foreach (const QString &dataKey, dataKeys)
    {
        QJsonObject jsonData = json[dataKey].toObject();

        if (!jsonData.contains("html") || !jsonData.contains("status"))
        {
            return false;
        }

        if (!jsonData["status"].isString())
        {
            return false;
        }

        if (jsonData["html"].isString())
        {
            html = jsonData["html"].toString();
        }
        status = jsonData["status"].toString();
    }

    QJsonArray jsonMessages = json["messages"].toArray();
    QJsonArray jsonWarnings = json["warnings"].toArray();
    QJsonArray jsonErrors = json["errors"].toArray();

    for (int i=0;i<jsonMessages.size();i++)
    {
        if (!jsonMessages.at(i).isString())
        {
            return false;
        }
        messages.append(jsonMessages.at(i).toString());
    }

    for (int i=0;i<jsonWarnings.size();i++)
    {
        if (!jsonWarnings.at(i).isString())
        {
            return false;
        }
        warnings.append(jsonWarnings.at(i).toString());
    }

    for (int i=0;i<jsonErrors.size();i++)
    {
        if (!jsonErrors.at(i).isString())
        {
            return false;
        }
        errors.append(jsonErrors.at(i).toString());
    }

    return true;
}
