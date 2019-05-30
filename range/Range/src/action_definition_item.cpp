/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   action_definition_item.cpp                               *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   10-th September 2014                                     *
 *                                                                   *
 *  DESCRIPTION: Action definition item class definition             *
 *********************************************************************/

#include <QObject>

#include "action_definition_item.h"


void ActionDefinitionItem::_init(const ActionDefinitionItem *pActionDefinitionItem)
{
    if (pActionDefinitionItem)
    {
        this->type = pActionDefinitionItem->type;
        this->groupType = pActionDefinitionItem->groupType;
        this->text = pActionDefinitionItem->text;
        this->desc = pActionDefinitionItem->desc;
        this->shortCut = pActionDefinitionItem->shortCut;
        this->icon = pActionDefinitionItem->icon;
        this->slot = pActionDefinitionItem->slot;
    }
}

ActionDefinitionItem::ActionDefinitionItem(ActionType type, ActionGroupType groupType, const QString &text, const QString &desc, const QString &shortCut, const QString &icon, PointerToMemberFunction slot)
    : type(type)
    , groupType(groupType)
    , text(text)
    , desc(desc)
    , shortCut(shortCut)
    , icon(icon)
    , slot(slot)
{
    this->_init();
}

ActionDefinitionItem::ActionDefinitionItem(const ActionDefinitionItem &actionDefinitionItem)
{
    this->_init(&actionDefinitionItem);
}

ActionDefinitionItem::~ActionDefinitionItem()
{

}

ActionDefinitionItem &ActionDefinitionItem::operator =(const ActionDefinitionItem &actionDefinitionItem)
{
    this->_init(&actionDefinitionItem);
    return (*this);
}

ActionType ActionDefinitionItem::getType() const
{
    return this->type;
}

ActionGroupType ActionDefinitionItem::getGroupType() const
{
    return this->groupType;
}

const QString &ActionDefinitionItem::getShortcut() const
{
    return this->shortCut;
}

QString ActionDefinitionItem::getGroupName(ActionGroupType groupType)
{
    switch (groupType)
    {
        case ACTION_GROUP_SESSION:
            return QObject::tr("Session");
        case ACTION_GROUP_MODEL:
            return QObject::tr("Model");
        case ACTION_GROUP_MATERIAL:
            return QObject::tr("Material");
        case ACTION_GROUP_GEOMETRY:
            return QObject::tr("Geometry");
        case ACTION_GROUP_PROBLEM:
            return QObject::tr("Problem");
        case ACTION_GROUP_SOLVER:
            return QObject::tr("Solver");
        case ACTION_GROUP_REPORT:
            return QObject::tr("Report");
        case ACTION_GROUP_APPLICATION:
            return QObject::tr("Application");
        default:
            return QString();
    }
}

QList<ActionGroupType> ActionDefinitionItem::getGroupTypes(ActionGroupTypeMask groupTypeMask)
{
    QList<ActionGroupType> groupTypes;

    if (groupTypeMask & ACTION_GROUP_SESSION)     groupTypes.append(ACTION_GROUP_SESSION);
    if (groupTypeMask & ACTION_GROUP_MODEL)       groupTypes.append(ACTION_GROUP_MODEL);
    if (groupTypeMask & ACTION_GROUP_MATERIAL)    groupTypes.append(ACTION_GROUP_MATERIAL);
    if (groupTypeMask & ACTION_GROUP_GEOMETRY)    groupTypes.append(ACTION_GROUP_GEOMETRY);
    if (groupTypeMask & ACTION_GROUP_PROBLEM)     groupTypes.append(ACTION_GROUP_PROBLEM);
    if (groupTypeMask & ACTION_GROUP_SOLVER)      groupTypes.append(ACTION_GROUP_SOLVER);
    if (groupTypeMask & ACTION_GROUP_REPORT)      groupTypes.append(ACTION_GROUP_REPORT);
    if (groupTypeMask & ACTION_GROUP_APPLICATION) groupTypes.append(ACTION_GROUP_APPLICATION);

    return groupTypes;
}
