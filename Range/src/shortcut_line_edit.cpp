/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   shortcut_line_edit.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   11-th September 2014                                     *
 *                                                                   *
 *  DESCRIPTION: Shortcut line edit class declaration                *
 *********************************************************************/

#include <QKeyEvent>

#include "shortcut_line_edit.h"

ShortcutLineEdit::ShortcutLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    this->setClearButtonEnabled(true);

    QObject::connect(this,
                     &QLineEdit::textChanged,
                     this,
                     &ShortcutLineEdit::onTextChanged);
}

void ShortcutLineEdit::keyPressEvent(QKeyEvent *keyEvent)
{
    QString grab;
    bool emitSignal = false;

    int modifiers = keyEvent->modifiers();

    if (modifiers & Qt::ControlModifier)
    {
        grab.append("Ctrl+");
    }
    if (modifiers & Qt::ShiftModifier)
    {
        grab.append("Shift+");
    }
    if (modifiers & Qt::AltModifier)
    {
        grab.append("Alt+");
    }
    if (QChar(keyEvent->key()).isLetter())
    {
        grab.append(QChar(keyEvent->key()).toUpper());
        emitSignal = true;
    }
    this->setText(grab);

    if (emitSignal)
    {
        emit this->shortcutChanged(this->text());
    }
}

void ShortcutLineEdit::onTextChanged(const QString &text)
{
    if (text.isEmpty())
    {
        emit this->shortcutChanged(this->text());
    }
}
