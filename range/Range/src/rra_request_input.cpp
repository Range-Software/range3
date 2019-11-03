/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rra_request_input.cpp                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   14-th November 2016                                      *
 *                                                                   *
 *  DESCRIPTION: Remote Range API request input class definition     *
 *********************************************************************/

#include "rra_request_input.h"
#include "main_settings.h"

RRARequestInput::RRARequestInput() : HttpRequestInput(RRARequestInput::findUrl(),"POST")
{

}

QString RRARequestInput::findUrl(void)
{
    return MainSettings::getInstance().getApplicationSettings()->getRangeApiServer() + "/";
}

RRARequestInput *RRARequestInput::requestLatestSoftwareVersion(void)
{
    RRARequestInput *pInput = new RRARequestInput;

    pInput->type = AVAILABLE_SOFTWARE;
    pInput->addVariable("pageMode","false");
    pInput->addVariable("data","available_software");
    pInput->addVariable("direct","true");
    pInput->addVariable("product_type",QSysInfo::productType());

    return pInput;
}

RRARequestInput *RRARequestInput::logIn(const QString &account, const QString &password)
{
    RRARequestInput *pInput = new RRARequestInput;

    pInput->type = LOGIN;
    pInput->addVariable("pageMode","false");
    pInput->addVariable("action","login");
    pInput->addVariable("username",account);
    pInput->addVariable("password",password);

    return pInput;
}

RRARequestInput *RRARequestInput::logOut(const QString &account)
{
    RRARequestInput *pInput = new RRARequestInput;

    pInput->type = LOGOUT;
    pInput->addVariable("pageMode","false");
    pInput->addVariable("action","logout");
    pInput->addVariable("username",account);

    return pInput;
}

RRARequestInput *RRARequestInput::sendUsageInfo(const QString &usageInfo)
{
    RRARequestInput *pInput = new RRARequestInput;

    pInput->type = SEND_USAGE_INFO;
    pInput->addVariable("pageMode","false");
    pInput->addVariable("action","usage_info");
//    pInput->addVariable("usage_info",usageInfo.toHtmlEscaped());
    pInput->addVariable("usage_info",usageInfo);

    return pInput;
}

RRARequestInput *RRARequestInput::sendCrashReport(const QString &crashReport)
{
    RRARequestInput *pInput = new RRARequestInput;

    pInput->type = SEND_CRASH_REPORT;
    pInput->addVariable("pageMode","false");
    pInput->addVariable("action","crash_report");
    pInput->addVariable("crash_report",crashReport);

    return pInput;
}

RRARequestInput::Type RRARequestInput::getType(void) const
{
    return this->type;
}
