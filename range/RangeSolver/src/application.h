/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   application.h                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   12-th July 2013                                          *
 *                                                                   *
 *  DESCRIPTION: Application class declaration                       *
 *********************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QCoreApplication>

class Application : public QCoreApplication
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit Application(int &argc, char **argv);

    signals:

        //! Application has started.
        void started(void);

    protected slots:

        //! Catch started signal.
        void onStarted(void);
        
};

#endif // APPLICATION_H
