/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_logger.cpp                                           *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   22-nd December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Logger class definition                             *
 *********************************************************************/

#include <QFile>
#include <QTextStream>

#include <vector>
#include <string>

#include "rbl_logger.h"
#include "rbl_locker.h"
#include "rbl_error.h"


void RLogger::_init (const RLogger *pLogger)
{
    if (pLogger)
    {
        this->setLevel (pLogger->getLevel());
        this->setHalted (pLogger->getHalted());
        this->setPrintTimeEnabled (pLogger->getPrintTimeEnabled());
        this->setAddNewLine (pLogger->getAddNewLine());
        this->setFile (pLogger->getFile());
        this->setLogHandler (pLogger->getLogHandler());
        this->setIndentLevel (pLogger->getIndentLevel());
        // Copy unprocessed messages.
    }
} /* RLogger::_init */


RLogger::RLogger (RLogLevel logLevel) : logHandler(0)
                                      , indentLevel(0)
{
    this->_init ();
    this->setLevel (logLevel);
    this->setHalted (false);
    this->setPrintTimeEnabled (true);
    this->setAddNewLine (false);
} /* RLogger::RLogger */


RLogger::RLogger (const RLogger &logger)
{
    this->_init (&logger);
} /* RLogger::RLogger (copy) */


RLogger::~RLogger ()
{
} /* RLogger::~RLogger */


RLogger & RLogger::operator = (const RLogger &logger)
{
    this->_init (&logger);
    return (*this);
} /* RLogger::operator = */


RLogger & RLogger::getInstance()
{
    static RLogger logger;
    return logger;
} /* rbl_logger_get_default_instance */


RLogLevel RLogger::getLevel (void) const
{
    RLogLevel tmpLevel;
    RLocker::lock();
    tmpLevel = this->logLevel;
    RLocker::unlock();
    return tmpLevel;
} /* RLogger::getLevel */


void RLogger::setLevel (RLogLevel level)
{
    R_ERROR_ASSERT(R_LOG_LEVEL_IS_VALID (level));

    RLocker::lock();
    this->logLevel = level;
    RLocker::unlock();
} /* RLogger::setLevel */


bool RLogger::getHalted (void) const
{
    bool tmpHalted;
    RLocker::lock();
    tmpHalted = this->halted;
    RLocker::unlock();
    return tmpHalted;
} /* RLogger::getHalted */


void RLogger::setHalted (bool halt)
{
    RLocker::lock();
    this->halted = halt;
    RLocker::unlock();
    if (!this->getHalted())
    {
        this->flush();
    }
} /* RLogger::setHalted */


bool RLogger::getPrintTimeEnabled (void) const
{
    bool tmpTime;
    RLocker::lock();
    tmpTime = this->printTime;
    RLocker::unlock();
    return tmpTime;
} /* RLogger::getPrintTimeEnabled */


void RLogger::setPrintTimeEnabled (bool printTime)
{
    RLocker::lock();
    this->printTime = printTime;
    RLocker::unlock();
} /* RLogger::setPrintTimeEnabled */


bool RLogger::getAddNewLine (void) const
{
    bool tmpNewLine;
    RLocker::lock();
    tmpNewLine = this->addNewLine;
    RLocker::unlock();
    return tmpNewLine;
} /* RLogger::getAddNewLine */


void RLogger::setAddNewLine (bool addNewLine)
{
    RLocker::lock();
    this->addNewLine = addNewLine;
    RLocker::unlock();
} /* RLogger::setAddNewLine */


const QString RLogger::getFile (void) const
{
    QString tmpFileName;
    RLocker::lock();
    tmpFileName = this->logFileName;
    RLocker::unlock();
    return tmpFileName;
} /* RLogger::getFile */


void RLogger::setFile (const QString & logFileName)
{
    RLocker::lock();
    this->logFileName = logFileName;
    RLocker::unlock();
} /* RLogger::setFile */


RLogHandler RLogger::getLogHandler (void) const
{
    RLogHandler tmpHandler;
    RLocker::lock();
    tmpHandler = this->logHandler;
    RLocker::unlock();
    return tmpHandler;
} /* RLogger::getLogHandler */


