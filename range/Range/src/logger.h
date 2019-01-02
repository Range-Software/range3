/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   logger.h                                                 *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   3-rd January 2012                                        *
 *                                                                   *
 *  DESCRIPTION: Logger functions                                    *
 *********************************************************************/

#ifndef LOGGER_H
#define LOGGER_H

#include <cstdarg>
#include <QObject>

#include "main_window.h"

class Logger : public QObject
{

    Q_OBJECT

    protected:

        //! Constructor.
        inline Logger() { }

    public:

        //! Initialize RProgress static instance.
        static void initialize(void);

        //! Disable message sending to GUI.
        static void halt(void);

        //! Enable message sending to GUI.
        static void unhalt(void);

        //! Return reference to static object.
        static Logger & getInstance(void);

        //! Log handler function.
        static void logHandler (const RMessage &message);

    signals:

        //! Signal containing info message.
        void infoPrinted(const QString &message);

        //! Signal containing notice message.
        void noticePrinted(const QString &message);

        //! Signal containing warning message.
        void warningPrinted(const QString &message);

        //! Signal containing error message.
        void errorPrinted(const QString &message);
};

#endif /* LOGGER_H */
