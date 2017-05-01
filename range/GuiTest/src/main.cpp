#include <QtGui>

#include "application.h"
#include "mainwindow.h"

int main (int argc, char *argv[])
{
    QLocale::setDefault(QLocale("en_US"));
    Application app(argc, argv);

    MainWindow *mainWindow = new MainWindow;
    mainWindow->show();

    int exitValue = 0;

    if ((exitValue = app.exec()) != 0)
    {
        printf("Application has terminated with error code (%d).\n", exitValue);
    }
    else
    {
        printf("Application has terminated successfully.\n");
    }

    delete mainWindow;

    return exitValue;
}
