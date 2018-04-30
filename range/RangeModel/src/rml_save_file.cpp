/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_save_file.cpp                                        *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   1-st January 2017                                        *
 *                                                                   *
 *  DESCRIPTION: Save file class definition                          *
 *********************************************************************/

#include "rml_save_file.h"

void RSaveFile::_init(const RSaveFile *pFile)
{
    if (pFile)
    {
        this->fileName = pFile->fileName;
        this->type = pFile->type;
        this->fileVersion = pFile->fileVersion;
//        this->textStream = pFile->textStream;
    }
    if (this->type == RSaveFile::ASCII)
    {
        this->textStream.setDevice(this);
    }
}

RSaveFile::RSaveFile(const QString &fileName, RSaveFile::Type type)
    : QSaveFile(fileName)
    , fileName(fileName)
    , type(type)
    , fileVersion(RVersion(0,0,0))
{
    this->_init();
}

RSaveFile::RSaveFile(const RSaveFile &file)
    : QSaveFile(fileName)
{
    this->_init(&file);
}

RSaveFile::~RSaveFile()
{

}

RSaveFile &RSaveFile::operator =(const RSaveFile &file)
{
    this->_init(&file);
    return (*this);
}

const RVersion &RSaveFile::getVersion(void) const
{
    return this->fileVersion;
}

void RSaveFile::setVersion(const RVersion &fileVersion)
{
    this->fileVersion = fileVersion;
}

QTextStream &RSaveFile::getTextStream(void)
{
    return this->textStream;
}

QString RSaveFile::getTextStreamStatusStringt(QTextStream::Status status)
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
