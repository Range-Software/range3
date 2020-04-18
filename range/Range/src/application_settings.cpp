/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   application_settings.cpp                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   12-th July 2013                                          *
 *                                                                   *
 *  DESCRIPTION: Application class class definition                  *
 *********************************************************************/

#include <omp.h>

#include <QStyleFactory>
#include <QApplication>
#include <QDir>

#include "application_settings.h"
#include "main_settings.h"

const QString ApplicationSettings::WindowsDark = "Windows-Dark";
const QString ApplicationSettings::FusionDark = "Fusion-Dark";
const QString ApplicationSettings::FusionRange = "Fusion-Range";
const QString ApplicationSettings::Fusion = "Fusion";

ApplicationSettings::ApplicationSettings(QObject *parent)
    : QObject(parent)
    , nThreads(ApplicationSettings::getDefaultNThreads())
    , nHistoryRecords(ApplicationSettings::getDefaultNHistoryRecords())
    , style(ApplicationSettings::getDefaultStyle())
    , toolbarIconSize(ApplicationSettings::getDefaultToolbarIconSize())
    , sendUsageInfo(ApplicationSettings::getDefaultSendUsageInfo())
    , rangeApiAllowed(ApplicationSettings::getDefaultRangeApiAllowed())
    , rangeApiServer(ApplicationSettings::getDefaultRangeApiServer())
    , rangeAccount(ApplicationSettings::getDefaultRangeAccount())
    , rangePassword(ApplicationSettings::getDefaultRangePassword())
{
    this->actionDefinition = new ActionDefinition(this);

    QObject::connect(this->actionDefinition,
                     &ActionDefinition::shortcutChanged,
                     this,
                     &ApplicationSettings::onShortcutChanged);
}

const ActionDefinition *ApplicationSettings::getActionDefinition() const
{
    return this->actionDefinition;
}

ActionDefinition *ApplicationSettings::getActionDefinition()
{
    return this->actionDefinition;
}

const QString &ApplicationSettings::getSolverPath() const
{
    return this->solverPath;
}

void ApplicationSettings::setSolverPath(const QString &rangeSolverPath)
{
    this->solverPath = rangeSolverPath;
}

const QString &ApplicationSettings::getHelpDir() const
{
    return this->helpDir;
}

void ApplicationSettings::setHelpDir(const QString &helpDir)
{
    this->helpDir = helpDir;
}

uint ApplicationSettings::getNThreads() const
{
    return this->nThreads;
}

void ApplicationSettings::setNThreads(uint nCPUs)
{
    uint defaultNCPUs = ApplicationSettings::getDefaultNThreads();
    this->nThreads = (nCPUs < 1 || nCPUs > defaultNCPUs) ? defaultNCPUs : nCPUs;
}

uint ApplicationSettings::getNHistoryRecords() const
{
    return this->nHistoryRecords;
}

void ApplicationSettings::setNHistoryRecords(uint nHistoryRecords)
{
    bool changed = false;
    nHistoryRecords = nHistoryRecords > 0 ? nHistoryRecords : 1;
    if (this->nHistoryRecords != nHistoryRecords)
    {
        changed = true;
    }
    this->nHistoryRecords = nHistoryRecords;
    if (changed)
    {
        emit this->nHistoryRecordsChanged(this->nHistoryRecords);
    }
}

const QString &ApplicationSettings::getStyle() const
{
    return this->style;
}

void ApplicationSettings::setStyle(const QString &style)
{
    QStringList styles = ApplicationSettings::getStyles();
    if (styles.contains(style))
    {
        this->style = style;
    }
    else
    {
        this->style = ApplicationSettings::getDefaultStyle();
    }
    emit this->styleChanged(this->style);
}

int ApplicationSettings::getToolbarIconSize() const
{
    return this->toolbarIconSize;
}

void ApplicationSettings::setToolbarIconSize(int toolbarIconSize)
{
    this->toolbarIconSize = toolbarIconSize;
    emit this->toolbarIconSizeChanged(this->toolbarIconSize);
}

bool ApplicationSettings::getSendUsageInfo() const
{
    return this->sendUsageInfo;
}

void ApplicationSettings::setSendUsageInfo(bool sendUsageInfo)
{
    this->sendUsageInfo = sendUsageInfo;
}

bool ApplicationSettings::getRangeApiAllowed() const
{
    return this->rangeApiAllowed;
}

void ApplicationSettings::setRangeApiAllowed(bool rangeApiAllowed)
{
    this->rangeApiAllowed = rangeApiAllowed;
}

const QString &ApplicationSettings::getRangeApiServer() const
{
    return this->rangeApiServer;
}

void ApplicationSettings::setRangeApiServer(const QString &rangeApiServer)
{
    this->rangeApiServer = rangeApiServer;
}

const QString &ApplicationSettings::getRangeAccount() const
{
    return this->rangeAccount;
}

void ApplicationSettings::setRangeAccount(const QString &rangeAccount)
{
    this->rangeAccount = rangeAccount;
}

const QString &ApplicationSettings::getRangePassword() const
{
    return this->rangePassword;
}

void ApplicationSettings::setRangePassword(const QString &rangePassword)
{
    this->rangePassword = rangePassword;
}

uint ApplicationSettings::getMaxThreads()
{
    return (uint)omp_get_num_procs();
}

QStringList ApplicationSettings::getStyles()
{
    QStringList styles = QStyleFactory::keys();
    styles.append(ApplicationSettings::FusionDark);
    styles.append(ApplicationSettings::FusionRange);
    styles.append(ApplicationSettings::WindowsDark);
    styles.sort();
    return styles;
}

QString ApplicationSettings::getDefaultRangeSolverExecutable()
{
#ifdef DEBUG
    QString baseName("RangeSolver_debug");
#else
    QString baseName("RangeSolver");
#endif

    return QDir(QApplication::applicationDirPath()).filePath(baseName);
}

QString ApplicationSettings::getDefaultHelpDir()
{
    return MainSettings::getInstance().findHelpDir();
}

uint ApplicationSettings::getDefaultNThreads()
{
    return ApplicationSettings::getMaxThreads();
}

uint ApplicationSettings::getDefaultNHistoryRecords()
{
    return 3;
}

QString ApplicationSettings::getDefaultStyle()
{
    return ApplicationSettings::Fusion;
}

int ApplicationSettings::getDefaultToolbarIconSize()
{
    return 32;
}

bool ApplicationSettings::getDefaultSendUsageInfo()
{
    return true;
}

bool ApplicationSettings::getDefaultRangeApiAllowed()
{
    return true;
}

const QString ApplicationSettings::getDefaultRangeApiServer()
{
    return RVendor::www;
}

const QString ApplicationSettings::getDefaultRangeAccount()
{
    return QString();
}

const QString ApplicationSettings::getDefaultRangePassword()
{
    return QString();
}

QColor ApplicationSettings::getDefaultBackgroundColor(const QString &style)
{
    if (style == ApplicationSettings::Fusion)
    {
        return QColor(34,68,102);
    }
    else if (style == ApplicationSettings::FusionRange)
    {
        return QColor(48,0,0);
    }
    else if (style == ApplicationSettings::FusionDark)
    {
        return QColor(53,53,53);
    }
    else if (style == ApplicationSettings::WindowsDark)
    {
        return QColor(53,53,53);
    }
    else
    {
        return QColor(34,68,102);
    }
}

void ApplicationSettings::onShortcutChanged(ActionType actionType, const QString &shortcut)
{
    emit this->shortcutChanged(actionType,shortcut);
}
