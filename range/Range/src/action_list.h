/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   action_list.h                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   16-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Action list class declaration                       *
 *********************************************************************/

#ifndef __ACTION_LIST_H__
#define __ACTION_LIST_H__

#include <QObject>

#include "action.h"

class ActionList : public QObject
{
    Q_OBJECT

    protected:

        //! List of actions.
        QVector<Action*> actions;

    public:

        //! Constructor.
        explicit ActionList(QObject *parent = 0);

        //! Return number of actions.
        uint getNActions() const;

        //! Return action for given action type.
        Action * getAction (ActionType type);

        //! Return action for given action name.
        Action * getAction (const QString &name);

        //! Enable disable actions.
        void setEnabled(bool enabled, bool allActions);

        //! Process action availability based on selections.
        void processAvailability(void);

    signals:

    public slots:

        //! Change action shortcut for given action type.
        void changeShortcut(ActionType actionType, const QString &shortcut);

};

#endif /* __ACTION_LIST_H__ */
