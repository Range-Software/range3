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
    //! Mouse buttons flag.
    Qt::MouseButtons buttons;
    //! Wheel scroll phase.
    Qt::ScrollPhase scrollPhase;
    //! OpenGL action event type.
    GLActionEventType eventType;
} GLActionCombination;

static std::vector<GLActionCombination> glActionCombinations =
{
    { Qt::NoModifier,                          0, Qt::NoButton,     Qt::NoScrollPhase, GL_ACTION_EVENT_NONE              },
    { Qt::NoModifier,                          0, Qt::LeftButton,   Qt::NoScrollPhase, GL_ACTION_EVENT_TRANSLATE         },
    { Qt::ControlModifier,                     0, Qt::MiddleButton, Qt::NoScrollPhase, GL_ACTION_EVENT_TRANSLATE_Z       },
    { Qt::ControlModifier,                     0, Qt::NoButton,     Qt::ScrollUpdate,  GL_ACTION_EVENT_TRANSLATE_Z       },
    { Qt::NoModifier,                          0, Qt::MiddleButton, Qt::NoScrollPhase, GL_ACTION_EVENT_ROTATE            },
    { Qt::NoModifier,                          0, Qt::RightButton,  Qt::NoScrollPhase, GL_ACTION_EVENT_ZOOM              },
    { Qt::NoModifier,                          0, Qt::NoButton,     Qt::ScrollUpdate,  GL_ACTION_EVENT_ZOOM              },
    { Qt::ControlModifier,                     0, Qt::LeftButton,   Qt::NoScrollPhase, GL_ACTION_EVENT_PICK_ELEMENT      },
    { Qt::ControlModifier | Qt::AltModifier,   0, Qt::LeftButton,   Qt::NoScrollPhase, GL_ACTION_EVENT_PICK_NODE         },
    { Qt::ControlModifier | Qt::ShiftModifier, 0, Qt::LeftButton,   Qt::NoScrollPhase, GL_ACTION_EVENT_PICK_HOLE_ELEMENT },
    { Qt::ControlModifier,                     0, Qt::RightButton,  Qt::NoScrollPhase, GL_ACTION_EVENT_PICK_CLEAR        }
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
    for (auto &glActionCombination : glActionCombinations)
    {
        if (this->buttons      == glActionCombination.buttons &&
            this->keyModifiers == glActionCombination.keyModifiers &&
            this->scrollPhase  == glActionCombination.scrollPhase)
        {
            return glActionCombination.eventType;
        }
    }

    return GL_ACTION_EVENT_NONE;
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

void GLActionEvent::setScrollPhase(Qt::ScrollPhase srcollPhase)
{
    GLActionEventType prevType = this->getType();
    this->scrollPhase = srcollPhase;
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

QString GLActionEvent::findKeyMouseCombination(GLActionEventType type)
{
    for (auto &glActionCombination : glActionCombinations)
    {
        if (glActionCombination.eventType == type)
        {
            QString keyString(QKeySequence(glActionCombination.keyModifiers + glActionCombination.key).toString());

            QString mouseButtonString;

            if (glActionCombination.buttons & Qt::LeftButton)
            {
                mouseButtonString = (mouseButtonString.length() > 0 ? " + " : "") + tr("Left mouse button");
            }
            if (glActionCombination.buttons & Qt::MiddleButton)
            {
                mouseButtonString = (mouseButtonString.length() > 0 ? " + " : "") + tr("Middle mouse button");
            }
            if (glActionCombination.buttons & Qt::RightButton)
            {
                mouseButtonString = (mouseButtonString.length() > 0 ? " + " : "") + tr("Right mouse button");
            }

            return keyString + mouseButtonString;
        }
    }
    return QString();
}
