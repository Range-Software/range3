#include <QMouseEvent>
#include <GL/glu.h>

#include "glwidget.h"

#define SELECTION_BUFFER_SIZE 1024

static bool insideBeginEnd = false;

void OpenGLFunctions::printError(const char *command, const char *file, unsigned int line)
{
    if (insideBeginEnd)
    {
        return;
    }

    GLenum glErrorNum = glGetError();

    QString errorMessage;

    switch (glErrorNum)
    {
        case GL_NO_ERROR:
        {
//            No error has been recorded.
            break;
        }
        case GL_INVALID_ENUM:
        {
            errorMessage += "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";
            break;
        }
        case GL_INVALID_VALUE:
        {
            errorMessage += "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
            break;
        }
        case GL_INVALID_OPERATION:
        {
            errorMessage += "The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
            break;
        }
        case GL_INVALID_FRAMEBUFFER_OPERATION:
        {
            errorMessage += "The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.";
            break;
        }
        case GL_OUT_OF_MEMORY:
        {
            errorMessage += "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
            break;
        }
        case GL_STACK_UNDERFLOW:
        {
            errorMessage += "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
            break;
        }
        case GL_STACK_OVERFLOW:
        {
            errorMessage += "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
            break;
        }
        default:
        {
            errorMessage += "Unknown OpenGL error";
            break;
        }
    }

    if (errorMessage.length() > 0)
    {
        printf("OpenGL call \'%s\' failed at %s:%d > %s\n",command,file,line,errorMessage.toUtf8().constData());
    }
}

void OpenGLFunctions::begin(GLenum mode)
{
    insideBeginEnd = true;
    glBegin(mode);
}

void OpenGLFunctions::end()
{
    GL_SAFE_CALL(glEnd());
    insideBeginEnd = false;
}

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , pickAction(false)
{
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setSamples(4);
    this->setFormat(format);
}

void OpenGLWidget::initializeGL(void)
{
    GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
    GL_SAFE_CALL(glShadeModel(GL_SMOOTH));
    GL_SAFE_CALL(glEnable(GL_MULTISAMPLE));
    GL_SAFE_CALL(glDepthFunc(GL_LEQUAL));
}

void OpenGLWidget::resizeGL(int width, int height)
{
    GL_SAFE_CALL(glViewport(0, 0, GLsizei(width), GLsizei(height)));
    GL_SAFE_CALL(glMatrixMode(GL_PROJECTION));
    GL_SAFE_CALL(glLoadIdentity());

    GLdouble winRatio = GLdouble(height)/GLdouble(width);
    GLdouble winScale = 1000.0;

    GL_SAFE_CALL(glOrtho(-1.0, 1.0, -winRatio, winRatio, -winScale, winScale));
    GL_SAFE_CALL(glMatrixMode(GL_MODELVIEW));
}

void OpenGLWidget::paintGL(void)
{
    this->qglClearColor(QColor(Qt::gray));

    if (this->pickAction)
    {
        printf("Picking\n");

        GL_SAFE_CALL(glPushMatrix());
        GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        static GLuint selectionBuffer[SELECTION_BUFFER_SIZE];
        GLdouble pickMatrixSize = 2.0;

        GLdouble winRatio = GLdouble(this->height())/GLdouble(this->width());
        GLdouble winScale = 1000.0;

        GL_SAFE_CALL(glViewport(0, 0, GLsizei(this->width()), GLsizei(this->height())));

        GL_SAFE_CALL(glMatrixMode(GL_PROJECTION));
        GL_SAFE_CALL(glPushMatrix());
        GL_SAFE_CALL(glLoadIdentity());

        GLint viewport[4];

        GL_SAFE_CALL(glGetIntegerv(GL_VIEWPORT,viewport));
        GL_SAFE_CALL(glSelectBuffer(SELECTION_BUFFER_SIZE,selectionBuffer));
        GL_SAFE_CALL(glRenderMode(GL_SELECT));

        GL_SAFE_CALL(gluPickMatrix(GLdouble(this->mousePosition.x()),
                                   GLdouble(this->mousePosition.y()),
                                   pickMatrixSize,
                                   pickMatrixSize,
                                   viewport));

        GL_SAFE_CALL(glOrtho(-1.0, 1.0, winRatio, -winRatio, -winScale, winScale));

        GL_SAFE_CALL(glMatrixMode(GL_MODELVIEW));
        GL_SAFE_CALL(glLoadIdentity());

        GL_SAFE_CALL(glInitNames());

        GL_SAFE_CALL(glColor3d(1.0,0.0,0.0));

        GL_SAFE_CALL(glPushName(1));

        OpenGLFunctions::begin(GL_POLYGON);

        GL_SAFE_CALL(glVertex3d(0.0,0.0,0.0));
        GL_SAFE_CALL(glVertex3d(0.5,0.0,0.0));
        GL_SAFE_CALL(glVertex3d(0.5,0.5,0.0));
        GL_SAFE_CALL(glVertex3d(0.0,0.5,0.0));

        OpenGLFunctions::end();

        GL_SAFE_CALL(glPopName());

        GL_SAFE_CALL(glMatrixMode(GL_PROJECTION));
        GL_SAFE_CALL(glPopMatrix());
        GL_SAFE_CALL(glMatrixMode(GL_MODELVIEW));
        GL_SAFE_CALL(glFlush());

        GL_SAFE_CALL(GLint nHits = glRenderMode(GL_RENDER));

        GL_SAFE_CALL(glPopMatrix());
        GL_SAFE_CALL(glFlush());

        this->pickAction = false;
    }
    else
    {
        printf("Drawing\n");

        GL_SAFE_CALL(glPushMatrix());
        GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        GL_SAFE_CALL(glColor3d(1.0,0.0,0.0));

        OpenGLFunctions::begin(GL_POLYGON);

        GL_SAFE_CALL(glVertex3d(0.0,0.0,0.0));
        GL_SAFE_CALL(glVertex3d(0.5,0.0,0.0));
        GL_SAFE_CALL(glVertex3d(0.5,0.5,0.0));
        GL_SAFE_CALL(glVertex3d(0.0,0.5,0.0));

        OpenGLFunctions::end();

        GL_SAFE_CALL(glPopMatrix());
        GL_SAFE_CALL(glFlush());
    }

    this->makeCurrent();

    fflush(stdout);
}

void OpenGLWidget::mousePressEvent(QMouseEvent *mouseEvent)
{
    this->mousePosition = mouseEvent->pos();
    this->pickAction = true;
    this->update();
}

void OpenGLWidget::qglClearColor(const QColor &clearColor)
{
    GL_SAFE_CALL(glClearColor(clearColor.redF(),clearColor.greenF(),clearColor.blueF(),clearColor.alphaF()));
}
