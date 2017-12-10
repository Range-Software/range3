/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_message.cpp                                          *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   22-nd December 2011                                      *
 *                                                                   *
 *  DESCRIPTION: Message class definition                            *
 *********************************************************************/

#include <QStringList>

#include <time.h>

#include "rbl_message.h"
#include "rbl_error.h"

RMessage::RMessage (RMessageType type)
{
    this->_init ();
    this->setAtimeFrozen(false);
    this->setType (type);
} /* RMessage::RMessage */


RMessage::RMessage (const RMessage &message) : QString (message)
{
    this->_init (&message);
    this->setAtimeFrozen(false);
    this->setType (type);
} /* RMessage::RMessage (copy) */


RMessage::RMessage (const QString &cppString, RMessageType type) : QString (cppString)
{
    this->_init ();
    this->setAtimeFrozen(false);
    this->setType (type);
} /* RMessage::RMessage (copy c++ string) */


RMessage::RMessage (const char *cString, RMessageType type) : QString (cString)
{
    this->_init ();
    this->setAtimeFrozen(false);
    this->setType (type);
} /* RMessage::RMessage (copy c string) */


RMessage::RMessage (size_t n, char cChar, RMessageType type) : QString (int(n),cChar)
{
    this->_init ();
    this->setAtimeFrozen(false);
    this->setType (type);
} /* RMessage::RMessage (copy c char) */


RMessage::~RMessage ()
{
} /* RMessage::~RMessage */


void RMessage::_init (const RMessage *pMessage)
{
    this->setAtimeToNow ();
    if (pMessage)
    {
        if (pMessage->getAtimeFrozen())
        {
            this->aTime = pMessage->getAtime();
        }
        this->setAtimeFrozen (pMessage->getAtimeFrozen());
        this->setType (pMessage->getType());
    }
} /* RMessage::_init */


RMessageType RMessage::getType (void) const
{
    return this->type;
} /* RMessage::get_type */


void RMessage::setType (RMessageType type)
{
    R_ERROR_ASSERT (R_MESSAGE_TYPE_IS_VALID (type));

    this->type = type;
} /* RMessage::set_type */


time_t RMessage::getAtime ( void ) const
{
    return this->aTime;
} /* RMessage::get_atime */

void RMessage::setAtimeToNow ( void )
{
    this->aTime = time(0);
} /* RMessage::set_atime_to_now */


RMessage & RMessage::operator = (const RMessage &message)
{
    this->QString::operator = (message);
    this->_init (&message);
    return (*this);
} /* RMessage::operator = (message) */


RMessage & RMessage::operator = (const QString &cppString)
{
    this->QString::operator = (cppString);
    this->_init ();
    return (*this);
} /* RMessage::operator = (cpp string) */


RMessage & RMessage::operator = (const char *cString)
{
    this->QString::operator = (cString);
    this->_init ();
    return (*this);
} /* RMessage::operator = (c string) */


RMessage & RMessage::operator = (char cChar)
{
    this->QString::operator = (cChar);
    this->_init ();
    return (*this);
}

std::vector<QString> RMessage::explode(const QString &str, char ch, bool keepCh)
{
    std::vector<QString> result;

    QStringList query = str.split(QRegExp(QString("(\\") + ch + ")"));

    for (int i=0;i<query.size();i++)
    {
        if (i == query.size()-1 && query[i].isEmpty() && query.size() > 1)
        {
            continue;
        }
        if (keepCh && query.size() > 1)
        {
            query[i].append(ch);
        }
        result.push_back(query[i]);
    }

    return result;
} /* RMessage::operator = (c char) */

