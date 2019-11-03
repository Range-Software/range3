/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_logger.h                                             *
 *  GROUP:  RBL                                                      *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   22-nd December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Logger class declaration                            *
 *********************************************************************/

#ifndef RBL_LOGGER_H
#define RBL_LOGGER_H

#include <QString>
#include <QTime>

#include <vector>
#include <cstdarg>

#include "rbl_message.h"

#define R_LOG_LEVEL_NORMAL static_cast<RLogLevel>(R_MESSAGE_INFO | R_MESSAGE_NOTICE | R_MESSAGE_ERROR)
#define R_LOG_LEVEL_DETAIL static_cast<RLogLevel>(R_MESSAGE_INFO | R_MESSAGE_NOTICE | R_MESSAGE_ERROR | R_MESSAGE_WARNING)
#define R_LOG_LEVEL_DEBUG  static_cast<RLogLevel>(R_MESSAGE_INFO | R_MESSAGE_NOTICE | R_MESSAGE_ERROR | R_MESSAGE_WARNING | R_MESSAGE_DEBUG)
#define R_LOG_LEVEL_TRACE  static_cast<RLogLevel>(R_MESSAGE_INFO | R_MESSAGE_NOTICE | R_MESSAGE_ERROR | R_MESSAGE_WARNING | R_MESSAGE_DEBUG | R_MESSAGE_TRACE)

#define R_LOG_LEVEL_IS_VALID(_level) \
(                                    \
  (_level == R_LOG_LEVEL_NORMAL) ||  \
  (_level == R_LOG_LEVEL_DETAIL) ||  \
  (_level == R_LOG_LEVEL_DEBUG)  ||  \
  (_level == R_LOG_LEVEL_TRACE)      \
)

typedef RMessageType RLogLevel;

typedef void (*RLogHandler) ( const RMessage &message );

//! Logger class.
class RLogger
{

    private:

        //! Internal initialization function.
        void _init ( const RLogger *pLogger = nullptr );

        //! Print to log file.
        void printToFile ( time_t         pTime,
                           const QString &cppString ) const;

        //! Insert label string to message.
        static void insertLabel(const QString &label, QString &message);

    protected:

        //! List of unprocessed messages.
        std::vector<RMessage> messages;
        //! Log file name.
        QString logFileName;
        //! Log level.
        RLogLevel logLevel;
        //! Logger halted state.
        //! If set to halt then all messages will be stored in history and
        //! printed after the logger will be unhalted.
        bool halted;
        //! If set to true a message assign time will be printed into
        //! a log file.
        bool printTime;
        //! Append new line character to ech string.
        bool addNewLine;
        //! Custom log handler.
        RLogHandler logHandler;
        //! Current indent level.
        unsigned int indentLevel;
        //! Time measurement stack.
        QList<QTime> timerStack;

    public:

        //! Constructor
        RLogger(RLogLevel logLevel = R_LOG_LEVEL_DETAIL);

        //! Copy constructor.
        RLogger(const RLogger &logger);

        //! Destructor.
        ~RLogger();

        //! Assignment operator.
        RLogger & operator = ( const RLogger &logger );

        //! Return static instance.
        static RLogger & getInstance ( void );

        //! Return log level.
        RLogLevel getLevel ( void ) const;

        //! Set log level.
        void setLevel ( RLogLevel level );

        //! Return halt state of the logger.
        bool getHalted ( void ) const;

        //! Halt/Unhalt the logger.
        //! If halted messages will not be printed into a file.
        void setHalted ( bool halt );

        //! Return true/false whether the time will be printed into
        //! a log file.
        bool getPrintTimeEnabled ( void ) const;

        //! Enable/disable print time into a log file
        void setPrintTimeEnabled ( bool printTime );

        //! Return whether a new line char should be added.
        bool getAddNewLine ( void ) const;

        //! Set whether a new line char should be added.
        void setAddNewLine ( bool addNewLine );

        //! Return log file name.
        const QString getFile ( void ) const;

        //! Set log file name.
        void setFile ( const QString & logFileName );

        //! Return custom log handler.
        RLogHandler getLogHandler ( void ) const;

        //! Set custom log handler.
        void setLogHandler ( RLogHandler logHandler );

        //! Return current indent level.
        unsigned int getIndentLevel ( void ) const;

        //! Set current indent level.
        void setIndentLevel ( unsigned int indentLevel );

        //! Increase current indent level.
        void increaseIndent ( void );

        //! Decrease current indent level.
        void decreaseIndent ( void );

        //! Print message.
        void print ( const RMessage &message);

        //! Print c++ string
        void print ( const QString &cppString,
                     RMessageType   messageType = R_MESSAGE_INFO );

        //! Print c string
        void print ( const char   *cString,
                     RMessageType  messageType = R_MESSAGE_INFO );

        //! Print c char
        void print ( const char   cChar,
                     RMessageType messageType = R_MESSAGE_INFO );

        //! Flush all unprocessed messages to file.
        void flush ( void );

        //! Purge unprocessed messages.
        //! If nMessages is set to 0 all messages will be purged.
        void purge ( unsigned int nMessages = 0 );

        //! Convenience function to log trace message.
        static int trace ( const char *format, ... );

        //! Convenience function to log debug message.
        static int debug ( const char *format, ... );

        //! Convenience function to log info message.
        static int info ( const char *format, ... );

        //! Convenience function to log notice message.
        static int notice ( const char *format, ... );

        //! Convenience function to log warning message.
        static int warning( const char *format, ... );

        //! Convenience function to log error message.
        static int error ( const char *format, ... );

        //! Convenience function to print timestamp.
        static void timestamp ( const QString prefix = QString() );

        //! Convenience function to increase indent.
        static void indent ( void );

        //! Convenience function to decrease indent.
        static void unindent ( bool printTime = true );

};

#endif /* RBL_LOGGER_H */
