/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   action_list.h                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   16-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Action list class declaration                       *
 *********************************************************************/

#ifndef ACTION_LIST_H
#define ACTION_LIST_H

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
        explicit ActionList(QObject *parent = nullptr);

        //! Return number of actions.
        uint getNActions() const;

        //! Return action for given action type.
        Action * getAction (ActionType type);

        //! Return action for given action name.
        Action * getAction (const QString &name);

        //! Process action availability based on selections.
        void processAvailability(void);

        //! Enable actions.
        void enable(void);

        //! Disable actions.
        void disable(void);

    private:

        //! Enable disable actions.
        void setDisabled(bool allActions);

    signals:

    public slots:

        //! Change action shortcut for given action type.
        void changeShortcut(ActionType actionType, const QString &shortcut);

};

#endif /* ACTION_LIST_H */
