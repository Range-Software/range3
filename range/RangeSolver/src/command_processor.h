/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   command_processor.h                                      *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th August 2013                                        *
 *                                                                   *
 *  DESCRIPTION: Command processor class declaration                 *
 *********************************************************************/

#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <QObject>
#include <QCoreApplication>
#include <QLocalSocket>

class CommandProcessor : public QObject
{

    Q_OBJECT

    protected:

        //! Local socket for IPC.
        QLocalSocket *localSocket;

        //! Task server name for IPC.
        QString taskServer;

        //! Task ID.
        QString taskID;

    public:

        //! Constructor.
        explicit CommandProcessor(const QString &taskServer, const QString &taskID, QCoreApplication *application = nullptr);

    protected:

        //! Start listening and processing.
        void start(void);

        //! Stop listening and processing.
        void stop(void);
        
    public slots:

        //! Read local socket.
        void readSocket(void);
        
};

#endif // COMMAND_PROCESSOR_H
