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

#include "application_settings.h"

const QString ApplicationSettings::FusionDark = "Fusion-dark";
const QString ApplicationSettings::Fusion = "Fusion";

ApplicationSettings::ApplicationSettings(QObject *parent)
    : QObject(parent)
    , nHistoryRecords(3)
    , style(ApplicationSettings::getDefaultStyle())
    , rangeApiAllowed(false)
    , rangeApiServer("http://range-software.com")
{
    this->nThreads = ApplicationSettings::getMaxThreads();
    this->actionDefinition = new ActionDefinition(this);

    QObject::connect(this->actionDefinition,
                     &ActionDefinition::shortcutChanged,
                     this,
                     &ApplicationSettings::onShortcutChanged);
}

const ActionDefinition *ApplicationSettings::getActionDefinition(void) const
{
    return this->actionDefinition;
}

ActionDefinition *ApplicationSettings::getActionDefinition(void)
{
    return this->actionDefinition;
}

const QString &ApplicationSettings::getSolverPath(void) const
{
    return this->solverPath;
}

void ApplicationSettings::setSolverPath(const QString &rangeSolverPath)
{
    this->solverPath = rangeSolverPath;
}

uint ApplicationSettings::getNThreads(void) const
{
    return this->nThreads;
}

void ApplicationSettings::setNThreads(uint nCPUs)
{
    uint defaultNCPUs = ApplicationSettings::getMaxThreads();
    this->nThreads = (nCPUs < 1 || nCPUs > defaultNCPUs) ? defaultNCPUs : nCPUs;
}

uint ApplicationSettings::getNHistoryRecords(void) const
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

const QString &ApplicationSettings::getStyle(void) const
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

bool ApplicationSettings::getRangeApiAllowed(void) const
{
    return this->rangeApiAllowed;
}

void ApplicationSettings::setRangeApiAllowed(bool rangeApiAllowed)
{
    this->rangeApiAllowed = rangeApiAllowed;
}

const QString &ApplicationSettings::getRangeApiServer(void) const
{
    return this->rangeApiServer;
}

void ApplicationSettings::setRangeApiServer(const QString &rangeApiServer)
{
    this->rangeApiServer = rangeApiServer;
}

const QString &ApplicationSettings::getRangeAccount(void) const
{
    return this->rangeAccount;
}

void ApplicationSettings::setRangeAccount(const QString &rangeAccount)
{
    this->rangeAccount = rangeAccount;
}

const QString &ApplicationSettings::getRangePassword(void) const
{
    return this->rangePassword;
}

void ApplicationSettings::setRangePassword(const QString &rangePassword)
{
    this->rangePassword = rangePassword;
}

QString ApplicationSettings::getDefaultRangeSolverExecutable(void)
{
#ifdef DEBUG
    return QString("RangeSolver_debug");
#else
    return QString("RangeSolver");
#endif
}

uint ApplicationSettings::getMaxThreads(void)
{
    return (uint)omp_get_num_procs();
}

QStringList ApplicationSettings::getStyles(void)
{
    QStringList styles = QStyleFactory::keys();
    styles.append(ApplicationSettings::FusionDark);
    styles.sort();
    return styles;
}

QString ApplicationSettings::getDefaultStyle(void)
{
    QStringList styles = QStyleFactory::keys();
    if (styles.contains(ApplicationSettings::Fusion))
    {
        return ApplicationSettings::Fusion;
    }
    return ApplicationSettings::FusionDark;
}

QColor ApplicationSettings::getDefaultBackgroundColor(const QString &style)
{
    if (style == ApplicationSettings::Fusion)
    {
        return QColor("#224466");
//        return QColor::fromCmykF(0.39, 0.39, 0.0, 0.0).dark();
    }
    else if (style == ApplicationSettings::FusionDark)
    {
        return QColor("#353535");
    }
    else
    {
        return QColor("#224466");
//        return QColor::fromCmykF(0.39, 0.39, 0.0, 0.0).dark();
    }
}

void ApplicationSettings::onShortcutChanged(ActionType actionType, const QString &shortcut)
{
    emit this->shortcutChanged(actionType,shortcut);
}
