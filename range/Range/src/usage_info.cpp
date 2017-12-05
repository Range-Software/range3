/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   usage_info.cpp                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   5-th December 2017                                       *
 *                                                                   *
 *  DESCRIPTION: Usage info class definition                         *
 *********************************************************************/

#include "main_settings.h"
#include "session.h"
#include "usage_info.h"

UsageInfo::UsageInfo()
{

}

UsageInfo &UsageInfo::getInstance(void)
{
    static UsageInfo usageInfo;
    return usageInfo;
}

QString UsageInfo::getReport(void) const
{
    QString report;

    report += "{";
    report += "\"account:\" \"" + MainSettings::getInstance().getApplicationSettings()->getRangeAccount() + "\",";
    report += "\"session:\" \"" + Session::getInstance().getID() + "\"";
    report += "}";

    return report;
}
