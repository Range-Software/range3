/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   action_definition.h                                      *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   9-th September 2014                                      *
 *                                                                   *
 *  DESCRIPTION: Action definition class declaration                 *
 *********************************************************************/

#ifndef ACTION_DEFINITION_H
#define ACTION_DEFINITION_H

#include <QString>
#include <QList>
#include <QObject>

#include "action_definition_item.h"


class ActionDefinition : public QObject
{

    Q_OBJECT

    protected:

        //! List of action descriptions.
        QList<ActionDefinitionItem> actionDesc;

    public:

        //! Constructor.
        explicit ActionDefinition(QObject *parent = nullptr);

        //! Find short-cut.
        //! If no shortcut was found ACTION_NONE is returned.
        ActionType findShortcutActionType(const QString &shortCut) const;

        //! Return action text.
        const QString &getText(ActionType type) const;

        //! Return action description.
        const QString &getDescription(ActionType type) const;

        //! Return action text.
        const QString &getShortCut(ActionType type) const;

        //! Set new shortcut.
        void setShortcut(ActionType type, const QString &shortCut);

        //! Return action text.
        const QString &getIcon(ActionType type) const;

        //! Return action text.
        PointerToMemberFunction getSlot(ActionType type) const;

        //! Return list of action types for given group type.
        QList<ActionType> getActionTypes(ActionGroupTypeMask actionGroupTypeMask = ACTION_GROUP_ALL) const;

        //! Generate list action descriptions.
        static void generateActionDescList(QList<ActionDefinitionItem> &actionDesc);

    signals:

        //! Shortcut changed signal.
        void shortcutChanged(ActionType actionType, const QString &shortcut);

};

#endif // ACTION_DEFINITION_H