void RLogger::setLogHandler (RLogHandler logHandler)
{
    RLocker::lock();
    this->logHandler = logHandler;
    RLocker::unlock();
} /* RLogger::setLogHandler */


unsigned int RLogger::getIndentLevel (void) const
{
    unsigned int tmpLevel;
    RLocker::lock();
    tmpLevel = this->indentLevel;
    RLocker::unlock();
    return tmpLevel;
} /* RLogger::getIndentLevel */


void RLogger::setIndentLevel (unsigned int indentLevel)
{
    RLocker::lock();
    this->indentLevel = indentLevel;
    RLocker::unlock();
} /* RLogger::setIndentLevel */


void RLogger::increaseIndent (void)
{
    RLocker::lock();
    this->indentLevel++;
    RLocker::unlock();
} /* RLogger::increaseIndent */


void RLogger::decreaseIndent (void)
{
    RLocker::lock();
    if (this->indentLevel > 0)
    {
        this->indentLevel--;
    }
    RLocker::unlock();
} /* RLogger::decreaseIndent */


void RLogger::printToFile (time_t         pTime,
                           const QString &cppString) const
{
    if (this->logFileName.isEmpty())
    {
        return;
    }

    QFile logFile(this->logFileName);

    if (!logFile.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream(stderr) << "Failed to open the log file '" << this->logFileName << "'\n";
        return;
    }

    QTextStream out(&logFile);

    if (this->printTime)
    {
        char buffer [80];
        strftime (buffer,80,"%Y/%m/%d - %H:%M:%S > ",localtime (&pTime));
        out << buffer;
    }
    out << cppString;
    if (out.status() != QTextStream::Ok)
    {
        QTextStream(stderr) << "Failed to write the log message to file '" << this->logFileName << "'\n";
    }

    logFile.close ();
} /* RLogger::printToFile */


void RLogger::insertLabel(const QString &label, QString &message)
{
    for (int i=0;i<message.length();i++)
    {
        if (!message.at(i).isSpace())
        {
            message.insert(i,label+": ");
            break;
        }
    }
} /* RLogger::insertLabel */


void RLogger::print (const RMessage &message)
{
    RMessageType messageType;
    messageType = message.getType();
    QString fullMessage;
    bool printToStderr = false;

    if (!(messageType & this->getLevel()))
    {
        // Message is out of the log level, so it will be dropped.
        return;
    }

    for (unsigned int i=0;i<this->getIndentLevel();i++)
    {
        fullMessage += "  ";
    }
    fullMessage += message;
    if (this->getAddNewLine())
    {
        fullMessage += '\n';
    }

    switch (messageType)
    {
        case R_MESSAGE_ERROR:
            RLogger::insertLabel("ERROR",fullMessage);
            printToStderr = true;
            break;
        case R_MESSAGE_WARNING:
            RLogger::insertLabel("WARNING",fullMessage);
            printToStderr = true;
            break;
        case R_MESSAGE_DEBUG:
            RLogger::insertLabel("DEBUG",fullMessage);
            printToStderr = false;
            break;
        case R_MESSAGE_TRACE:
            RLogger::insertLabel("TRACE",fullMessage);
            printToStderr = false;
            break;
        case R_MESSAGE_NOTICE:
            RLogger::insertLabel("NOTICE",fullMessage);
            printToStderr = false;
            break;
        case R_MESSAGE_INFO:
        default:
            printToStderr = false;
            break;
    }

    if (printToStderr)
    {
        QTextStream(stderr) << fullMessage;
        QTextStream(stderr).flush();
    }
    else
    {
        QTextStream(stdout) << fullMessage;
        QTextStream(stdout).flush();

    }

    if (this->getHalted())
    {
        RLocker::lock();
        // Print to file is halted => store message
        this->messages.push_back (fullMessage);
        RLocker::unlock();
    }
    else
    {
        RLocker::lock();
        this->printToFile (message.getAtime(), fullMessage);
        if (this->logHandler)
        {
            this->logHandler(RMessage(fullMessage, messageType));
        }
        RLocker::unlock();
    }
} /* RLogger::print */


