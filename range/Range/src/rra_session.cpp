/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rra_session.cpp                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   4-th July 2016                                           *
 *                                                                   *
 *  DESCRIPTION: Remote Range API session class definition           *
 *********************************************************************/

#include <QTimer>

#include <rblib.h>

#include "rra_session.h"
#include "main_settings.h"
#include "usage_info.h"

const int RRASession::relaxInterval = 1000;
const int RRASession::timeoutInterval = 10000;

RRASession::RRASession(QObject *parent)
    : QObject(parent)
    , isRunning(false)
    , availableSoftwareVersion(RVendor::version)
    , loggedIn(false)
{
    this->networkAccessManager = new QNetworkAccessManager(this);

    this->rraRequestWorker = new RRARequestWorker(this->networkAccessManager,this);

    this->connect(this->rraRequestWorker,SIGNAL(availableSoftware(RVersion,QString,QString)),SLOT(onRraAvailableSoftware(RVersion,QString,QString)));
    QObject::connect(this->rraRequestWorker,&RRARequestWorker::loginStatus,this,&RRASession::onRraLoginStatus);
    QObject::connect(this->rraRequestWorker,&RRARequestWorker::failed,this,&RRASession::onRraFailed);
}

RRARequestInput::Type RRASession::getNextType(RRARequestInput::Type type)
{
    switch (type)
    {
        case RRARequestInput::AVAILABLE_SOFTWARE:
//            return RRARequestInput::LOGIN;
//        case RRARequestInput::LOGIN:
//            return RRARequestInput::LOGOUT;
//        case RRARequestInput::LOGOUT:
            return RRARequestInput::SEND_USAGE_INFO;
        case RRARequestInput::SEND_USAGE_INFO:
        default:
            return RRARequestInput::AVAILABLE_SOFTWARE;
    }
}

RRASession &RRASession::getInstance()
{
    static RRASession rraSession;
    return rraSession;
}

void RRASession::start(void)
{
    if (this->isRunning)
    {
        RLogger::warning("RRA Session is already running.\n");
        return;
    }
    QTimer::singleShot(RRASession::relaxInterval,Qt::VeryCoarseTimer,this,&RRASession::submitNextRequest);
    this->isRunning = true;
}

void RRASession::stop(void)
{
    if (!this->isRunning)
    {
        RLogger::warning("RRA Session is not running.\n");
        return;
    }
    this->isRunning = false;
}

const RVersion &RRASession::getAvailableSoftwareVersion(void) const
{
    return this->availableSoftwareVersion;
}

const QString &RRASession::getAvailableSoftwareLink(void) const
{
    return this->availableSoftwareLink;
}

void RRASession::submitCrashReport(const QString &crashReport)
{
    this->rraRequestWorker->sendCrashReport(crashReport);
}

void RRASession::submitNextRequest(void)
{
    if (!MainSettings::getInstancePtr()->getApplicationSettings()->getRangeApiAllowed())
    {
        if (this->isRunning)
        {
            QTimer::singleShot(RRASession::relaxInterval,Qt::VeryCoarseTimer,this,&RRASession::submitNextRequest);
        }
        return;
    }

    switch (this->nextRequest)
    {
        case RRARequestInput::AVAILABLE_SOFTWARE:
        {
            this->rraRequestWorker->availableSoftware();
            break;
        }
        case RRARequestInput::LOGIN:
        {
            this->rraRequestWorker->logIn(MainSettings::getInstancePtr()->getApplicationSettings()->getRangeAccount(),
                                          MainSettings::getInstancePtr()->getApplicationSettings()->getRangePassword());
            break;
        }
        case RRARequestInput::LOGOUT:
        {
            this->rraRequestWorker->logOut(MainSettings::getInstancePtr()->getApplicationSettings()->getRangeAccount());
            break;
        }
        case RRARequestInput::SEND_USAGE_INFO:
        {
            if (MainSettings::getInstancePtr()->getApplicationSettings()->getSendUsageInfo())
            {
                this->rraRequestWorker->sendUsageInfo(UsageInfo::getInstance().getReport());
            }
            break;
        }
        case RRARequestInput::SEND_CRASH_REPORT:
        {
            break;
        }
    }

    this->nextRequest = RRASession::getNextType(this->nextRequest);
}

void RRASession::onRraAvailableSoftware(const RVersion &version, const QString &link, const QString &responseMessage)
{
    if (!responseMessage.isEmpty())
    {
        RLogger::info("RRA response: %s\n",responseMessage.toUtf8().constData());
    }
    if (this->availableSoftwareVersion != version || this->availableSoftwareLink != link)
    {
        this->availableSoftwareVersion = version;
        this->availableSoftwareLink = link;
        RLogger::info("Latest available version: %s\n",this->availableSoftwareVersion.toString().toUtf8().constData());
        emit this->availableSoftware(this->availableSoftwareVersion,this->availableSoftwareLink);
    }
    QTimer::singleShot(RRASession::relaxInterval,Qt::VeryCoarseTimer,this,&RRASession::submitNextRequest);
}

void RRASession::onRraLoginStatus(const QString &status, bool loggedIn, const QString &responseMessage)
{
    if (!responseMessage.isEmpty())
    {
        RLogger::info("RRA response: %s\n",responseMessage.toUtf8().constData());
    }
    if (this->loggedIn != loggedIn)
    {
        this->loggedIn = loggedIn;
        RLogger::info("%s\n",status.toUtf8().constData());
        if (this->loggedIn)
        {
            emit this->signedIn();
        }
        else
        {
            emit this->signedOut();
        }
    }
    QTimer::singleShot(RRASession::relaxInterval,Qt::VeryCoarseTimer,this,&RRASession::submitNextRequest);
}

void RRASession::onRraFailed(RRARequestInput::Type type, const QString &errorMessage)
{
    QString requestStr;

    switch (type)
    {
        case RRARequestInput::AVAILABLE_SOFTWARE:
        {
            requestStr = "available software";
            break;
        }
        case RRARequestInput::LOGIN:
        {
            requestStr = "log in";
            break;
        }
        case RRARequestInput::LOGOUT:
        {
            requestStr = "log out";
            break;
        }
        case RRARequestInput::SEND_USAGE_INFO:
        {
            requestStr = "usage info";
            break;
        }
        case RRARequestInput::SEND_CRASH_REPORT:
        {
            requestStr = "crash report";
            break;
        }
    }

    RLogger::error("Failed to process RRA request (%s): %s\n",requestStr.toUtf8().constData(),errorMessage.toUtf8().constData());

    QTimer::singleShot(RRASession::relaxInterval,Qt::VeryCoarseTimer,this,&RRASession::submitNextRequest);
}
