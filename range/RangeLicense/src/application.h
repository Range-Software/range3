/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   application.h                                            *
 *  GROUP:  RangeLicense                                             *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   7-th September 2016                                      *
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
        
};

#endif // APPLICATION_H
