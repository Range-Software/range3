/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   shortcut_line_edit.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   11-th September 2014                                     *
 *                                                                   *
 *  DESCRIPTION: Shortcut line edit class declaration                *
 *********************************************************************/

#ifndef SHORTCUT_LINE_EDIT_H
#define SHORTCUT_LINE_EDIT_H

#include <QLineEdit>

class ShortcutLineEdit : public QLineEdit
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit ShortcutLineEdit(QWidget *parent = nullptr);

    protected:

        //! Process key press event.
        void keyPressEvent(QKeyEvent *keyEvent);

    signals:

        //! Shortcut changed signal.
        void shortcutChanged(const QString &shortcut);

    protected slots:

        //! Process signal QLineEdit::textChanged()
        void onTextChanged(const QString &text);

};

#endif // SHORTCUT_LINE_EDIT_H