void RLogger::print (const QString &cppString,
                     RMessageType   messageType)
{
    std::vector<QString> messages = RMessage::explode(cppString,'\n',true);
    for (unsigned int i=0;i<messages.size();i++)
    {
        RMessage message(messages[i]);
        message.setType (messageType);
        this->print (message);
    }
} /* RLogger::print */


void RLogger::print (const char   *cString,
                     RMessageType  messageType)
{
    this->print (QString(cString),messageType);
} /* RLogger::print */


void RLogger::print (const char   cChar,
                     RMessageType messageType)
{
    RMessage message(1,cChar);
    message.setType (messageType);
    this->print (message);
} /* RLogger::print */


void RLogger::flush (void)
{
    std::vector<RMessage>::iterator iter;

    RLocker::lock();
    for (iter = this->messages.begin();
         iter != this->messages.end();
         ++iter)
    {
        this->printToFile (iter->getAtime(), *iter);
        if (this->logHandler)
        {
            this->logHandler(*iter);
        }
    }
    RLocker::unlock();
    this->purge();
} /* RLogger::flush */


void RLogger::purge (unsigned int nMessages)
{
    RLocker::lock();
    if (nMessages == 0)
    {
        this->messages.clear ();
        RLocker::unlock();
        return;
    }

    unsigned int n;
    std::vector<RMessage>::iterator iter = this->messages.begin();

    n = (unsigned int)this->messages.size();
    if (n > nMessages)
    {
        n = nMessages;
    }

    std::advance(iter,n);

    this->messages.erase (this->messages.begin(),iter);
    RLocker::unlock();
} /* RLogger::purge */


int RLogger::trace(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    RLogger::getInstance().print(QString::vasprintf(format, ap), R_MESSAGE_TRACE);
    va_end(ap);
    return 0;
} /* RLogger::trace */


int RLogger::debug(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    RLogger::getInstance().print(QString::vasprintf(format, ap), R_MESSAGE_DEBUG);
    va_end(ap);
    return 0;
} /* RLogger::debug */


int RLogger::info(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    RLogger::getInstance().print(QString::vasprintf(format, ap), R_MESSAGE_INFO);
    va_end(ap);
    return 0;
} /* RLogger::info */


int RLogger::notice(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    RLogger::getInstance().print(QString::vasprintf(format, ap), R_MESSAGE_NOTICE);
    va_end(ap);
    return 0;
} /* RLogger::notice */


int RLogger::warning(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    RLogger::getInstance().print(QString::vasprintf(format, ap), R_MESSAGE_WARNING);
    va_end(ap);
    return 0;
} /* RLogger::warning */


int RLogger::error(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    RLogger::getInstance().print(QString::vasprintf(format, ap), R_MESSAGE_ERROR);
    va_end(ap);
    return 0;
} /* RLogger::error */


void RLogger::timestamp(const QString prefix)
{
    QDateTime timeStamp = QDateTime::currentDateTime();
    RLogger::info("%s%s%s\n",
                  prefix.toUtf8().constData(),
                  prefix.size()>0?" ":"",
                  timeStamp.toString("dd/MM/yyyy hh:mm:ss").toUtf8().constData());
} /* RLogger::timestamp */


void RLogger::indent (void)
{
    RLogger::getInstance().timerStack.push_front(QTime());
    RLogger::getInstance().timerStack.first().start();
    RLogger::info("{\n");
    RLogger::getInstance().increaseIndent();
} /* RLogger::indent */


void RLogger::unindent (bool printTime)
{
    RLogger::getInstance().decreaseIndent();
    int elapsed = qRound(double(RLogger::getInstance().timerStack.first().elapsed())/1000.0);
    if (printTime)
    {
        RLogger::info("} %s\n", QDateTime::fromTime_t(elapsed).toUTC().toString("hh:mm:ss").toUtf8().constData());
    }
    else
    {
        RLogger::info("}\n");
    }
    RLogger::getInstance().timerStack.pop_front();
} /* RLogger::unindent */
