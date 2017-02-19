/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_file.cpp                                             *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   9-th November 2015                                       *
 *                                                                   *
 *  DESCRIPTION: File class definition                               *
 *********************************************************************/

#include "rml_file.h"

void RFile::_init(const RFile *pFile)
{
    if (pFile)
    {
        this->fileName = pFile->fileName;
        this->type = pFile->type;
        this->fileVersion = pFile->fileVersion;
//        this->textStream = pFile->textStream;
    }
    if (this->type == RFile::ASCII)
    {
        this->textStream.setDevice(this);
    }
}

RFile::RFile(const QString &fileName, RFile::Type type)
    : QFile(fileName)
    , fileName(fileName)
    , type(type)
    , fileVersion(RVersion(0,0,0))
{
    this->_init();
}

RFile::RFile(const RFile &file)
    : QFile(fileName)
{
    this->_init(&file);
}

RFile::~RFile()
{

}

RFile &RFile::operator =(const RFile &file)
{
    this->_init(&file);
    return (*this);
}

const RVersion &RFile::getVersion(void) const
{
    return this->fileVersion;
}

void RFile::setVersion(const RVersion &fileVersion)
{
    this->fileVersion = fileVersion;
}

QTextStream &RFile::getTextStream(void)
{
    return this->textStream;
}

QString RFile::getTextStreamStatusStringt(QTextStream::Status status)
{
    switch (status)
    {
        case QTextStream::Ok:
            return "ok";
        case QTextStream::ReadPastEnd:
            return "read past end";
        case QTextStream::ReadCorruptData:
            return "read corrupt data";
        case QTextStream::WriteFailed:
            return "write failed";
        default:
            return "unknown";
    }
}
