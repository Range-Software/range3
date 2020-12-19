/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_acion_event.cpp                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   21-st February 2012                                      *
 *                                                                   *
 *  DESCRIPTION: OpenGL acion event class definition                 *
 *********************************************************************/

#include <QGuiApplication>

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
    // Translate
    { Qt::NoModifier,                          0, Qt::LeftButton,   Qt::NoScrollPhase, GL_ACTION_EVENT_TRANSLATE         },
    // Translate Z
    { Qt::ControlModifier,                     0, Qt::NoButton,     Qt::ScrollUpdate,  GL_ACTION_EVENT_TRANSLATE_Z       },
    // Zoom
    { Qt::NoModifier,                          0, Qt::NoButton,     Qt::ScrollUpdate,  GL_ACTION_EVENT_ZOOM              },
    { Qt::ShiftModifier | Qt::AltModifier,     0, Qt::LeftButton,   Qt::NoScrollPhase, GL_ACTION_EVENT_ZOOM              },
    // Rotate
    { Qt::ShiftModifier,                       0, Qt::LeftButton,   Qt::NoScrollPhase, GL_ACTION_EVENT_ROTATE            },
    { Qt::NoModifier,                          0, Qt::MiddleButton, Qt::NoScrollPhase, GL_ACTION_EVENT_ROTATE            },
    // Pick element
    { Qt::ControlModifier,                     0, Qt::LeftButton,   Qt::NoScrollPhase, GL_ACTION_EVENT_PICK_ELEMENT      },
    // Pick node
    { Qt::ControlModifier | Qt::AltModifier,   0, Qt::LeftButton,   Qt::NoScrollPhase, GL_ACTION_EVENT_PICK_NODE         },
    // Pick hole element
    { Qt::ControlModifier | Qt::ShiftModifier, 0, Qt::LeftButton,   Qt::NoScrollPhase, GL_ACTION_EVENT_PICK_HOLE_ELEMENT },
    // Pick clear

    { Qt::ControlModifier | Qt::MetaModifier,  0, Qt::LeftButton,   Qt::NoScrollPhase, GL_ACTION_EVENT_PICK_CLEAR        },
    { Qt::ControlModifier,                     0, Qt::RightButton,  Qt::NoScrollPhase, GL_ACTION_EVENT_PICK_CLEAR        }
};

GLActionEvent::GLActionEvent(QObject *parent)
    : QObject(parent)
    , keyModifiers(Qt::NoModifier)
    , key(0)
    , buttons(Qt::NoButton)
    , scrollPhase(Qt::NoScrollPhase)
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

void GLActionEvent::setKeyEvent(QKeyEvent *keyEvent, bool release)
{
    GLActionEventType prevType = this->getType();
    this->key = release ? 0 : keyEvent->key();
    this->keyModifiers = QGuiApplication::queryKeyboardModifiers();
    GLActionEventType currType = this->getType();
    this->actionChanged = (prevType != currType);
    emit this->changed(currType);
}

void GLActionEvent::setMouseEvent(QMouseEvent *mouseEvent, bool release)
{
    GLActionEventType prevType = this->getType();
    this->buttons = release ? Qt::NoButton : mouseEvent->buttons();
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
    this->scrollPhase = Qt::NoScrollPhase;
    GLActionEventType currType = this->getType();
    this->actionChanged = (prevType != currType);
    emit this->changed(currType);
}

QString GLActionEvent::getKeyMouseCombination(void) const
{
    return GLActionEvent::findKeyMouseCombination(this->keyModifiers,this->key,this->buttons,this->scrollPhase);
}

QString GLActionEvent::findKeyMouseCombination(Qt::KeyboardModifiers keyModifiers, int key, Qt::MouseButtons buttons, Qt::ScrollPhase scrollPhase)
{
    int _key = key;
    if (_key == Qt::Key_Alt     ||
        _key == Qt::Key_AltGr   ||
        _key == Qt::Key_Control ||
        _key == Qt::Key_Shift   ||
        _key == Qt::Key_Meta)
    {
        _key = 0;
    }
    QString keyString(QKeySequence(int(keyModifiers) + _key).toString(QKeySequence::NativeText));
    QString mouseButtonString;

    if (buttons & Qt::LeftButton)
    {
        mouseButtonString += (mouseButtonString.length() > 0 ? "+" : "") + tr("Left mouse button");
    }
    if (buttons & Qt::MiddleButton)
    {
        mouseButtonString += (mouseButtonString.length() > 0 ? "+" : "") + tr("Middle mouse button");
    }
    if (buttons & Qt::RightButton)
    {
        mouseButtonString += (mouseButtonString.length() > 0 ? "+" : "") + tr("Right mouse button");
    }
    if (scrollPhase == Qt::ScrollUpdate)
    {
        mouseButtonString += (mouseButtonString.length() > 0 ? "+" : "") + tr("Mouse scroll");
    }

    return keyString + mouseButtonString;
}

QVector<QString> GLActionEvent::findKeyMouseCombinations(GLActionEventType type)
{
    QVector<QString> combinations;
    for (auto &glActionCombination : glActionCombinations)
    {
        if (glActionCombination.eventType == type)
        {
            combinations.append(GLActionEvent::findKeyMouseCombination(glActionCombination.keyModifiers,glActionCombination.key,glActionCombination.buttons,glActionCombination.scrollPhase));
        }
    }
    return combinations;
}

QString GLActionEvent::toString(GLActionEventType type)
{
    switch (type)
    {
        case GL_ACTION_EVENT_NONE:
            return QString();
        case GL_ACTION_EVENT_TRANSLATE:
            return tr("Translate");
        case GL_ACTION_EVENT_TRANSLATE_Z:
            return tr("Translate in Z direction");
        case GL_ACTION_EVENT_ROTATE:
            return tr("Rotate");
        case GL_ACTION_EVENT_ZOOM:
            return tr("Zoom");
        case GL_ACTION_EVENT_PICK_ELEMENT:
            return tr("Pick element");
        case GL_ACTION_EVENT_PICK_NODE:
            return tr("Pick node");
        case GL_ACTION_EVENT_PICK_HOLE_ELEMENT:
            return tr("Pick hole element");
        case GL_ACTION_EVENT_PICK_CLEAR:
            return tr("Clear pick information");
        default:
            return QString();
    }
}
