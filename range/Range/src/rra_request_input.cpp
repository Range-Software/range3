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

RRARequestInput *RRARequestInput::requestClientLicense(void)
{
    RRARequestInput *pInput = new RRARequestInput;

    pInput->type = CLIENT_LICENSE;
    pInput->addVariable("pageMode","false");
    pInput->addVariable("data","client_license");

    return pInput;
}

RRARequestInput *RRARequestInput::sendUsageInfo(void)
{
    RRARequestInput *pInput = new RRARequestInput;

    pInput->type = SEND_USAGE_INFO;
    pInput->addVariable("pageMode","false");
    pInput->addVariable("action","usage_info");

    return pInput;
}

RRARequestInput::Type RRARequestInput::getType(void) const
{
    return this->type;
}
