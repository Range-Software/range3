/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   keyboard_shortcuts_edit.h                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   10-th September 2014                                     *
 *                                                                   *
 *  DESCRIPTION: Keyboard shortcuts editor class declaration         *
 *********************************************************************/

#ifndef KEYBOARD_SHORTCUTS_EDIT_H
#define KEYBOARD_SHORTCUTS_EDIT_H

#include <QTreeWidget>
#include <QLineEdit>

#include "action_definition.h"
#include "shortcut_line_edit.h"

class KeyboardShortcutsEdit : public QWidget
{

    Q_OBJECT

    protected:

        //! Tree view.
        QTreeWidget *tree;
        //! Line edit.
        ShortcutLineEdit *lineEdit;
        //! Action definitions.
        const ActionDefinition *actionDefinition;

    public:

        //! Constructor.
        explicit KeyboardShortcutsEdit(const ActionDefinition *actionDefinition,QWidget *parent = nullptr);

        //! Reset to default.
        void resetToDefault(void);

    protected:

        //! Populate tree.
        void populateTree(void);

        //! Find items with given action type.
        QList<QTreeWidgetItem *> findItems(ActionType type);

        //! Find items with given shortcut.
        QList<QTreeWidgetItem *> findItems(const QString &shortcut);

    signals:

        //! Shortcut changed signal.
        void shortcutChanged(ActionType actionType, const QString &shortcut);

    protected slots:

        //! Tree selection has changed.
        void onSelectionChanged(void);

        //! On reset all button clicked.
        void onResetAllClicked(void);

        //! Shortcut changed in shortcut line edit.
        void onShortcutChanged(const QString &shortcut);

};

#endif // KEYBOARD_SHORTCUTS_EDIT_H
