/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   application.h                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   28-th June 2013                                          *
 *                                                                   *
 *  DESCRIPTION: Application class declaration                       *
 *********************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QPalette>
#include <QLockFile>

#include <rblib.h>

class Application : public QApplication
{

    Q_OBJECT

    protected:

        //! Lock file.
        QLockFile *pLockFile;

    public:

        //! Constructor.
        explicit Application(int &argc, char **argv);

        //! Apply style.
        void applyStyle(const QString &styleName);
        
    signals:

        //! Application has started.
        void started(void);

    protected slots:

        //! Catch started signal.
        void onStarted(void);

        //! Catch aboutToQuit signal.
        void onAboutToQuit(void);

        //! Catch started signal.
        void onStyleChanged(const QString &styleName);

        //! Software version is available.
        void onAvailableSoftware(const RVersion &version, const QString &link);

        //! User signed in.
        void onSignedIn(void);

        //! User signed out.
        void onSignedOut(void);
        
};

#endif // APPLICATION_H
