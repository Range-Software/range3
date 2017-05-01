#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>

#include <qglfunctions.h>

#define GL_SAFE_CALL(__safe_call) __safe_call; OpenGLFunctions::printError(#__safe_call,__FILE__,__LINE__)

namespace OpenGLFunctions
{

    //! Wrapper for glBegin().
    void begin(GLenum  mode);

    //! Wrapper for glEnd().
    void end();

    void printError(const char *command, const char *file, unsigned int line);

}

class OpenGLWidget : public QOpenGLWidget
{

    Q_OBJECT

    protected:

        QPoint mousePosition;
        bool pickAction;

    public:

        explicit OpenGLWidget(QWidget *parent = 0);

    protected:

        //! Initialize scene.
        void initializeGL(void);

        //! Resize scene.
        void resizeGL(int width, int height);

        //! Paint scene.
        void paintGL(void);

        void mousePressEvent(QMouseEvent *mouseEvent);

        void qglClearColor(const QColor &clearColor);

};

#endif // GLWIDGET_H
