/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   shortcut_line_edit.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   11-th September 2014                                     *
 *                                                                   *
 *  DESCRIPTION: Shortcut line edit class declaration                *
 *********************************************************************/

#ifndef __SHORTCUT_LINE_EDIT_H__
#define __SHORTCUT_LINE_EDIT_H__

#include <QLineEdit>

class ShortcutLineEdit : public QLineEdit
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit ShortcutLineEdit(QWidget *parent = 0);

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

#endif // __SHORTCUT_LINE_EDIT_H__
