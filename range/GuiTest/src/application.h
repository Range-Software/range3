#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class Application : public QApplication
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit Application(int &argc, char **argv);

        //! Apply style.
        void applyStyle(const QString &styleName);

    signals:

        //! Application has started.
        void started(void);

    public slots:

        //! Catch started signal.
        void onStyleChanged(const QString &styleName);

};

#endif // APPLICATION_H
