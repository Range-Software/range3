#include <QDebug>
#include <QCloseEvent>

#include "mainwindow.h"
#include "glwidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    OpenGLWidget *glWidget = new OpenGLWidget;

    this->setCentralWidget(glWidget);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    printf("Closing\n");
    event->accept();
}
