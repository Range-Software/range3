#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{

    Q_OBJECT

    public:

        explicit MainWindow(QWidget *parent = 0);

    protected:

        //! Overloaded closeEvent function to catch close event.
        void closeEvent(QCloseEvent *event);

    signals:

    public slots:
};

#endif // MAINWINDOW_H
