/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_acion_event.h                                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   21-st February 2012                                      *
 *                                                                   *
 *  DESCRIPTION: OpenGL acion event class declaration                *
 *********************************************************************/

#ifndef GL_ACTION_EVENT_H
#define GL_ACTION_EVENT_H

#include <QObject>
#include <QKeyEvent>
#include <QMouseEvent>

typedef enum _GLActionEventType {
    GL_ACTION_EVENT_NONE = 0,
    GL_ACTION_EVENT_TRANSLATE,
    GL_ACTION_EVENT_TRANSLATE_Z,
    GL_ACTION_EVENT_ROTATE,
    GL_ACTION_EVENT_ZOOM,
    GL_ACTION_EVENT_PICK_ELEMENT,
    GL_ACTION_EVENT_PICK_NODE,
    GL_ACTION_EVENT_PICK_HOLE_ELEMENT,
    GL_ACTION_EVENT_PICK_CLEAR,
    GL_ACTION_N_EVENTS
} GLActionEventType;

class GLActionEvent : public QObject
{
    Q_OBJECT

    protected:

        //! Key modifier.
        Qt::KeyboardModifiers keyModifiers;
        //! Key.
        int key;
        //! Mause buttons flag.
        Qt::MouseButtons buttons;
        //! Wheel is scrolling
        Qt::ScrollPhase scrollPhase;
        //! Even changed.
        bool actionChanged;

    public:

        //! Constructor.
        GLActionEvent(QObject *parent = nullptr);

        //! Return action event type.
        GLActionEventType getType(void) const;

        //! Check whether action event has changed.
        bool getChanged(void) const;

        //! Return key modifier.
        inline Qt::KeyboardModifiers getKeyModifiers(void) const
        {
            return this->keyModifiers;
        }

        //! Return key.
        inline int getKey(void) const
        {
            return this->key;
        }

        //! Return mouse buttons.
        inline Qt::MouseButtons getMouseButtons(void) const
        {
            return this->buttons;
        }

        //! Set Qt key event.
        void setKeyEvent(QKeyEvent *keyEvent, bool release);

        //! Set Qt mouse event.
        void setMouseEvent(QMouseEvent *mouseEvent, bool release);

        //! Set Qt wheel event.
        void setScrollPhase(Qt::ScrollPhase srcollPhase);

        //! Clear.
        void clear(void);

        //! Return current key mouse conbination
        QString getKeyMouseCombination(void) const;

        //! Return string representation of keay mouse combination.
        static QString findKeyMouseCombination(GLActionEventType type);

        //! Get string representation of action type.
        static QString toString(GLActionEventType type);

    signals:

        //! Emit signal that action has changed.
        void changed(GLActionEventType type);

};

#endif /* GL_ACTION_EVENT_H */
