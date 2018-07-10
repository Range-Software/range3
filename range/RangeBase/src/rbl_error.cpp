/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_error.cpp                                            *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   11-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Error class definition                              *
 *********************************************************************/

#include <sstream>
#include <string>
#include <cstring>
#include <cstdio>

#ifdef DEBUG
#  include <assert.h>
#endif

#include "rbl_error.h"


const QString errorTypeMessages [] =
{
    "No error.",
    "Failed to open the file.",
    "Failed to read the file.",
    "Failed to write the file.",
    "Failed to remove the file.",
    "Failed to rename the file.",
    "Invalid file name.",
    "Invalid file format.",
    "Failed to open the directory.",
    "Failed to read the directory.",
    "Application error.",
    "Invalid input.",
    "Unknown error."
};


RError::RError(RErrorType type, const char *file, unsigned int line, const QString &message)
{
    this->_init();
    this->setType(type);
    QString devInfo;
#ifdef DEBUG
    devInfo = QString(file) + "@" + QString::number(line) + ":";
#else
    // Suppress unused parameter warnings
    (void)file;
    (void)line;
#endif
    this->setMessage(this->getMessage() + " " + devInfo + message);
} /* RError::RError */


RError::RError(RErrorType type, const char *file, unsigned int line, const char *format, ...)
{
    QString buffer;

#ifdef DEBUG
    buffer = QString::asprintf("%s@%u: ",file,line);
#else
    // Suppress unused parameter warnings
    (void)file;
    (void)line;
#endif

    va_list ap;
    va_start(ap, format);
    buffer += QString::vasprintf(format,ap);
    va_end(ap);
    this->_init();
    this->setType(type);
    this->setMessage(this->getMessage() + " " + buffer);
} /* RError::RError */


RError::RError(const RError &error)
{
    this->_init(&error);
} /* RError::RError(copy) */


RError::~RError()
{
} /* RError::~RError */


void RError::_init(const RError *pError)
{
    if (pError)
    {
        this->type = pError->type;
        this->message = pError->message;
    }
} /* RError::_init */


RError & RError::operator =(const RError &error)
{
    this->_init(&error);
    return (*this);
} /* RError::operator = */


RErrorType RError::getType() const
{
    return this->type;
} /* RError::getType */


void RError::setType(RErrorType type)
{
#ifdef DEBUG
    assert (R_ERROR_IS_VALID(type));
#endif
    this->type = type;
    this->setMessage(RErrorTypeGetMessage(this->type));
} /* RError::setType */


QString RError::getMessage() const
{
    return this->message;
} /* RError::getMessage */


void RError::setMessage(QString message)
{
    this->message = message;
} /* RError::setMessage */


const QString & RErrorTypeGetMessage (RErrorType type)
{
#ifdef DEBUG
    assert (R_ERROR_IS_VALID(type));
#endif
    return errorTypeMessages[type];
} /* RErrorTypeGetMessage */
