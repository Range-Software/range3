/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_acion_event.cpp                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   21-st February 2012                                      *
 *                                                                   *
 *  DESCRIPTION: OpenGL acion event class definition                 *
 *********************************************************************/

#include "gl_acion_event.h"

typedef struct _GLActionCombination
{
    //! Key modifier.
    Qt::KeyboardModifiers keyModifiers;
    //! Key.
    int key;
    //! Mause buttons flag.
    Qt::MouseButtons buttons;
} GLActionCombination;

static GLActionCombination glActionCombinations[GL_ACTION_N_EVENTS]
{
    {Qt::NoModifier,0,Qt::NoButton}, // GL_ACTION_EVENT_NONE
    {Qt::NoModifier,0,Qt::LeftButton}, // GL_ACTION_EVENT_TRANSLATE
    {Qt::NoModifier,0,Qt::MiddleButton}, // GL_ACTION_EVENT_ROTATE
    {Qt::NoModifier,0,Qt::RightButton}, // GL_ACTION_EVENT_ZOOM
    {Qt::ControlModifier,0,Qt::LeftButton}, // GL_ACTION_EVENT_PICK_ELEMENT
    {Qt::ControlModifier | Qt::AltModifier,0,Qt::LeftButton}, // GL_ACTION_EVENT_PICK_NODE
    {Qt::ControlModifier | Qt::ShiftModifier,0,Qt::LeftButton}, // GL_ACTION_EVENT_PICK_HOLE_ELEMENT
    {Qt::ControlModifier,0,Qt::RightButton}  // GL_ACTION_EVENT_PICK_CLEAR
};

GLActionEvent::GLActionEvent(QObject *parent)
    : QObject(parent)
    , keyModifiers(Qt::NoModifier)
    , key(0)
    , buttons(Qt::NoButton)
    , actionChanged(false)
{
}

GLActionEventType GLActionEvent::getType(void) const
{
    GLActionEventType type = GL_ACTION_EVENT_NONE;

    if (this->buttons & Qt::LeftButton)
    {
        type = GL_ACTION_EVENT_TRANSLATE;
        if (this->keyModifiers & Qt::ControlModifier)
        {
            type = GL_ACTION_EVENT_PICK_ELEMENT;
            if (this->keyModifiers & Qt::AltModifier)
            {
                type = GL_ACTION_EVENT_PICK_NODE;
            }
            if (this->keyModifiers & Qt::ShiftModifier)
            {
                type = GL_ACTION_EVENT_PICK_HOLE_ELEMENT;
            }
        }
        else
        {
            if (this->keyModifiers & Qt::AltModifier)
            {
                type = GL_ACTION_EVENT_ROTATE;
            }
        }
    }
    if (this->buttons & Qt::MiddleButton)
    {
        type = GL_ACTION_EVENT_ROTATE;
    }
    if (this->buttons & Qt::RightButton)
    {
        type = GL_ACTION_EVENT_ZOOM;
        if (this->keyModifiers & Qt::ControlModifier)
        {
            type = GL_ACTION_EVENT_PICK_CLEAR;
        }
    }

    return type;
}

bool GLActionEvent::getChanged(void) const
{
    return actionChanged;
}

void GLActionEvent::setKeyEvent(QKeyEvent *keyEvent)
{
    GLActionEventType prevType = this->getType();
    this->key = keyEvent->key();
    this->keyModifiers = keyEvent->modifiers();
    GLActionEventType currType = this->getType();
    this->actionChanged = (prevType != currType);
    emit this->changed(currType);
}

void GLActionEvent::setMouseEvent(QMouseEvent *mouseEvent)
{
    GLActionEventType prevType = this->getType();
    this->buttons = mouseEvent->buttons();
    GLActionEventType currType = this->getType();
    this->actionChanged = (prevType != currType);
    emit this->changed(currType);
}

void GLActionEvent::clear(void)
{
    GLActionEventType prevType = this->getType();
    this->buttons = Qt::NoButton;
    this->keyModifiers = Qt::NoModifier;
    this->key = 0;
    GLActionEventType currType = this->getType();
    this->actionChanged = (prevType != currType);
    emit this->changed(currType);
}

#include <rblib.h>
QString GLActionEvent::findKeyMouseCombination(GLActionEventType type)
{
    QString keyString(QKeySequence(glActionCombinations[type].keyModifiers + glActionCombinations[type].key).toString());

    QString mouseButtonString;

    if (glActionCombinations[type].buttons & Qt::LeftButton)
    {
        mouseButtonString = (mouseButtonString.length() > 0 ? " + " : "") + tr("Left mouse button");
    }
    if (glActionCombinations[type].buttons & Qt::MiddleButton)
    {
        mouseButtonString = (mouseButtonString.length() > 0 ? " + " : "") + tr("Middle mouse button");
    }
    if (glActionCombinations[type].buttons & Qt::RightButton)
    {
        mouseButtonString = (mouseButtonString.length() > 0 ? " + " : "") + tr("Right mouse button");
    }

    return keyString + mouseButtonString;
}
