/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_file_io.cpp                                          *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   21-th January 2012                                       *
 *                                                                   *
 *  DESCRIPTION: File IO operation functions definitions             *
 *********************************************************************/

#include "rml_file_io.h"


void RFileIO::writeNewLineAscii(RSaveFile &outFile)
{
    outFile.getTextStream() << RConstants::endl;
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write the new line.");
    }
} /* RFileIO::writeNewLineAscii */


void RFileIO::writeNewLineAscii(RFile &outFile)
{
    outFile.getTextStream() << RConstants::endl;
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write the new line.");
    }
} /* RFileIO::writeNewLineAscii */


/*********************************************************************
 *  bool                                                             *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, bool &bValue)
{
    char c;
    inFile.getTextStream() >> c;
    bValue = (c == 0);
    if (inFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to read bool value.");
    }
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, bool &bValue)
{
    inFile.read((char*)&bValue,sizeof(bool));
    if (inFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read bool value.");
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const bool &bValue, bool addNewLine)
{
    if (!addNewLine)
    {
        outFile.getTextStream() << bValue;
    }
    else
    {
        outFile.getTextStream() << char(bValue) << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write bool value.");
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const bool &bValue)
{
    outFile.write((char*)&bValue,sizeof(bool));
    if (outFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write bool value.");
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  char                                                             *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, char &cValue)
{
    inFile.getTextStream() >> cValue;
    if (inFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to read char value.");
    }
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, char &cValue)
{

    inFile.read((char*)&cValue,sizeof(char));
    if (inFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read char value.");
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const char &cValue, bool addNewLine)
{
    if (!addNewLine)
    {
        outFile.getTextStream() << cValue;
    }
    else
    {
        outFile.getTextStream() << cValue << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write char value.");
    }
} /* RFileIO::writeAscii */


void RFileIO::writeAscii(RFile &outFile, const char &cValue, bool addNewLine)
{
    if (!addNewLine)
    {
        outFile.getTextStream() << cValue;
    }
    else
    {
        outFile.getTextStream() << cValue << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write char value.");
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const char &cValue)
{
    outFile.write((char*)&cValue,sizeof(char));
    if (outFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write char value.");
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  int                                                              *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, int &iValue)
{
    inFile.getTextStream() >> iValue;
    if (inFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to read int value.");
    }
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, int &iValue)
{
    inFile.read((char*)&iValue,sizeof(int));
    if (inFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read int value.");
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const int &iValue, bool addNewLine)
{
    if (!addNewLine)
    {
        outFile.getTextStream() << iValue;
    }
    else
    {
        outFile.getTextStream() << iValue << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write int value.");
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const int &iValue)
{
    outFile.write((char*)&iValue,sizeof(int));
    if (outFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write int value.");
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  unsigned int                                                     *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, unsigned int &uValue)
{
    inFile.getTextStream() >> uValue;
    if (inFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to read unsigned int value.");
    }
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, unsigned int &uValue)
{
    inFile.read((char*)&uValue,sizeof(unsigned int));
    if (inFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read unsigned int value.");
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const unsigned int &uValue, bool addNewLine)
{
    if (!addNewLine)
    {
        outFile.getTextStream() << uValue;
    }
    else
    {
        outFile.getTextStream() << uValue << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write unsigned int value.");
    }
} /* RFileIO::writeAscii */


void RFileIO::writeAscii(RFile &outFile, const unsigned int &uValue, bool addNewLine)
{
    if (!addNewLine)
    {
        outFile.getTextStream() << uValue;
    }
    else
    {
        outFile.getTextStream() << uValue << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write unsigned int value.");
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const unsigned int &uValue)
{
    outFile.write((char*)&uValue,sizeof(unsigned int));
    if (outFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write unsigned int value.");
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  double                                                           *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, double &dValue)
{
    inFile.getTextStream() >> dValue;
    if (inFile.getTextStream().status() != QTextStream::Ok)
    {
        if (inFile.getTextStream().status() == QTextStream::ReadCorruptData)
        {
            inFile.getTextStream().resetStatus();
            return;
        }
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to read double value.");
    }
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, double &dValue)
{
    inFile.read((char*)&dValue,sizeof(double));
    if (inFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read double value.");
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const double &dValue, bool addNewLine)
{
    if (!addNewLine)
    {
        outFile.getTextStream() << dValue;
    }
    else
    {
        outFile.getTextStream() << dValue << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write double value.");
    }
} /* RFileIO::writeAscii */


void RFileIO::writeAscii(RFile &outFile, const double &dValue, bool addNewLine)
{
    if (!addNewLine)
    {
        outFile.getTextStream() << dValue;
    }
    else
    {
        outFile.getTextStream() << dValue << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write double value.");
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const double &dValue)
{
    outFile.write((char*)&dValue,sizeof(double));
    if (outFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write double value.");
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  QString                                                          *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, QString &sValue)
{
//    QTextCodec *textCodec = inFile.codec();
//    bool autoDetectUnicode = inFile.autoDetectUnicode();

//    inFile.setCodec("UTF-8");
//    inFile.setAutoDetectUnicode(true);

    sValue.clear();
    QChar endChar = '\0';

    bool sStarted = false;
    bool cStarted = false;
    bool readNext = true;
    while (readNext)
    {
        QChar c;
        inFile.getTextStream() >> c;
        if (inFile.getTextStream().status() != QTextStream::Ok)
        {
            throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to read string value.");
        }
        if (c == '"')
        {
            if (cStarted)
            {
                readNext = false;
            }
            cStarted = true;
        }
        else if (c.isSpace())
        {
            if (!cStarted && sStarted)
            {
                readNext = false;
            }
            if (cStarted && sStarted)
            {
                sValue += c;
                sStarted = true;
            }
        }
        else
        {
            sValue += c;
            sStarted = true;
        }
    }

    if (!sValue.isEmpty())
    {
        if (sValue[sValue.length()-1] == endChar && sValue.length() > 1)
        {
            sValue.remove(sValue.length()-1,1);
        }
        if (sValue[0] == endChar)
        {
            sValue.remove(0,1);
        }
    }

//    inFile.setCodec(textCodec);
//    inFile.setAutoDetectUnicode(autoDetectUnicode);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, QString &sValue)
{
    int stringSize = 0;
    RFileIO::readBinary(inFile,stringSize);
    QByteArray array = inFile.read(stringSize);
    if (inFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF, "Failed to read string value.");
    }
    sValue.clear();
    sValue.append(array);
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const QString &sValue, bool addNewLine)
{
//    QTextCodec *textCodec = outFile.codec();
//    outFile.setCodec("UTF-8");
    QString oValue(sValue);
    if (oValue.isEmpty())
    {
        oValue = "\"\"";
    }
    if (!addNewLine)
    {
        outFile.getTextStream() << oValue;
    }
    else
    {
        outFile.getTextStream() << oValue << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF, "Failed to write string value.");
    }
//    outFile.setCodec(textCodec);
} /* RFileIO::writeAscii */


void RFileIO::writeAscii(RFile &outFile, const QString &sValue, bool addNewLine)
{
//    QTextCodec *textCodec = outFile.codec();
//    outFile.setCodec("UTF-8");
    QString oValue(sValue);
    if (oValue.isEmpty())
    {
        oValue = "\"\"";
    }
    if (!addNewLine)
    {
        outFile.getTextStream() << oValue;
    }
    else
    {
        outFile.getTextStream() << oValue << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF, "Failed to write string value.");
    }
//    outFile.setCodec(textCodec);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const QString &sValue)
{
    QByteArray array = sValue.toUtf8();
    RFileIO::writeBinary(outFile,array.size());
    outFile.write(array,array.size());
    if (outFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF, "Failed to write string value.");
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  std::string                                                      *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, std::string &sValue)
{
    QString qString;
    RFileIO::readAscii(inFile,qString);
    sValue = qString.toStdString();
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, std::string &sValue)
{
    QString qString;
    RFileIO::readBinary(inFile,qString);
    sValue = qString.toStdString();
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const char *sValue, bool addNewLine)
{
    RFileIO::writeAscii(outFile,QString(sValue),addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeAscii(RSaveFile &outFile, const std::string &sValue, bool addNewLine)
{
    RFileIO::writeAscii(outFile,QString(sValue.c_str()),addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const char *sValue)
{
    RFileIO::writeBinary(outFile,QString(sValue));
} /* RFileIO::writeBinary */


void RFileIO::writeBinary(RSaveFile &outFile, const std::string &sValue)
{
    RFileIO::writeBinary(outFile,QString(sValue.c_str()));
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RIVector                                                         *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RIVector &iVector, bool readSize)
{
    unsigned int nr = 0;

    if (readSize)
    {
        RFileIO::readAscii(inFile,nr);
        iVector.resize(nr);
    }
    else
    {
        nr = iVector.getNRows();
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::readAscii(inFile,iVector[i]);
    }
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RIVector &iVector, bool readSize)
{
    unsigned int nr = 0;

    if (readSize)
    {
        RFileIO::readBinary(inFile,nr);
        iVector.resize(nr);
    }
    else
    {
        nr = iVector.getNRows();
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::readBinary(inFile,iVector[i]);
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RIVector &iVector, bool writeSize, bool addNewLine)
{
    unsigned int nr = iVector.getNRows();
    if (writeSize)
    {
        if (addNewLine)
        {
            RFileIO::writeAscii(outFile,nr,true);
        }
        else
        {
            RFileIO::writeAscii(outFile,nr,false);
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::writeAscii(outFile,iVector[i], false);
        if (i+1 < nr)
        {
            RFileIO::writeAscii(outFile,' ', false);
        }
    }
    if (addNewLine)
    {
        RFileIO::writeNewLineAscii(outFile);
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RIVector &iVector, bool writeSize)
{
    unsigned int nr = iVector.getNRows();
    if (writeSize)
    {
        RFileIO::writeBinary(outFile,nr);
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::writeBinary(outFile,iVector[i]);
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RUVector                                                         *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RUVector &uVector, bool readSize)
{
    unsigned int nr = 0;

    if (readSize)
    {
        RFileIO::readAscii(inFile,nr);
        uVector.resize(nr);
    }
    else
    {
        nr = uVector.getNRows();
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::readAscii(inFile,uVector[i]);
    }
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RUVector &uVector, bool readSize)
{
    unsigned int nr = 0;

    if (readSize)
    {
        RFileIO::readBinary(inFile,nr);
        uVector.resize(nr);
    }
    else
    {
        nr = uVector.getNRows();
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::readBinary(inFile,uVector[i]);
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RUVector &uVector, bool writeSize, bool addNewLine)
{
    unsigned int nr = uVector.getNRows();
    if (writeSize)
    {
        if (addNewLine)
        {
            RFileIO::writeAscii(outFile,nr,true);
        }
        else
        {
            RFileIO::writeAscii(outFile,nr,false);
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::writeAscii(outFile,uVector[i], false);
        if (i+1 < nr)
        {
            RFileIO::writeAscii(outFile,' ', false);
        }
    }
    if (addNewLine)
    {
        RFileIO::writeNewLineAscii(outFile);
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RUVector &uVector, bool writeSize)
{
    unsigned int nr = uVector.getNRows();
    if (writeSize)
    {
        RFileIO::writeBinary(outFile,nr);
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::writeBinary(outFile,uVector[i]);
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RRVector                                                         *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RRVector &rVector, bool readSize)
{
    unsigned int nr = 0;

    if (readSize)
    {
        RFileIO::readAscii(inFile,nr);
        rVector.resize(nr);
    }
    else
    {
        nr = rVector.getNRows();
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::readAscii(inFile,rVector[i]);
    }
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RRVector &rVector, bool readSize)
{
    unsigned int nr = 0;

    if (readSize)
    {
        RFileIO::readBinary(inFile,nr);
        rVector.resize(nr);
    }
    else
    {
        nr = rVector.getNRows();
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::readBinary(inFile,rVector[i]);
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RRVector &rVector, bool writeSize, bool addNewLine)
{
    unsigned int nr = rVector.getNRows();
    if (writeSize)
    {
        RFileIO::writeAscii(outFile,nr,addNewLine);
        if (!addNewLine)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::writeAscii(outFile,rVector[i], false);
        if (i+1 < nr)
        {
            RFileIO::writeAscii(outFile,' ', false);
        }
    }
    if (addNewLine)
    {
        RFileIO::writeNewLineAscii(outFile);
    }
} /* RFileIO::writeAscii */


void RFileIO::writeAscii(RFile &outFile, const RRVector &rVector, bool writeSize, bool addNewLine)
{
    unsigned int nr = rVector.getNRows();
    if (writeSize)
    {
        RFileIO::writeAscii(outFile,nr,addNewLine);
        if (!addNewLine)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::writeAscii(outFile,rVector[i], false);
        if (i+1 < nr)
        {
            RFileIO::writeAscii(outFile,' ', false);
        }
    }
    if (addNewLine)
    {
        RFileIO::writeNewLineAscii(outFile);
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RRVector &rVector, bool writeSize)
{
    unsigned int nr = rVector.getNRows();
    if (writeSize)
    {
        RFileIO::writeBinary(outFile,nr);
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::writeBinary(outFile,rVector[i]);
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RR3Vector                                                         *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RR3Vector &rVector)
{
    RFileIO::readAscii(inFile,rVector[0]);
    RFileIO::readAscii(inFile,rVector[1]);
    RFileIO::readAscii(inFile,rVector[2]);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RR3Vector &rVector)
{
    RFileIO::readBinary(inFile,rVector[0]);
    RFileIO::readBinary(inFile,rVector[1]);
    RFileIO::readBinary(inFile,rVector[2]);
} /* RFileIO::readAscii */


void RFileIO::writeAscii(RSaveFile &outFile, const RR3Vector &rVector, bool addNewLine)
{
    RFileIO::writeAscii(outFile,rVector[0],addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile," ",false);
    }
    RFileIO::writeAscii(outFile,rVector[1],addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile," ",false);
    }
    RFileIO::writeAscii(outFile,rVector[2],addNewLine);
} /* RFileIO::readAscii */


void RFileIO::writeAscii(RFile &outFile, const RR3Vector &rVector, bool addNewLine)
{
    RFileIO::writeAscii(outFile,rVector[0],addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile," ",false);
    }
    RFileIO::writeAscii(outFile,rVector[1],addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile," ",false);
    }
    RFileIO::writeAscii(outFile,rVector[2],addNewLine);
} /* RFileIO::readAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RR3Vector &rVector)
{
    RFileIO::writeBinary(outFile,rVector[0]);
    RFileIO::writeBinary(outFile,rVector[1]);
    RFileIO::writeBinary(outFile,rVector[2]);
} /* RFileIO::readAscii */


/*********************************************************************
 *  RIMatrix                                                         *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RIMatrix &iMatrix, bool readSize)
{
    unsigned int nr = 0;

    if (readSize)
    {
        unsigned int nc = 0;
        RFileIO::readAscii(inFile,nr);
        RFileIO::readAscii(inFile,nc);
        iMatrix.resize(nr,nc);
    }
    else
    {
        nr = iMatrix.getNRows();
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::readAscii(inFile,iMatrix[i],false);
    }
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RIMatrix &iMatrix, bool readSize)
{
    unsigned int nr = 0;

    if (readSize)
    {
        unsigned int nc = 0;
        RFileIO::readBinary(inFile,nr);
        RFileIO::readBinary(inFile,nc);
        iMatrix.resize(nr,nc);
    }
    else
    {
        nr = iMatrix.getNRows();
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::readBinary(inFile,iMatrix[i]);
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RIMatrix &iMatrix, bool writeSize, bool addNewLine)
{
    unsigned int nr = iMatrix.getNRows();
    if (writeSize)
    {
        unsigned int nc = iMatrix.getNColumns();
        RFileIO::writeAscii(outFile,nr,false);
        RFileIO::writeAscii(outFile,' ',false);
        if (addNewLine)
        {
            RFileIO::writeAscii(outFile,nc,true);
        }
        else
        {
            RFileIO::writeAscii(outFile,nc,false);
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::writeAscii(outFile,iMatrix[i],writeSize,addNewLine);
        if (i+1 < nr && !addNewLine)
        {
            RFileIO::writeAscii(outFile,' ');
        }
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RIMatrix &iMatrix, bool writeSize)
{
    unsigned int nr = iMatrix.getNRows();
    if (writeSize)
    {
        RFileIO::writeBinary(outFile,nr);
        RFileIO::writeBinary(outFile,iMatrix.getNColumns());
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::writeBinary(outFile,iMatrix[i],writeSize);
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RRMatrix                                                         *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RRMatrix &rMatrix, bool readSize)
{

    unsigned int nr = 0;

    if (readSize)
    {
        unsigned int nc = 0;
        RFileIO::readAscii(inFile,nr);
        RFileIO::readAscii(inFile,nc);
        rMatrix.resize(nr,nc);
    }
    else
    {
        nr = rMatrix.getNRows();
    }
    for (unsigned int i=0;i<nr;i++)
    {
        try {
        RFileIO::readAscii(inFile,rMatrix[i],false);
        } catch (const RError &error)
        {
            RLogger::warning("i = %u\n",i);
            throw error;
        }
    }
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RRMatrix &rMatrix, bool readSize)
{
    unsigned int nr = 0;

    if (readSize)
    {
        unsigned int nc = 0;
        RFileIO::readBinary(inFile,nr);
        RFileIO::readBinary(inFile,nc);
        rMatrix.resize(nr,nc);
    }
    else
    {
        nr = rMatrix.getNRows();
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::readBinary(inFile,rMatrix[i],false);
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RRMatrix &rMatrix, bool writeSize, bool addNewLine)
{
    unsigned int nr = rMatrix.getNRows();
    if (writeSize)
    {
        unsigned int nc = rMatrix.getNColumns();
        RFileIO::writeAscii(outFile,nr,false);
        RFileIO::writeAscii(outFile,' ',false);
        if (addNewLine)
        {
            RFileIO::writeAscii(outFile,nc,true);
        }
        else
        {
            RFileIO::writeAscii(outFile,nc,false);
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::writeAscii(outFile,rMatrix[i],false,addNewLine);
        if (i+1 < nr && !addNewLine)
        {
            RFileIO::writeAscii(outFile,' ');
        }
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile  &outFile, const RRMatrix &rMatrix, bool writeSize)
{
    unsigned int nr = rMatrix.getNRows();
    if (writeSize)
    {
        RFileIO::writeBinary(outFile,nr);
        RFileIO::writeBinary(outFile,rMatrix.getNColumns());
    }
    for (unsigned int i=0;i<nr;i++)
    {
        RFileIO::writeBinary(outFile,rMatrix[i],false);
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RVersion                                                         *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RVersion &version)
{
    QString versionStr;
    RFileIO::readAscii(inFile,versionStr);
    version = RVersion(versionStr);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RVersion &version)
{
    unsigned int major = 0;
    unsigned int minor = 0;
    unsigned int release = 0;
    unsigned int releaseType = RVersion::Release;
    RFileIO::readBinary(inFile,major);
    RFileIO::readBinary(inFile,minor);
    RFileIO::readBinary(inFile,release);
    RFileIO::readBinary(inFile,releaseType);
    version = RVersion(major,minor,release,RVersion::ReleaseType(releaseType));
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RVersion &version, bool addNewLine)
{
    RFileIO::writeAscii(outFile,version.toString(),addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile  &outFile, const RVersion &version)
{
    RFileIO::writeBinary(outFile,version.getMajor());
    RFileIO::writeBinary(outFile,version.getMinor());
    RFileIO::writeBinary(outFile,version.getRelease());
    RFileIO::writeBinary(outFile,(unsigned int)(version.getReleaseType()));
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RValueVector                                                     *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RValueVector &valueVector)
{
    unsigned int n;
    QString name;
    QString units;

    RFileIO::readAscii(inFile,name);
    RFileIO::readAscii(inFile,units);
    RFileIO::readAscii(inFile,n);

    valueVector.setName(name);
    valueVector.setUnits(units);
    valueVector.resize(n);

    for (unsigned int i=0;i<n;i++)
    {
        RFileIO::readAscii(inFile,valueVector[i]);
    }
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RValueVector &valueVector)
{
    unsigned int n;
    QString name;
    QString units;

    RFileIO::readBinary(inFile,name);
    RFileIO::readBinary(inFile,units);
    RFileIO::readBinary(inFile,n);

    valueVector.setName(name);
    valueVector.setUnits(units);
    valueVector.resize(n);

    for (unsigned int i=0;i<n;i++)
    {
        RFileIO::readBinary(inFile,valueVector[i]);
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RValueVector &valueVector, bool addNewLine)
{
    unsigned int n = valueVector.size();

    RFileIO::writeAscii(outFile,"\"" + valueVector.getName() + "\"",addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile, "\"" + valueVector.getUnits() + "\"",addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,n,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }

    for (unsigned int i=0;i<n;i++)
    {
        RFileIO::writeAscii(outFile,valueVector[i],addNewLine);
        if (!addNewLine && i+1<n)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RValueVector &valueVector)
{
    unsigned int n = valueVector.size();

    RFileIO::writeBinary(outFile,valueVector.getName());
    RFileIO::writeBinary(outFile,valueVector.getUnits());
    RFileIO::writeBinary(outFile,n);

    for (unsigned int i=0;i<n;i++)
    {
        RFileIO::writeBinary(outFile,valueVector[i]);
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RPlane                                                           *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RPlane &plane)
{
    RR3Vector position;
    RR3Vector normal;

    RFileIO::readAscii(inFile,position);
    RFileIO::readAscii(inFile,normal);

    plane.setPosition(position);
    plane.setNormal(normal);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RPlane &plane)
{
    RR3Vector position;
    RR3Vector normal;

    RFileIO::readBinary(inFile,position);
    RFileIO::readBinary(inFile,normal);

    plane.setPosition(position);
    plane.setNormal(normal);
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RPlane &plane, bool addNewLine)
{
    RFileIO::writeAscii(outFile,plane.getPosition(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,plane.getNormal(),addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RPlane &plane)
{
    RFileIO::writeBinary(outFile,plane.getPosition());
    RFileIO::writeBinary(outFile,plane.getNormal());
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RValueTable                                                      *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RValueTable &valueTable)
{
    unsigned int n;
    QString keyName;
    QString keyUnits;
    QString valueName;
    QString valueUnits;
    double key;
    double value;

    RFileIO::readAscii(inFile,keyName);
    RFileIO::readAscii(inFile,keyUnits);
    RFileIO::readAscii(inFile,valueName);
    RFileIO::readAscii(inFile,valueUnits);
    RFileIO::readAscii(inFile,n);

    valueTable.clear();

    valueTable.setKeyName(keyName);
    valueTable.setKeyName(keyUnits);
    valueTable.setValueName(valueName);
    valueTable.setValueUnits(valueUnits);

    for (unsigned int i=0;i<n;i++)
    {
        RFileIO::readAscii(inFile,key);
        RFileIO::readAscii(inFile,value);
        valueTable.add(key,value);
    }
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RValueTable &valueTable)
{
    unsigned int n;
    QString keyName;
    QString keyUnits;
    QString valueName;
    QString valueUnits;
    double key;
    double value;

    RFileIO::readBinary(inFile,keyName);
    RFileIO::readBinary(inFile,keyUnits);
    RFileIO::readBinary(inFile,valueName);
    RFileIO::readBinary(inFile,valueUnits);
    RFileIO::readBinary(inFile,n);

    valueTable.clear();

    valueTable.setKeyName(keyName);
    valueTable.setKeyUnits(keyUnits);
    valueTable.setValueName(valueName);
    valueTable.setValueUnits(valueUnits);

    for (unsigned int i=0;i<n;i++)
    {
        RFileIO::readBinary(inFile,key);
        RFileIO::readBinary(inFile,value);
        valueTable.add(key,value);
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RValueTable &valueTable, bool addNewLine)
{
    unsigned int n = valueTable.size();

    RFileIO::writeAscii(outFile,"\"" + valueTable.getKeyName() + "\"",addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,"\"" + valueTable.getKeyUnits() + "\"",addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,"\"" + valueTable.getValueName() + "\"",addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,"\"" + valueTable.getValueUnits() + "\"",addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,n,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }

    for (unsigned int i=0;i<n;i++)
    {
        RFileIO::writeAscii(outFile,valueTable.getKey(i),addNewLine);
        if (!addNewLine)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
        RFileIO::writeAscii(outFile,valueTable.getValue(i),addNewLine);
        if (!addNewLine && i+1 < n)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RValueTable &valueTable)
{
    unsigned int n = valueTable.size();

    RFileIO::writeBinary(outFile,valueTable.getKeyName());
    RFileIO::writeBinary(outFile,valueTable.getKeyUnits());
    RFileIO::writeBinary(outFile,valueTable.getValueName());
    RFileIO::writeBinary(outFile,valueTable.getValueUnits());
    RFileIO::writeBinary(outFile,n);

    for (unsigned int i=0;i<n;i++)
    {
        RFileIO::writeBinary(outFile,valueTable.getKey(i));
        RFileIO::writeBinary(outFile,valueTable.getValue(i));
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  QColor                                                           *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, QColor &color)
{
    int r,g,b,a;
    RFileIO::readAscii(inFile,r);
    RFileIO::readAscii(inFile,g);
    RFileIO::readAscii(inFile,b);
    RFileIO::readAscii(inFile,a);
    color = QColor(r,g,b,a);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, QColor &color)
{
    int r,g,b,a;
    RFileIO::readBinary(inFile,r);
    RFileIO::readBinary(inFile,g);
    RFileIO::readBinary(inFile,b);
    RFileIO::readBinary(inFile,a);
    color = QColor(r,g,b,a);
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const QColor &color, bool addNewLine)
{
    RFileIO::writeAscii(outFile,color.red(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,color.green(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,color.blue(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,color.alpha(),addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const QColor &color)
{
    RFileIO::writeBinary(outFile,color.red());
    RFileIO::writeBinary(outFile,color.green());
    RFileIO::writeBinary(outFile,color.blue());
    RFileIO::writeBinary(outFile,color.alpha());
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RLight                                                           *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RGLLight &light)
{
    RFileIO::readAscii(inFile,light.enabled);
    RFileIO::readAscii(inFile,light.lightNum);
    RFileIO::readAscii(inFile,light.ambient);
    RFileIO::readAscii(inFile,light.diffuse);
    RFileIO::readAscii(inFile,light.specular);
    RFileIO::readAscii(inFile,light.direction);
    RFileIO::readAscii(inFile,light.position);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RGLLight &light)
{
    RFileIO::readBinary(inFile,light.enabled);
    RFileIO::readBinary(inFile,light.lightNum);
    RFileIO::readBinary(inFile,light.ambient);
    RFileIO::readBinary(inFile,light.diffuse);
    RFileIO::readBinary(inFile,light.specular);
    RFileIO::readBinary(inFile,light.direction);
    RFileIO::readBinary(inFile,light.position);
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RGLLight &light, bool addNewLine)
{
    RFileIO::writeAscii(outFile,light.enabled,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,light.lightNum,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,light.ambient,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,light.diffuse,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,light.specular,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,light.direction,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,light.position,addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RGLLight &light)
{
    RFileIO::writeBinary(outFile,light.enabled);
    RFileIO::writeBinary(outFile,light.lightNum);
    RFileIO::writeBinary(outFile,light.ambient);
    RFileIO::writeBinary(outFile,light.diffuse);
    RFileIO::writeBinary(outFile,light.specular);
    RFileIO::writeBinary(outFile,light.direction);
    RFileIO::writeBinary(outFile,light.position);
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RGLDisplayProperties                                             *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RGLDisplayProperties &displayProperties)
{
    unsigned int nLights;
    RFileIO::readAscii(inFile,displayProperties.drawGlobalAxis);
    RFileIO::readAscii(inFile,displayProperties.drawLocalAxis);
    RFileIO::readAscii(inFile,displayProperties.showModelEdges);
    RFileIO::readAscii(inFile,displayProperties.showModelDimensions);
    RFileIO::readAscii(inFile,displayProperties.showErrors);
    RFileIO::readAscii(inFile,displayProperties.bgColor);
    RFileIO::readAscii(inFile,displayProperties.bgGradient);
    RFileIO::readAscii(inFile,nLights);
    displayProperties.setNLights(nLights);
    for (unsigned int i=0;i<nLights;i++)
    {
        RFileIO::readAscii(inFile,displayProperties.lights[i]);
    }
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RGLDisplayProperties &displayProperties)
{
    unsigned int nLights;
    RFileIO::readBinary(inFile,displayProperties.drawGlobalAxis);
    RFileIO::readBinary(inFile,displayProperties.drawLocalAxis);
    RFileIO::readBinary(inFile,displayProperties.showModelEdges);
    RFileIO::readBinary(inFile,displayProperties.showModelDimensions);
    RFileIO::readBinary(inFile,displayProperties.showErrors);
    RFileIO::readBinary(inFile,displayProperties.bgColor);
    RFileIO::readBinary(inFile,displayProperties.bgGradient);
    RFileIO::readBinary(inFile,nLights);
    displayProperties.setNLights(nLights);
    for (unsigned int i=0;i<displayProperties.getNLights();i++)
    {
        RFileIO::readBinary(inFile,displayProperties.lights[i]);
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RGLDisplayProperties &displayProperties, bool addNewLine)
{
    RFileIO::writeAscii(outFile,displayProperties.drawGlobalAxis,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,displayProperties.drawLocalAxis,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,displayProperties.showModelEdges,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,displayProperties.showModelDimensions,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,displayProperties.showErrors,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,displayProperties.bgColor,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,displayProperties.bgGradient,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,displayProperties.getNLights(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    for (unsigned int i=0;i<displayProperties.getNLights();i++)
    {
        RFileIO::writeAscii(outFile,displayProperties.lights[i]);
        if (!addNewLine && i+1 < displayProperties.getNLights())
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RGLDisplayProperties &displayProperties)
{
    RFileIO::writeBinary(outFile,displayProperties.drawGlobalAxis);
    RFileIO::writeBinary(outFile,displayProperties.drawLocalAxis);
    RFileIO::writeBinary(outFile,displayProperties.showModelEdges);
    RFileIO::writeBinary(outFile,displayProperties.showModelDimensions);
    RFileIO::writeBinary(outFile,displayProperties.showErrors);
    RFileIO::writeBinary(outFile,displayProperties.bgColor);
    RFileIO::writeBinary(outFile,displayProperties.bgGradient);
    RFileIO::writeBinary(outFile,displayProperties.getNLights());
    for (unsigned int i=0;i<displayProperties.getNLights();i++)
    {
        RFileIO::writeBinary(outFile,displayProperties.lights[i]);
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RModelData                                                       *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RModelData &modelData)
{
    RFileIO::readAscii(inFile,modelData.selected);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RModelData &modelData)
{
    RFileIO::readBinary(inFile,modelData.selected);
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RModelData &modelData, bool addNewLine)
{
    RFileIO::writeAscii(outFile,modelData.selected,addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RModelData &modelData)
{
    RFileIO::writeBinary(outFile,modelData.selected);
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RVariableType                                                    *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RVariableType &variableType)
{
    QString variableId;
    RFileIO::readAscii(inFile,variableId);
    variableType = RVariable::getTypeFromId(variableId);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RVariableType &variableType)
{
    QString variableId;
    RFileIO::readBinary(inFile,variableId);
    variableType = RVariable::getTypeFromId(variableId);
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RVariableType &variableType, bool addNewLine)
{
    RFileIO::writeAscii(outFile,RVariable::getId(variableType),addNewLine);
} /* RFileIO::writeAscii */

void RFileIO::writeAscii(RFile &outFile, const RVariableType &variableType, bool addNewLine)
{
    RFileIO::writeAscii(outFile,RVariable::getId(variableType),addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RVariableType &variableType)
{
    RFileIO::writeBinary(outFile,RVariable::getId(variableType));
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RVariableApplyType                                               *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RVariableApplyType &variableApplyType)
{
    int iValue;
    inFile.getTextStream() >> iValue;
    if (inFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to read RVariableApplyType value.");
    }
    variableApplyType = RVariableApplyType(iValue);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RVariableApplyType &variableApplyType)
{
    inFile.read((char*)&variableApplyType,sizeof(RVariableApplyType));
    if (inFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read RVariableApplyType value.");
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RVariableApplyType &variableApplyType, bool addNewLine)
{
    if (!addNewLine)
    {
        outFile.getTextStream() << int(variableApplyType);
    }
    else
    {
        outFile.getTextStream() << int(variableApplyType) << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write RVariableApplyType value.");
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RVariableApplyType &variableApplyType)
{
    outFile.write((char*)&variableApplyType,sizeof(RVariableApplyType));
    if (outFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write RVariableApplyType value.");
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RVariableData                                                    *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RVariableData &variableData)
{
    RFileIO::readAscii(inFile,variableData.minDisplayValue);
    RFileIO::readAscii(inFile,variableData.maxDisplayValue);
    RFileIO::readAscii(inFile,variableData.displayValueRange);
    RFileIO::readAscii(inFile,variableData.valueRangeName);
    RFileIO::readAscii(inFile,variableData.scale);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RVariableData &variableData)
{
    RFileIO::readBinary(inFile,variableData.minDisplayValue);
    RFileIO::readBinary(inFile,variableData.maxDisplayValue);
    RFileIO::readBinary(inFile,variableData.displayValueRange);
    RFileIO::readBinary(inFile,variableData.valueRangeName);
    RFileIO::readBinary(inFile,variableData.scale);
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RVariableData &variableData, bool addNewLine)
{
    RFileIO::writeAscii(outFile,variableData.minDisplayValue,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,variableData.maxDisplayValue,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,variableData.displayValueRange,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,variableData.valueRangeName,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,variableData.scale,addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RVariableData &variableData)
{
    RFileIO::writeBinary(outFile,variableData.minDisplayValue);
    RFileIO::writeBinary(outFile,variableData.maxDisplayValue);
    RFileIO::writeBinary(outFile,variableData.displayValueRange);
    RFileIO::writeBinary(outFile,variableData.valueRangeName);
    RFileIO::writeBinary(outFile,variableData.scale);
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RVariable                                                        *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RVariable &variable)
{
    RFileIO::readAscii(inFile,variable.type);
    RFileIO::readAscii(inFile,variable.applyType);
    RFileIO::readAscii(inFile,variable.name);
    RFileIO::readAscii(inFile,variable.units);
    unsigned int nValues = 0;
    RFileIO::readAscii(inFile,nValues);
    variable.values.resize(nValues);
    for (unsigned int i=0;i<variable.values.size();i++)
    {
        RFileIO::readAscii(inFile,variable.values[i]);
    }
    RFileIO::readAscii(inFile,variable.variableData);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RVariable &variable)
{
    RFileIO::readBinary(inFile,variable.type);
    RFileIO::readBinary(inFile,variable.applyType);
    RFileIO::readBinary(inFile,variable.name);
    RFileIO::readBinary(inFile,variable.units);
    unsigned int nValues = 0;
    RFileIO::readBinary(inFile,nValues);
    variable.values.resize(nValues);
    for (unsigned int i=0;i<variable.values.size();i++)
    {
        RFileIO::readBinary(inFile,variable.values[i]);
    }
    RFileIO::readBinary(inFile,variable.variableData);
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RVariable &variable, bool addNewLine)
{
    RFileIO::writeAscii(outFile,variable.type,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,variable.applyType,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,variable.name,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,variable.units,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,(unsigned int)variable.values.size(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    for (unsigned int i=0;i<variable.values.size();i++)
    {
        RFileIO::writeAscii(outFile,variable.values[i],addNewLine);
        if (!addNewLine && i+1 < variable.values.size())
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
    RFileIO::writeAscii(outFile,variable.variableData,addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RVariable &variable)
{
    RFileIO::writeBinary(outFile,variable.type);
    RFileIO::writeBinary(outFile,variable.applyType);
    RFileIO::writeBinary(outFile,variable.name);
    RFileIO::writeBinary(outFile,variable.units);
    RFileIO::writeBinary(outFile,(unsigned int)variable.values.size());
    for (unsigned int i=0;i<variable.values.size();i++)
    {
        RFileIO::writeBinary(outFile,variable.values[i]);
    }
    RFileIO::writeBinary(outFile,variable.variableData);
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RBoundaryConditionType                                           *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RBoundaryConditionType &bcType)
{
    QString propertyId;
    RFileIO::readAscii(inFile,propertyId);
    bcType = RBoundaryCondition::getTypeFromId(propertyId);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RBoundaryConditionType &bcType)
{
    QString propertyId;
    RFileIO::readBinary(inFile,propertyId);
    bcType = RBoundaryCondition::getTypeFromId(propertyId);
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RBoundaryConditionType &bcType, bool addNewLine)
{
    RFileIO::writeAscii(outFile,RBoundaryCondition::getId(bcType),addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RBoundaryConditionType &bcType)
{
    RFileIO::writeBinary(outFile,RBoundaryCondition::getId(bcType));
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RInitialConditionType                                            *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RInitialConditionType &icType)
{
    QString propertyId;
    RFileIO::readAscii(inFile,propertyId);
    icType = RInitialCondition::getTypeFromId(propertyId);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RInitialConditionType &icType)
{
    QString propertyId;
    RFileIO::readBinary(inFile,propertyId);
    icType = RInitialCondition::getTypeFromId(propertyId);
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RInitialConditionType &icType, bool addNewLine)
{
    RFileIO::writeAscii(outFile,RInitialCondition::getId(icType),addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RInitialConditionType &icType)
{
    RFileIO::writeBinary(outFile,RInitialCondition::getId(icType));
} /* RFileIO::writeBinary */


/*********************************************************************
 *  REnvironmentConditionType                                        *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, REnvironmentConditionType &ecType)
{
    QString propertyId;
    RFileIO::readAscii(inFile,propertyId);
    ecType = REnvironmentCondition::getTypeFromId(propertyId);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, REnvironmentConditionType &ecType)
{
    QString propertyId;
    RFileIO::readBinary(inFile,propertyId);
    ecType = REnvironmentCondition::getTypeFromId(propertyId);
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const REnvironmentConditionType &ecType, bool addNewLine)
{
    RFileIO::writeAscii(outFile,REnvironmentCondition::getId(ecType),addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const REnvironmentConditionType &ecType)
{
    RFileIO::writeBinary(outFile,REnvironmentCondition::getId(ecType));
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RConditionComponent                                              *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RConditionComponent &conditionComponent)
{
    RFileIO::readAscii(inFile,static_cast<RValueTable&>(conditionComponent));
    RFileIO::readAscii(inFile,conditionComponent.keyType);
    RFileIO::readAscii(inFile,conditionComponent.type);
    RFileIO::readAscii(inFile,conditionComponent.enabled);
}

void RFileIO::readBinary(RFile &inFile, RConditionComponent &conditionComponent)
{
    RFileIO::readBinary(inFile,static_cast<RValueTable&>(conditionComponent));
    RFileIO::readBinary(inFile,conditionComponent.keyType);
    RFileIO::readBinary(inFile,conditionComponent.type);
    RFileIO::readBinary(inFile,conditionComponent.enabled);
}

void RFileIO::writeAscii(RSaveFile &outFile, const RConditionComponent &conditionComponent, bool addNewLine)
{
    RFileIO::writeAscii(outFile,static_cast<const RValueTable&>(conditionComponent),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,conditionComponent.keyType,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,conditionComponent.type,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,conditionComponent.enabled,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RConditionComponent &conditionComponent)
{
    RFileIO::writeBinary(outFile,static_cast<const RValueTable&>(conditionComponent));
    RFileIO::writeBinary(outFile,conditionComponent.keyType);
    RFileIO::writeBinary(outFile,conditionComponent.type);
    RFileIO::writeBinary(outFile,conditionComponent.enabled);
}


/*********************************************************************
 *  RCondition                                                       *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RCondition &condition)
{
    RFileIO::readAscii(inFile,condition.name);
    unsigned int nComponents;
    RFileIO::readAscii(inFile,nComponents);
    condition.components.resize(nComponents);
    for (unsigned int i=0;i<nComponents;i++)
    {
        RFileIO::readAscii(inFile,condition.components[i]);
    }
    RFileIO::readAscii(inFile,condition.applyOnPoint);
    RFileIO::readAscii(inFile,condition.applyOnLine);
    RFileIO::readAscii(inFile,condition.applyOnSurface);
    RFileIO::readAscii(inFile,condition.applyOnVolume);
    RFileIO::readAscii(inFile,condition.problemTypeMask);
}


void RFileIO::readBinary(RFile &inFile, RCondition &condition)
{
    RFileIO::readBinary(inFile,condition.name);
    unsigned int nComponents;
    RFileIO::readBinary(inFile,nComponents);
    condition.components.resize(nComponents);
    for (unsigned int i=0;i<nComponents;i++)
    {
        RFileIO::readBinary(inFile,condition.components[i]);
    }
    RFileIO::readBinary(inFile,condition.applyOnPoint);
    RFileIO::readBinary(inFile,condition.applyOnLine);
    RFileIO::readBinary(inFile,condition.applyOnSurface);
    RFileIO::readBinary(inFile,condition.applyOnVolume);
    RFileIO::readBinary(inFile,condition.problemTypeMask);
}


void RFileIO::writeAscii(RSaveFile &outFile, const RCondition &condition, bool addNewLine)
{
    RFileIO::writeAscii(outFile,"\"" + condition.name + "\"",addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,condition.size(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    for (unsigned int i=0;i<condition.components.size();i++)
    {
        RFileIO::writeAscii(outFile,condition.components[i],addNewLine);
        if (!addNewLine)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
    RFileIO::writeAscii(outFile,condition.applyOnPoint,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,condition.applyOnLine,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,condition.applyOnSurface,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,condition.applyOnVolume,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,condition.problemTypeMask,addNewLine);
}


void RFileIO::writeBinary(RSaveFile &outFile, const RCondition &condition)
{
    RFileIO::writeBinary(outFile,condition.name);
    RFileIO::writeBinary(outFile,condition.size());
    for (unsigned int i=0;i<condition.components.size();i++)
    {
        RFileIO::writeBinary(outFile,condition.components[i]);
    }
    RFileIO::writeBinary(outFile,condition.applyOnPoint);
    RFileIO::writeBinary(outFile,condition.applyOnLine);
    RFileIO::writeBinary(outFile,condition.applyOnSurface);
    RFileIO::writeBinary(outFile,condition.applyOnVolume);
    RFileIO::writeBinary(outFile,condition.problemTypeMask);
}


/*********************************************************************
 *  RBoundaryCondition                                               *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RBoundaryCondition &boundaryCondition)
{
    RFileIO::readAscii(inFile,static_cast<RCondition&>(boundaryCondition));
    RFileIO::readAscii(inFile,boundaryCondition.type);
    RFileIO::readAscii(inFile,boundaryCondition.optional);
    RFileIO::readAscii(inFile,boundaryCondition.isExplicit);
    RFileIO::readAscii(inFile,boundaryCondition.hasLocalDirection);
    RFileIO::readAscii(inFile,boundaryCondition.direction[0]);
    RFileIO::readAscii(inFile,boundaryCondition.direction[1]);
    RFileIO::readAscii(inFile,boundaryCondition.direction[2]);
}


void RFileIO::readBinary(RFile &inFile, RBoundaryCondition &boundaryCondition)
{
    RFileIO::readBinary(inFile,static_cast<RCondition&>(boundaryCondition));
    RFileIO::readBinary(inFile,boundaryCondition.type);
    RFileIO::readBinary(inFile,boundaryCondition.optional);
    RFileIO::readBinary(inFile,boundaryCondition.isExplicit);
    RFileIO::readBinary(inFile,boundaryCondition.hasLocalDirection);
    RFileIO::readBinary(inFile,boundaryCondition.direction[0]);
    RFileIO::readBinary(inFile,boundaryCondition.direction[1]);
    RFileIO::readBinary(inFile,boundaryCondition.direction[2]);
}


void RFileIO::writeAscii(RSaveFile &outFile, const RBoundaryCondition &boundaryCondition, bool addNewLine)
{
    RFileIO::writeAscii(outFile,static_cast<const RCondition&>(boundaryCondition),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,boundaryCondition.type,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,boundaryCondition.optional,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,boundaryCondition.isExplicit,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,boundaryCondition.hasLocalDirection,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,boundaryCondition.direction[0],addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,boundaryCondition.direction[1],addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,boundaryCondition.direction[2],addNewLine);
}


void RFileIO::writeBinary(RSaveFile &outFile, const RBoundaryCondition &boundaryCondition)
{
    RFileIO::writeBinary(outFile,static_cast<const RCondition&>(boundaryCondition));
    RFileIO::writeBinary(outFile,boundaryCondition.type);
    RFileIO::writeBinary(outFile,boundaryCondition.optional);
    RFileIO::writeBinary(outFile,boundaryCondition.isExplicit);
    RFileIO::writeBinary(outFile,boundaryCondition.hasLocalDirection);
    RFileIO::writeBinary(outFile,boundaryCondition.direction[0]);
    RFileIO::writeBinary(outFile,boundaryCondition.direction[1]);
    RFileIO::writeBinary(outFile,boundaryCondition.direction[2]);
}


/*********************************************************************
 *  RInitialCondition                                                *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RInitialCondition &initialCondition)
{
    RFileIO::readAscii(inFile,static_cast<RCondition&>(initialCondition));
    RFileIO::readAscii(inFile,initialCondition.type);
}


void RFileIO::readBinary(RFile &inFile, RInitialCondition &initialCondition)
{
    RFileIO::readBinary(inFile,static_cast<RCondition&>(initialCondition));
    RFileIO::readBinary(inFile,initialCondition.type);
}


void RFileIO::writeAscii(RSaveFile &outFile, const RInitialCondition &initialCondition, bool addNewLine)
{
    RFileIO::writeAscii(outFile,static_cast<const RCondition&>(initialCondition),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,initialCondition.type,addNewLine);
}


void RFileIO::writeBinary(RSaveFile &outFile, const RInitialCondition &initialCondition)
{
    RFileIO::writeBinary(outFile,static_cast<const RCondition&>(initialCondition));
    RFileIO::writeBinary(outFile,initialCondition.type);
}


/*********************************************************************
 *  REnvironmentCondition                                            *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, REnvironmentCondition &environmentCondition)
{
    RFileIO::readAscii(inFile,static_cast<RCondition&>(environmentCondition));
    RFileIO::readAscii(inFile,environmentCondition.type);
}


void RFileIO::readBinary(RFile &inFile, REnvironmentCondition &environmentCondition)
{
    RFileIO::readBinary(inFile,static_cast<RCondition&>(environmentCondition));
    RFileIO::readBinary(inFile,environmentCondition.type);
}


void RFileIO::writeAscii(RSaveFile &outFile, const REnvironmentCondition &environmentCondition, bool addNewLine)
{
    RFileIO::writeAscii(outFile,static_cast<const RCondition&>(environmentCondition),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,environmentCondition.type,addNewLine);
}


void RFileIO::writeBinary(RSaveFile &outFile, const REnvironmentCondition &environmentCondition)
{
    RFileIO::writeBinary(outFile,static_cast<const RCondition&>(environmentCondition));
    RFileIO::writeBinary(outFile,environmentCondition.type);
}


/*********************************************************************
 *  RMaterialPropertyType                                            *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RMaterialPropertyType &mpType)
{
    QString propertyId;
    RFileIO::readAscii(inFile,propertyId);
    mpType = RMaterialProperty::getTypeFromId(propertyId);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RMaterialPropertyType &mpType)
{
    QString propertyId;
    RFileIO::readBinary(inFile,propertyId);
    mpType = RMaterialProperty::getTypeFromId(propertyId);
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RMaterialPropertyType &mpType, bool addNewLine)
{
    RFileIO::writeAscii(outFile,RMaterialProperty::getId(mpType),addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RMaterialPropertyType &mpType)
{
    RFileIO::writeBinary(outFile,RMaterialProperty::getId(mpType));

} /* RFileIO::writeBinary */


/*********************************************************************
 *  RMaterialState                                                   *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RMaterialState &state)
{
    int iValue;
    inFile.getTextStream() >> iValue;
    if (inFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to read RMaterialState value.");
    }
    state = RMaterialState(iValue);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RMaterialState &state)
{
    inFile.read((char*)&state,sizeof(RMaterialState));
    if (inFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read RMaterialState value.");
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RMaterialState &state, bool addNewLine)
{
    if (!addNewLine)
    {
        outFile.getTextStream() << int(state);
    }
    else
    {
        outFile.getTextStream() << int(state) << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write RMaterialState value.");
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RMaterialState &state)
{
    outFile.write((char*)&state,sizeof(RMaterialState));
    if (outFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write RMaterialState value.");
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RMaterialProperty                                                *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RMaterialProperty &materialProperty)
{
    RFileIO::readAscii(inFile,static_cast<RValueTable&>(materialProperty));
    RFileIO::readAscii(inFile,materialProperty.keyType);
    RFileIO::readAscii(inFile,materialProperty.type);
    // HACK - always based on temperature
    materialProperty.keyType = R_VARIABLE_TEMPERATURE;
}

void RFileIO::readBinary(RFile &inFile, RMaterialProperty &materialProperty)
{
    RFileIO::readBinary(inFile,static_cast<RValueTable&>(materialProperty));
    RFileIO::readBinary(inFile,materialProperty.keyType);
    RFileIO::readBinary(inFile,materialProperty.type);
    // HACK - always based on temperature
    materialProperty.keyType = R_VARIABLE_TEMPERATURE;
}

void RFileIO::writeAscii(RSaveFile &outFile, const RMaterialProperty &materialProperty, bool addNewLine)
{
    RFileIO::writeAscii(outFile,static_cast<const RValueTable&>(materialProperty),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,materialProperty.keyType,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,materialProperty.type,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RMaterialProperty &materialProperty)
{
    RFileIO::writeBinary(outFile,static_cast<const RValueTable&>(materialProperty));
    RFileIO::writeBinary(outFile,materialProperty.keyType);
    RFileIO::writeBinary(outFile,materialProperty.type);
}


/*********************************************************************
 *  RMaterial                                                        *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RMaterial &material)
{
    RFileIO::readAscii(inFile,material.id);
    RFileIO::readAscii(inFile,material.name);
    RFileIO::readAscii(inFile,material.state);
    unsigned int nPorperties;
    RFileIO::readAscii(inFile,nPorperties);
    material.properties.resize(nPorperties);
    for (unsigned int i=0;i<nPorperties;i++)
    {
        RFileIO::readAscii(inFile,material.properties[i]);
    }
}


void RFileIO::readBinary(RFile &inFile, RMaterial &material)
{
    RFileIO::readBinary(inFile,material.id);
    RFileIO::readBinary(inFile,material.name);
    RFileIO::readBinary(inFile,material.state);
    unsigned int nPorperties;
    RFileIO::readBinary(inFile,nPorperties);
    material.properties.resize(nPorperties);
    for (unsigned int i=0;i<nPorperties;i++)
    {
        RFileIO::readBinary(inFile,material.properties[i]);
    }
}


void RFileIO::writeAscii(RSaveFile &outFile, const RMaterial &material, bool addNewLine)
{
    RFileIO::writeAscii(outFile,material.id,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,"\"" + material.name + "\"",addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,material.state,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,material.size(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    for (unsigned int i=0;i<material.properties.size();i++)
    {
        RFileIO::writeAscii(outFile,material.properties[i],addNewLine);
        if (!addNewLine && i+1 < material.properties.size())
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
}


void RFileIO::writeBinary(RSaveFile &outFile, const RMaterial &material)
{
    RFileIO::writeBinary(outFile,material.id);
    RFileIO::writeBinary(outFile,material.name);
    RFileIO::writeBinary(outFile,material.state);
    RFileIO::writeBinary(outFile,material.size());
    for (unsigned int i=0;i<material.properties.size();i++)
    {
        RFileIO::writeBinary(outFile,material.properties[i]);
    }
}


/*********************************************************************
 *  RNode                                                            *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RNode &node)
{
    RFileIO::readAscii(inFile,node.x);
    RFileIO::readAscii(inFile,node.y);
    RFileIO::readAscii(inFile,node.z);
}


void RFileIO::readBinary(RFile &inFile, RNode &node)
{
    RFileIO::readBinary(inFile,node.x);
    RFileIO::readBinary(inFile,node.y);
    RFileIO::readBinary(inFile,node.z);
}


void RFileIO::writeAscii(RSaveFile &outFile, const RNode &node, bool addNewLine)
{
    RFileIO::writeAscii(outFile,node.x,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,node.y,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,node.z,addNewLine);
}


void RFileIO::writeBinary(RSaveFile &outFile, const RNode &node)
{
    RFileIO::writeBinary(outFile,node.x);
    RFileIO::writeBinary(outFile,node.y);
    RFileIO::writeBinary(outFile,node.z);
}


/*********************************************************************
 *  RElementType                                                     *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RElementType &elementType)
{
    QString propertyId;
    RFileIO::readAscii(inFile,propertyId);
    elementType = RElement::getTypeFromId(propertyId);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RElementType &elementType)
{
    QString propertyId;
    RFileIO::readBinary(inFile,propertyId);
    elementType = RElement::getTypeFromId(propertyId);
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RElementType &elementType, bool addNewLine)
{
    RFileIO::writeAscii(outFile,RElement::getId(elementType),addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RElementType &elementType)
{
    RFileIO::writeBinary(outFile,RElement::getId(elementType));
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RElement                                                         *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RElement &element)
{
    RFileIO::readAscii(inFile,element.type);
    unsigned int nNodeIDs;
    RFileIO::readAscii(inFile,nNodeIDs);
    element.nodeIDs.resize(nNodeIDs);
    for (unsigned int i=0;i<nNodeIDs;i++)
    {
        RFileIO::readAscii(inFile,element.nodeIDs[i]);
    }
}


void RFileIO::readBinary(RFile &inFile, RElement &element)
{
    RFileIO::readBinary(inFile,element.type);
    unsigned int nNodeIDs;
    RFileIO::readBinary(inFile,nNodeIDs);
    element.nodeIDs.resize(nNodeIDs);
    for (unsigned int i=0;i<nNodeIDs;i++)
    {
        RFileIO::readBinary(inFile,element.nodeIDs[i]);
    }
}


void RFileIO::writeAscii(RSaveFile &outFile, const RElement &element, bool addNewLine)
{
    RFileIO::writeAscii(outFile,element.type,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,element.size(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    for (unsigned int i=0;i<element.nodeIDs.size();i++)
    {
        RFileIO::writeAscii(outFile,element.nodeIDs[i],addNewLine);
        if (!addNewLine && i+1 < element.nodeIDs.size())
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
}


void RFileIO::writeBinary(RSaveFile &outFile, const RElement &element)
{
    RFileIO::writeBinary(outFile,element.type);
    RFileIO::writeBinary(outFile,element.size());
    for (unsigned int i=0;i<element.nodeIDs.size();i++)
    {
        RFileIO::writeBinary(outFile,element.nodeIDs[i]);
    }
}


/*********************************************************************
 *  RElementGroupVariableDisplayType                                 *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, REntityGroupVariableDisplayType &elementGroupVariableDisplayType)
{
    int iValue;
    inFile.getTextStream() >> iValue;
    if (inFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to read RElementGroupVariableDisplayType value.");
    }
    elementGroupVariableDisplayType = REntityGroupVariableDisplayType(iValue);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, REntityGroupVariableDisplayType &elementGroupVariableDisplayType)
{
    inFile.read((char*)&elementGroupVariableDisplayType,sizeof(REntityGroupVariableDisplayType));
    if (inFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read RElementGroupVariableDisplayType value.");
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const REntityGroupVariableDisplayType &elementGroupVariableDisplayType, bool addNewLine)
{
    if (!addNewLine)
    {
        outFile.getTextStream() << int(elementGroupVariableDisplayType);
    }
    else
    {
        outFile.getTextStream() << int(elementGroupVariableDisplayType) << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write RElementGroupVariableDisplayType value.");
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const REntityGroupVariableDisplayType &elementGroupVariableDisplayType)
{
    outFile.write((char*)&elementGroupVariableDisplayType,sizeof(REntityGroupVariableDisplayType));
    if (outFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write RElementGroupVariableDisplayType value.");
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RElementGroupData                                                *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, REntityGroupData &elementGroupData)
{
    RFileIO::readAscii(inFile,elementGroupData.selected);
    RFileIO::readAscii(inFile,elementGroupData.visible);
    RFileIO::readAscii(inFile,elementGroupData.rgba[0]);
    RFileIO::readAscii(inFile,elementGroupData.rgba[1]);
    RFileIO::readAscii(inFile,elementGroupData.rgba[2]);
    RFileIO::readAscii(inFile,elementGroupData.rgba[3]);
    RFileIO::readAscii(inFile,elementGroupData.drawWire);
    RFileIO::readAscii(inFile,elementGroupData.drawEdges);
    RFileIO::readAscii(inFile,elementGroupData.drawNodes);
    RFileIO::readAscii(inFile,elementGroupData.drawElementNumbers);
    RFileIO::readAscii(inFile,elementGroupData.drawNodeNumbers);
    RFileIO::readAscii(inFile,elementGroupData.drawArrowHeads);
    RFileIO::readAscii(inFile,elementGroupData.drawEqualArrowLength);
    RFileIO::readAscii(inFile,elementGroupData.drawArrowFrom);
    RFileIO::readAscii(inFile,elementGroupData.colorByPatch);
    RFileIO::readAscii(inFile,elementGroupData.colorByViewFactor);
    elementGroupData.variableData.clear();
    unsigned int nVariableData = 0;
    RFileIO::readAscii(inFile,nVariableData);
    for (unsigned int i=0;i<nVariableData;i++)
    {
        RVariableType variableType;
        REntityGroupVariableDisplayType variableDisplayType;
        RFileIO::readAscii(inFile,variableType);
        RFileIO::readAscii(inFile,variableDisplayType);
        elementGroupData.variableData[variableType] = variableDisplayType;
    }
}


void RFileIO::readBinary(RFile &inFile, REntityGroupData &elementGroupData)
{
    RFileIO::readBinary(inFile,elementGroupData.selected);
    RFileIO::readBinary(inFile,elementGroupData.visible);
    RFileIO::readBinary(inFile,elementGroupData.rgba[0]);
    RFileIO::readBinary(inFile,elementGroupData.rgba[1]);
    RFileIO::readBinary(inFile,elementGroupData.rgba[2]);
    RFileIO::readBinary(inFile,elementGroupData.rgba[3]);
    RFileIO::readBinary(inFile,elementGroupData.drawWire);
    RFileIO::readBinary(inFile,elementGroupData.drawEdges);
    RFileIO::readBinary(inFile,elementGroupData.drawNodes);
    RFileIO::readBinary(inFile,elementGroupData.drawElementNumbers);
    RFileIO::readBinary(inFile,elementGroupData.drawNodeNumbers);
    RFileIO::readBinary(inFile,elementGroupData.drawArrowHeads);
    RFileIO::readBinary(inFile,elementGroupData.drawEqualArrowLength);
    RFileIO::readBinary(inFile,elementGroupData.drawArrowFrom);
    RFileIO::readBinary(inFile,elementGroupData.colorByPatch);
    RFileIO::readBinary(inFile,elementGroupData.colorByViewFactor);
    elementGroupData.variableData.clear();
    unsigned int nVariableData = 0;
    RFileIO::readBinary(inFile,nVariableData);
    for (unsigned int i=0;i<nVariableData;i++)
    {
        RVariableType variableType;
        REntityGroupVariableDisplayType variableDisplayType;
        RFileIO::readBinary(inFile,variableType);
        RFileIO::readBinary(inFile,variableDisplayType);
        elementGroupData.variableData[variableType] = variableDisplayType;
    }
}


void RFileIO::writeAscii(RSaveFile &outFile, const REntityGroupData &elementGroupData, bool addNewLine)
{
    RFileIO::writeAscii(outFile,elementGroupData.selected,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroupData.visible,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroupData.rgba[0],addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroupData.rgba[1],addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroupData.rgba[2],addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroupData.rgba[3],addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroupData.drawWire,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroupData.drawEdges,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroupData.drawNodes,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroupData.drawElementNumbers,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroupData.drawNodeNumbers,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroupData.drawArrowHeads,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroupData.drawEqualArrowLength,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroupData.drawArrowFrom,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroupData.colorByPatch,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroupData.colorByViewFactor,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,(unsigned int)elementGroupData.variableData.size(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    typedef std::map<RVariableType,REntityGroupVariableDisplayTypeMask>::const_iterator it_type;
    for (it_type iterator = elementGroupData.variableData.begin(); iterator != elementGroupData.variableData.end(); iterator++) {
        RFileIO::writeAscii(outFile,RVariableType(iterator->first),addNewLine);
        if (!addNewLine)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
        RFileIO::writeAscii(outFile,REntityGroupVariableDisplayTypeMask(iterator->second),addNewLine);
        if (!addNewLine)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
}


void RFileIO::writeBinary(RSaveFile &outFile, const REntityGroupData &elementGroupData)
{
    RFileIO::writeBinary(outFile,elementGroupData.selected);
    RFileIO::writeBinary(outFile,elementGroupData.visible);
    RFileIO::writeBinary(outFile,elementGroupData.rgba[0]);
    RFileIO::writeBinary(outFile,elementGroupData.rgba[1]);
    RFileIO::writeBinary(outFile,elementGroupData.rgba[2]);
    RFileIO::writeBinary(outFile,elementGroupData.rgba[3]);
    RFileIO::writeBinary(outFile,elementGroupData.drawWire);
    RFileIO::writeBinary(outFile,elementGroupData.drawEdges);
    RFileIO::writeBinary(outFile,elementGroupData.drawNodes);
    RFileIO::writeBinary(outFile,elementGroupData.drawElementNumbers);
    RFileIO::writeBinary(outFile,elementGroupData.drawNodeNumbers);
    RFileIO::writeBinary(outFile,elementGroupData.drawArrowHeads);
    RFileIO::writeBinary(outFile,elementGroupData.drawEqualArrowLength);
    RFileIO::writeBinary(outFile,elementGroupData.drawArrowFrom);
    RFileIO::writeBinary(outFile,elementGroupData.colorByPatch);
    RFileIO::writeBinary(outFile,elementGroupData.colorByViewFactor);
    RFileIO::writeBinary(outFile,(unsigned int)elementGroupData.variableData.size());
    typedef std::map<RVariableType,REntityGroupVariableDisplayTypeMask>::const_iterator it_type;
    for (it_type iterator = elementGroupData.variableData.begin(); iterator != elementGroupData.variableData.end(); iterator++) {
        RFileIO::writeBinary(outFile,RVariableType(iterator->first));
        RFileIO::writeBinary(outFile,REntityGroupVariableDisplayTypeMask(iterator->second));
    }
}


/*********************************************************************
 *  REntityGroup                                                     *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, REntityGroup &entityGroup)
{
    RFileIO::readAscii(inFile,entityGroup.name);
    RFileIO::readAscii(inFile,entityGroup.data);
}

void RFileIO::readBinary(RFile &inFile, REntityGroup &entityGroup)
{
    RFileIO::readBinary(inFile,entityGroup.name);
    RFileIO::readBinary(inFile,entityGroup.data);
}

void RFileIO::writeAscii(RSaveFile &outFile, const REntityGroup &entityGroup, bool addNewLine)
{
    RFileIO::writeAscii(outFile,"\"" + entityGroup.name + "\"",addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,entityGroup.data,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const REntityGroup &entityGroup)
{
    RFileIO::writeBinary(outFile,entityGroup.name);
    RFileIO::writeBinary(outFile,entityGroup.data);
}


/*********************************************************************
 *  RElementGroup                                                    *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RElementGroup &elementGroup)
{
    RFileIO::readAscii(inFile,static_cast<REntityGroup&>(elementGroup));
    unsigned int nElementIDs = 0;
    RFileIO::readAscii(inFile,nElementIDs);
    elementGroup.elementIDs.resize(nElementIDs);
    for (unsigned int i=0;i<nElementIDs;i++)
    {
        RFileIO::readAscii(inFile,elementGroup.elementIDs[i]);
    }
    unsigned int nBConditions = 0;
    RFileIO::readAscii(inFile,nBConditions);
    RBoundaryCondition bCondition;
    for (unsigned int i=0;i<nBConditions;i++)
    {
        RFileIO::readAscii(inFile,bCondition);
        elementGroup.addBoundaryCondition(bCondition);
    }
    unsigned int nEConditions = 0;
    RFileIO::readAscii(inFile,nEConditions);
    REnvironmentCondition eCondition;
    for (unsigned int i=0;i<nEConditions;i++)
    {
        RFileIO::readAscii(inFile,eCondition);
        elementGroup.addEnvironmentCondition(eCondition);
    }
    unsigned int nIConditions = 0;
    RFileIO::readAscii(inFile,nIConditions);
    RInitialCondition iCondition;
    for (unsigned int i=0;i<nIConditions;i++)
    {
        RFileIO::readAscii(inFile,iCondition);
        elementGroup.addInitialCondition(iCondition);
    }
    RFileIO::readAscii(inFile,elementGroup.material);
}


void RFileIO::readBinary(RFile &inFile, RElementGroup &elementGroup)
{
    RFileIO::readBinary(inFile,static_cast<REntityGroup&>(elementGroup));
    unsigned int nElementIDs = 0;
    RFileIO::readBinary(inFile,nElementIDs);
    elementGroup.elementIDs.resize(nElementIDs);
    for (unsigned int i=0;i<nElementIDs;i++)
    {
        RFileIO::readBinary(inFile,elementGroup.elementIDs[i]);
    }
    unsigned int nBConditions = 0;
    RFileIO::readBinary(inFile,nBConditions);
    RBoundaryCondition bCondition;
    for (unsigned int i=0;i<nBConditions;i++)
    {
        RFileIO::readBinary(inFile,bCondition);
        elementGroup.addBoundaryCondition(bCondition);
    }
    unsigned int nEConditions = 0;
    RFileIO::readBinary(inFile,nEConditions);
    REnvironmentCondition eCondition;
    for (unsigned int i=0;i<nEConditions;i++)
    {
        RFileIO::readBinary(inFile,eCondition);
        elementGroup.addEnvironmentCondition(eCondition);
    }
    unsigned int nIConditions = 0;
    RFileIO::readBinary(inFile,nIConditions);
    RInitialCondition iCondition;
    for (unsigned int i=0;i<nIConditions;i++)
    {
        RFileIO::readBinary(inFile,iCondition);
        elementGroup.addInitialCondition(iCondition);
    }
    RFileIO::readBinary(inFile,elementGroup.material);
}


void RFileIO::writeAscii(RSaveFile &outFile, const RElementGroup &elementGroup, bool addNewLine)
{
    RFileIO::writeAscii(outFile,static_cast<const REntityGroup&>(elementGroup),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,elementGroup.size(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    for (unsigned int i=0;i<elementGroup.elementIDs.size();i++)
    {
        RFileIO::writeAscii(outFile,elementGroup.elementIDs[i],addNewLine);
        if (!addNewLine)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
    RFileIO::writeAscii(outFile,elementGroup.getNBoundaryConditions(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    for (unsigned int i=0;i<elementGroup.bConditions.size();i++)
    {
        RFileIO::writeAscii(outFile,elementGroup.getBoundaryCondition(i),addNewLine);
        if (!addNewLine)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
    RFileIO::writeAscii(outFile,elementGroup.getNEnvironmentConditions(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    for (unsigned int i=0;i<elementGroup.eConditions.size();i++)
    {
        RFileIO::writeAscii(outFile,elementGroup.getEnvironmentCondition(i),addNewLine);
        if (!addNewLine)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
    RFileIO::writeAscii(outFile,elementGroup.getNInitialConditions(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    for (unsigned int i=0;i<elementGroup.iConditions.size();i++)
    {
        RFileIO::writeAscii(outFile,elementGroup.getInitialCondition(i),addNewLine);
        if (!addNewLine)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
    RFileIO::writeAscii(outFile,elementGroup.material,addNewLine);
}


void RFileIO::writeBinary(RSaveFile &outFile, const RElementGroup &elementGroup)
{
    RFileIO::writeBinary(outFile,static_cast<const REntityGroup&>(elementGroup));
    RFileIO::writeBinary(outFile,elementGroup.size());
    for (unsigned int i=0;i<elementGroup.elementIDs.size();i++)
    {
        RFileIO::writeBinary(outFile,elementGroup.elementIDs[i]);
    }
    RFileIO::writeBinary(outFile,elementGroup.getNBoundaryConditions());
    for (unsigned int i=0;i<elementGroup.bConditions.size();i++)
    {
        RFileIO::writeBinary(outFile,elementGroup.getBoundaryCondition(i));
    }
    RFileIO::writeBinary(outFile,elementGroup.getNEnvironmentConditions());
    for (unsigned int i=0;i<elementGroup.eConditions.size();i++)
    {
        RFileIO::writeBinary(outFile,elementGroup.getEnvironmentCondition(i));
    }
    RFileIO::writeBinary(outFile,elementGroup.getNInitialConditions());
    for (unsigned int i=0;i<elementGroup.iConditions.size();i++)
    {
        RFileIO::writeBinary(outFile,elementGroup.getInitialCondition(i));
    }
    RFileIO::writeBinary(outFile,elementGroup.material);
}


/*********************************************************************
 *  RPoint                                                           *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RPoint &point)
{
    RFileIO::readAscii(inFile,static_cast<RElementGroup&>(point));
    RFileIO::readAscii(inFile,point.volume);
}

void RFileIO::readBinary(RFile &inFile, RPoint &point)
{
    RFileIO::readBinary(inFile,static_cast<RElementGroup&>(point));
    RFileIO::readBinary(inFile,point.volume);
}

void RFileIO::writeAscii(RSaveFile &outFile, const RPoint &point, bool addNewLine)
{
    RFileIO::writeAscii(outFile,static_cast<const RElementGroup&>(point),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,point.volume,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RPoint &point)
{
    RFileIO::writeBinary(outFile,static_cast<const RElementGroup&>(point));
    RFileIO::writeBinary(outFile,point.volume);
}


/*********************************************************************
 *  RLine                                                            *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RLine &line)
{
    RFileIO::readAscii(inFile,static_cast<RElementGroup&>(line));
    RFileIO::readAscii(inFile,line.crossArea);
}

void RFileIO::readBinary(RFile &inFile, RLine &line)
{
    RFileIO::readBinary(inFile,static_cast<RElementGroup&>(line));
    RFileIO::readBinary(inFile,line.crossArea);
}

void RFileIO::writeAscii(RSaveFile &outFile, const RLine &line, bool addNewLine)
{
    RFileIO::writeAscii(outFile,static_cast<const RElementGroup&>(line),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,line.crossArea,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RLine &line)
{
    RFileIO::writeBinary(outFile,static_cast<const RElementGroup&>(line));
    RFileIO::writeBinary(outFile,line.crossArea);
}


/*********************************************************************
 *  RSurface                                                         *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RSurface &surface)
{
    RFileIO::readAscii(inFile,static_cast<RElementGroup&>(surface));
    RFileIO::readAscii(inFile,surface.thickness);
}

void RFileIO::readBinary(RFile &inFile, RSurface &surface)
{
    RFileIO::readBinary(inFile,static_cast<RElementGroup&>(surface));
    RFileIO::readBinary(inFile,surface.thickness);
}

void RFileIO::writeAscii(RSaveFile &outFile, const RSurface &surface, bool addNewLine)
{
    RFileIO::writeAscii(outFile,static_cast<const RElementGroup&>(surface),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,surface.thickness,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RSurface &surface)
{
    RFileIO::writeBinary(outFile,static_cast<const RElementGroup&>(surface));
    RFileIO::writeBinary(outFile,surface.thickness);
}


/*********************************************************************
 *  RVolume                                                          *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RVolume &volume)
{
    RFileIO::readAscii(inFile,static_cast<RElementGroup&>(volume));
}

void RFileIO::readBinary(RFile &inFile, RVolume &volume)
{
    RFileIO::readBinary(inFile,static_cast<RElementGroup&>(volume));
}

void RFileIO::writeAscii(RSaveFile &outFile, const RVolume &volume, bool addNewLine)
{
    RFileIO::writeAscii(outFile,static_cast<const RElementGroup&>(volume),addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RVolume &volume)
{
    RFileIO::writeBinary(outFile,static_cast<const RElementGroup&>(volume));
}


/*********************************************************************
 *  RVectorField                                                     *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RVectorField &vectorField)
{
    RFileIO::readAscii(inFile,static_cast<REntityGroup&>(vectorField));
    RFileIO::readAscii(inFile,vectorField.variableType);
    if (inFile.getVersion() > RVersion(0,3,3))
    {
        RFileIO::readAscii(inFile,vectorField.type3d);
    }
    unsigned int nElementGroupIDs;
    RFileIO::readAscii(inFile,nElementGroupIDs);
    vectorField.elementGroupIDs.resize(nElementGroupIDs);
    for (unsigned int i=0;i<nElementGroupIDs;i++)
    {
        RFileIO::readAscii(inFile,vectorField.elementGroupIDs[i]);
    }
}


void RFileIO::readBinary(RFile &inFile, RVectorField &vectorField)
{
    RFileIO::readBinary(inFile,static_cast<REntityGroup&>(vectorField));
    RFileIO::readBinary(inFile,vectorField.variableType);
    if (inFile.getVersion() > RVersion(0,3,3))
    {
        RFileIO::readBinary(inFile,vectorField.type3d);
    }
    unsigned int nElementGroupIDs;
    RFileIO::readBinary(inFile,nElementGroupIDs);
    vectorField.elementGroupIDs.resize(nElementGroupIDs);
    for (unsigned int i=0;i<nElementGroupIDs;i++)
    {
        RFileIO::readBinary(inFile,vectorField.elementGroupIDs[i]);
    }
}


void RFileIO::writeAscii(RSaveFile &outFile, const RVectorField &vectorField, bool addNewLine)
{
    RFileIO::writeAscii(outFile,static_cast<const REntityGroup&>(vectorField),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,vectorField.variableType,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,vectorField.type3d,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,(unsigned int)vectorField.elementGroupIDs.size(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    for (unsigned int i=0;i<vectorField.elementGroupIDs.size();i++)
    {
        RFileIO::writeAscii(outFile,vectorField.elementGroupIDs[i],addNewLine);
        if (!addNewLine && i < vectorField.elementGroupIDs.size()-1)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
}


void RFileIO::writeBinary(RSaveFile &outFile, const RVectorField &vectorField)
{
    RFileIO::writeBinary(outFile,static_cast<const REntityGroup&>(vectorField));
    RFileIO::writeBinary(outFile,vectorField.variableType);
    RFileIO::writeBinary(outFile,vectorField.type3d);
    RFileIO::writeBinary(outFile,(unsigned int)vectorField.elementGroupIDs.size());
    for (unsigned int i=0;i<vectorField.elementGroupIDs.size();i++)
    {
        RFileIO::writeBinary(outFile,vectorField.elementGroupIDs[i]);
    }
}


/*********************************************************************
 *  RScalarField                                                   *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RScalarField &scalarField)
{
    RFileIO::readAscii(inFile,static_cast<RVectorField&>(scalarField));
}

void RFileIO::readBinary(RFile &inFile, RScalarField &scalarField)
{
    RFileIO::readBinary(inFile,static_cast<RVectorField&>(scalarField));
}

void RFileIO::writeAscii(RSaveFile &outFile, const RScalarField &scalarField, bool addNewLine)
{
    RFileIO::writeAscii(outFile,static_cast<const RVectorField&>(scalarField),addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RScalarField &scalarField)
{
    RFileIO::writeBinary(outFile,static_cast<const RVectorField&>(scalarField));
}


/*********************************************************************
 *  RStreamLine                                                      *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RStreamLine &streamLine)
{
    RFileIO::readAscii(inFile,static_cast<REntityGroup&>(streamLine));
    RFileIO::readAscii(inFile,streamLine.variableType);
    RFileIO::readAscii(inFile,streamLine.position);
}


void RFileIO::readBinary(RFile &inFile, RStreamLine &streamLine)
{
    RFileIO::readBinary(inFile,static_cast<REntityGroup&>(streamLine));
    RFileIO::readBinary(inFile,streamLine.variableType);
    RFileIO::readBinary(inFile,streamLine.position);
}


void RFileIO::writeAscii(RSaveFile &outFile, const RStreamLine &streamLine, bool addNewLine)
{
    RFileIO::writeAscii(outFile,static_cast<const REntityGroup&>(streamLine),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,streamLine.variableType,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,streamLine.position,addNewLine);
}


void RFileIO::writeBinary(RSaveFile &outFile, const RStreamLine &streamLine)
{
    RFileIO::writeBinary(outFile,static_cast<const REntityGroup&>(streamLine));
    RFileIO::writeBinary(outFile,streamLine.variableType);
    RFileIO::writeBinary(outFile,streamLine.position);
}


/*********************************************************************
 *  RCut                                                             *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RCut &cut)
{
    RFileIO::readAscii(inFile,static_cast<REntityGroup&>(cut));
    RFileIO::readAscii(inFile,cut.plane);
    unsigned int nElementGroupIDs;
    RFileIO::readAscii(inFile,nElementGroupIDs);
    cut.elementGroupIDs.resize(nElementGroupIDs);
    for (unsigned int i=0;i<nElementGroupIDs;i++)
    {
        RFileIO::readAscii(inFile,cut.elementGroupIDs[i]);
    }
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RCut &cut)
{
    RFileIO::readBinary(inFile,static_cast<REntityGroup&>(cut));
    RFileIO::readBinary(inFile,cut.plane);
    unsigned int nElementGroupIDs;
    RFileIO::readBinary(inFile,nElementGroupIDs);
    cut.elementGroupIDs.resize(nElementGroupIDs);
    for (unsigned int i=0;i<nElementGroupIDs;i++)
    {
        RFileIO::readBinary(inFile,cut.elementGroupIDs[i]);
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RCut &cut, bool addNewLine)
{
    RFileIO::writeAscii(outFile,static_cast<const REntityGroup&>(cut),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,cut.plane,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,(unsigned int)cut.elementGroupIDs.size(),addNewLine);
    for (unsigned int i=0;i<cut.elementGroupIDs.size();i++)
    {
        RFileIO::writeAscii(outFile,cut.elementGroupIDs[i],addNewLine);
        if (!addNewLine && i < cut.elementGroupIDs.size()-1)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RCut &cut)
{
    RFileIO::writeBinary(outFile,static_cast<const REntityGroup&>(cut));
    RFileIO::writeBinary(outFile,cut.plane);
    RFileIO::writeBinary(outFile,(unsigned int)cut.elementGroupIDs.size());
    for (unsigned int i=0;i<cut.elementGroupIDs.size();i++)
    {
        RFileIO::writeBinary(outFile,cut.elementGroupIDs[i]);
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RIso                                                             *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RIso &iso)
{
    RFileIO::readAscii(inFile,static_cast<REntityGroup&>(iso));
    RFileIO::readAscii(inFile,iso.variableType);
    RFileIO::readAscii(inFile,iso.variableValue);
    unsigned int nElementGroupIDs;
    RFileIO::readAscii(inFile,nElementGroupIDs);
    iso.elementGroupIDs.resize(nElementGroupIDs);
    for (unsigned int i=0;i<nElementGroupIDs;i++)
    {
        RFileIO::readAscii(inFile,iso.elementGroupIDs[i]);
    }
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RIso &iso)
{
    RFileIO::readBinary(inFile,static_cast<REntityGroup&>(iso));
    RFileIO::readBinary(inFile,iso.variableType);
    RFileIO::readBinary(inFile,iso.variableValue);
    unsigned int nElementGroupIDs;
    RFileIO::readBinary(inFile,nElementGroupIDs);
    iso.elementGroupIDs.resize(nElementGroupIDs);
    for (unsigned int i=0;i<nElementGroupIDs;i++)
    {
        RFileIO::readBinary(inFile,iso.elementGroupIDs[i]);
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RIso &iso, bool addNewLine)
{
    RFileIO::writeAscii(outFile,static_cast<const REntityGroup&>(iso),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,iso.variableType,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,iso.variableValue,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,(unsigned int)iso.elementGroupIDs.size(),addNewLine);
    for (unsigned int i=0;i<iso.elementGroupIDs.size();i++)
    {
        RFileIO::writeAscii(outFile,iso.elementGroupIDs[i],addNewLine);
        if (!addNewLine && i < iso.elementGroupIDs.size()-1)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RIso &iso)
{
    RFileIO::writeBinary(outFile,static_cast<const REntityGroup&>(iso));
    RFileIO::writeBinary(outFile,iso.variableType);
    RFileIO::writeBinary(outFile,iso.variableValue);
    RFileIO::writeBinary(outFile,(unsigned int)iso.elementGroupIDs.size());
    for (unsigned int i=0;i<iso.elementGroupIDs.size();i++)
    {
        RFileIO::writeBinary(outFile,iso.elementGroupIDs[i]);
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RFileType                                                        *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RFileType &fileType)
{
    int iValue;
    inFile.getTextStream() >> iValue;
    if (inFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to read RFileType value.");
    }
    fileType = RFileType(iValue);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RFileType &fileType)
{
    int iValue;
    RFileIO::readBinary(inFile,iValue);
    fileType = RFileType(iValue);
//    inFile.read((char*)&fileType,sizeof(RFileType));
//    if (inFile.error() != RFile::NoError)
//    {
//        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read RFileType value.");
//    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RFileType &fileType, bool addNewLine)
{
    if (!addNewLine)
    {
        outFile.getTextStream() << int(fileType);
    }
    else
    {
        outFile.getTextStream() << int(fileType) << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write RFileType value.");
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RFileType &fileType)
{
    RFileIO::writeBinary(outFile,int(fileType));
//    outFile.write((char*)&fileType,sizeof(RFileType));
//    if (outFile.error() != RFile::NoError)
//    {
//        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write RFileType value.");
//    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RFileHeader                                                      *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RFileHeader &fileHeader)
{
    RFileIO::readAscii(inFile,fileHeader.version);
    RFileIO::readAscii(inFile,fileHeader.type);
    RFileIO::readAscii(inFile,fileHeader.information);
}

void RFileIO::readBinary(RFile &inFile, RFileHeader &fileHeader)
{
    RFileIO::readBinary(inFile,fileHeader.version);
    RFileIO::readBinary(inFile,fileHeader.type);
    RFileIO::readBinary(inFile,fileHeader.information);
}

void RFileIO::writeAscii(RSaveFile &outFile, const RFileHeader &fileHeader, bool addNewLine)
{
    RFileIO::writeAscii(outFile,fileHeader.version,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,fileHeader.type,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,fileHeader.information,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RFileHeader &fileHeader)
{
    RFileIO::writeBinary(outFile,fileHeader.version);
    RFileIO::writeBinary(outFile,fileHeader.type);
    RFileIO::writeBinary(outFile,fileHeader.information);
}


/*********************************************************************
 *  RTimeMarchApproximation                                          *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RTimeMarchApproximation &timeMarchApproximation)
{
    int iValue;
    inFile.getTextStream() >> iValue;
    if (inFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to read RTimeMarchApproximation value.");
    }
    timeMarchApproximation = RTimeMarchApproximation(iValue);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RTimeMarchApproximation &timeMarchApproximation)
{
    inFile.read((char*)&timeMarchApproximation,sizeof(RTimeMarchApproximation));
    if (inFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read RTimeMarchApproximation value.");
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RTimeMarchApproximation &timeMarchApproximation, bool addNewLine)
{
    if (!addNewLine)
    {
        outFile.getTextStream() << int(timeMarchApproximation);
    }
    else
    {
        outFile.getTextStream() << int(timeMarchApproximation) << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write RTimeMarchApproximation value.");
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RTimeMarchApproximation &timeMarchApproximation)
{
    outFile.write((char*)&timeMarchApproximation,sizeof(RTimeMarchApproximation));
    if (outFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write RTimeMarchApproximation value.");
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RModalMethod                                                     *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RModalMethod &method)
{
    int iValue;
    inFile.getTextStream() >> iValue;
    if (inFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to read RModalMethod value.");
    }
    method = RModalMethod(iValue);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RModalMethod &method)
{
    inFile.read((char*)&method,sizeof(RModalMethod));
    if (inFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read RModalMethod value.");
    }
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RModalMethod &method, bool addNewLine)
{
    if (!addNewLine)
    {
        outFile.getTextStream() << int(method);
    }
    else
    {
        outFile.getTextStream() << int(method) << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write RModalMethod value.");
    }
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RModalMethod &method)
{
    outFile.write((char*)&method,sizeof(RModalMethod));
    if (outFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write RModalMethod value.");
    }
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RTimeSolver                                                      *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RTimeSolver &timeSolver)
{
    RFileIO::readAscii(inFile,timeSolver.enabled);
    RFileIO::readAscii(inFile,timeSolver.timeMarchApproximation);
    unsigned int nTimes = 0;
    RFileIO::readAscii(inFile,nTimes);
    timeSolver.times.resize(nTimes);
    for (unsigned int i=0;i<timeSolver.times.size();i++)
    {
        RFileIO::readAscii(inFile,timeSolver.times[i]);
    }
    RFileIO::readAscii(inFile,timeSolver.inputNTimeSteps);
    RFileIO::readAscii(inFile,timeSolver.inputStartTime);
    RFileIO::readAscii(inFile,timeSolver.inputTimeStepSize);
    RFileIO::readAscii(inFile,timeSolver.currentTimeStep);
    RFileIO::readAscii(inFile,timeSolver.computedTime);
    RFileIO::readAscii(inFile,timeSolver.outputFrequency);
}

void RFileIO::readBinary(RFile &inFile, RTimeSolver &timeSolver)
{
    RFileIO::readBinary(inFile,timeSolver.enabled);
    RFileIO::readBinary(inFile,timeSolver.timeMarchApproximation);
    unsigned int nTimes = 0;
    RFileIO::readBinary(inFile,nTimes);
    timeSolver.times.resize(nTimes);
    for (unsigned int i=0;i<timeSolver.times.size();i++)
    {
        RFileIO::readBinary(inFile,timeSolver.times[i]);
    }
    RFileIO::readBinary(inFile,timeSolver.inputNTimeSteps);
    RFileIO::readBinary(inFile,timeSolver.inputStartTime);
    RFileIO::readBinary(inFile,timeSolver.inputTimeStepSize);
    RFileIO::readBinary(inFile,timeSolver.currentTimeStep);
    RFileIO::readBinary(inFile,timeSolver.computedTime);
    RFileIO::readBinary(inFile,timeSolver.outputFrequency);
}

void RFileIO::writeAscii(RSaveFile &outFile, const RTimeSolver &timeSolver, bool addNewLine)
{
    RFileIO::writeAscii(outFile,timeSolver.enabled,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,timeSolver.timeMarchApproximation,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,(unsigned int)timeSolver.times.size(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    for (unsigned int i=0;i<timeSolver.times.size();i++)
    {
        RFileIO::writeAscii(outFile,timeSolver.times[i],addNewLine);
        if (!addNewLine)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
    RFileIO::writeAscii(outFile,timeSolver.inputNTimeSteps,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,timeSolver.inputStartTime,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,timeSolver.inputTimeStepSize,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,timeSolver.currentTimeStep,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,timeSolver.computedTime,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,timeSolver.outputFrequency,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RTimeSolver &timeSolver)
{
    RFileIO::writeBinary(outFile,timeSolver.enabled);
    RFileIO::writeBinary(outFile,timeSolver.timeMarchApproximation);
    RFileIO::writeBinary(outFile,(unsigned int)timeSolver.times.size());
    for (unsigned int i=0;i<timeSolver.times.size();i++)
    {
        RFileIO::writeBinary(outFile,timeSolver.times[i]);
    }
    RFileIO::writeBinary(outFile,timeSolver.inputNTimeSteps);
    RFileIO::writeBinary(outFile,timeSolver.inputStartTime);
    RFileIO::writeBinary(outFile,timeSolver.inputTimeStepSize);
    RFileIO::writeBinary(outFile,timeSolver.currentTimeStep);
    RFileIO::writeBinary(outFile,timeSolver.computedTime);
    RFileIO::writeBinary(outFile,timeSolver.outputFrequency);
}


/*********************************************************************
 *  RMatrixSolverConf                                                *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RMatrixSolverConf &matrixSolver)
{
    RFileIO::readAscii(inFile,matrixSolver.type);
    RFileIO::readAscii(inFile,matrixSolver.nInnerIterations);
    RFileIO::readAscii(inFile,matrixSolver.nOuterIterations);
    RFileIO::readAscii(inFile,matrixSolver.solverCvgValue);
    RFileIO::readAscii(inFile,matrixSolver.outputFrequency);
} /* RFileIO::readAscii */


void RFileIO::readBinary(RFile &inFile, RMatrixSolverConf &matrixSolver)
{
    RFileIO::readBinary(inFile,matrixSolver.type);
    RFileIO::readBinary(inFile,matrixSolver.nInnerIterations);
    RFileIO::readBinary(inFile,matrixSolver.nOuterIterations);
    RFileIO::readBinary(inFile,matrixSolver.solverCvgValue);
    RFileIO::readBinary(inFile,matrixSolver.outputFrequency);
} /* RFileIO::readBinary */


void RFileIO::writeAscii(RSaveFile &outFile, const RMatrixSolverConf &matrixSolver, bool addNewLine)
{
    RFileIO::writeAscii(outFile,matrixSolver.type,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,matrixSolver.nInnerIterations,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,matrixSolver.nOuterIterations,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,matrixSolver.solverCvgValue,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,matrixSolver.outputFrequency,addNewLine);
} /* RFileIO::writeAscii */


void RFileIO::writeBinary(RSaveFile &outFile, const RMatrixSolverConf &matrixSolver)
{
    RFileIO::writeBinary(outFile,matrixSolver.type);
    RFileIO::writeBinary(outFile,matrixSolver.nInnerIterations);
    RFileIO::writeBinary(outFile,matrixSolver.nOuterIterations);
    RFileIO::writeBinary(outFile,matrixSolver.solverCvgValue);
    RFileIO::writeBinary(outFile,matrixSolver.outputFrequency);
} /* RFileIO::writeBinary */


/*********************************************************************
 *  RMonitoringPoint                                                 *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RMonitoringPoint &monitoringPoint)
{
    RFileIO::readAscii(inFile,monitoringPoint.variableType);
    RFileIO::readAscii(inFile,monitoringPoint.position);
}


void RFileIO::readBinary(RFile &inFile, RMonitoringPoint &monitoringPoint)
{
    RFileIO::readBinary(inFile,monitoringPoint.variableType);
    RFileIO::readBinary(inFile,monitoringPoint.position);
}


void RFileIO::writeAscii(RSaveFile &outFile, const RMonitoringPoint &monitoringPoint, bool addNewLine)
{
    RFileIO::writeAscii(outFile,monitoringPoint.variableType,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,monitoringPoint.position,false,addNewLine);
}


void RFileIO::writeAscii(RFile &outFile, const RMonitoringPoint &monitoringPoint, bool addNewLine)
{
    RFileIO::writeAscii(outFile,monitoringPoint.variableType,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,monitoringPoint.position,false,addNewLine);
}


void RFileIO::writeBinary(RSaveFile &outFile, const RMonitoringPoint &monitoringPoint)
{
    RFileIO::writeBinary(outFile,monitoringPoint.variableType);
    RFileIO::writeBinary(outFile,monitoringPoint.position);
}


/*********************************************************************
 *  RMonitoringPointManager                                          *
 *********************************************************************/


void RFileIO::readAscii(RFile &inFile, RMonitoringPointManager &monitoringPointManager)
{
    unsigned int nMonitoringPoints;
    RFileIO::readAscii(inFile,nMonitoringPoints);
    monitoringPointManager.resize(nMonitoringPoints);
    for (unsigned int i=0;i<nMonitoringPoints;i++)
    {
        RFileIO::readAscii(inFile,monitoringPointManager[i]);
    }
}


void RFileIO::readBinary(RFile &inFile, RMonitoringPointManager &monitoringPointManager)
{
    unsigned int nMonitoringPoints;
    RFileIO::readBinary(inFile,nMonitoringPoints);
    monitoringPointManager.resize(nMonitoringPoints);
    for (unsigned int i=0;i<nMonitoringPoints;i++)
    {
        RFileIO::readBinary(inFile,monitoringPointManager[i]);
    }
}


void RFileIO::writeAscii(RSaveFile &outFile, const RMonitoringPointManager &monitoringPointManager, bool addNewLine)
{
    RFileIO::writeAscii(outFile,(unsigned int)monitoringPointManager.size(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    for (unsigned int i=0;i<monitoringPointManager.size();i++)
    {
        RFileIO::writeAscii(outFile,monitoringPointManager[i],addNewLine);
        if (i < monitoringPointManager.size()-1 && !addNewLine)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
}


void RFileIO::writeBinary(RSaveFile &outFile, const RMonitoringPointManager &monitoringPointManager)
{
    RFileIO::writeBinary(outFile,(unsigned int)monitoringPointManager.size());
    for (unsigned int i=0;i<monitoringPointManager.size();i++)
    {
        RFileIO::writeBinary(outFile,monitoringPointManager[i]);
    }
}


/*********************************************************************
 *  RProblemType                                                     *
 *********************************************************************/

void RFileIO::readAscii(RFile &inFile, RProblemType &problemType)
{
    QString id;
    RFileIO::readAscii(inFile,id);
    problemType = RProblem::getTypeFromId(id);
}

void RFileIO::readBinary(RFile &inFile, RProblemType &problemType)
{
    QString id;
    RFileIO::readBinary(inFile,id);
    problemType = RProblem::getTypeFromId(id);
}

void RFileIO::writeAscii(RSaveFile &outFile, const RProblemType &problemType, bool addNewLine)
{
    RFileIO::writeAscii(outFile,RProblem::getId(problemType),addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RProblemType &problemType)
{
    RFileIO::writeBinary(outFile,RProblem::getId(problemType));
}


/*********************************************************************
 *  RProblemTaskItem                                                 *
 *********************************************************************/

void RFileIO::readAscii(RFile &inFile, RProblemTaskItem &problemTaskItem)
{
    RFileIO::readAscii(inFile,problemTaskItem.problemType);
    RFileIO::readAscii(inFile,problemTaskItem.nIterations);
    uint nChildren = 0;
    RFileIO::readAscii(inFile,nChildren);
    for (uint i=0;i<nChildren;i++)
    {
        RProblemTaskItem child;
        RFileIO::readAscii(inFile,child);
        problemTaskItem.addChild(child);
    }
}

void RFileIO::readBinary(RFile &inFile, RProblemTaskItem &problemTaskItem)
{
    RFileIO::readBinary(inFile,problemTaskItem.problemType);
    RFileIO::readBinary(inFile,problemTaskItem.nIterations);
    uint nChildren = 0;
    RFileIO::readBinary(inFile,nChildren);
    for (uint i=0;i<nChildren;i++)
    {
        RProblemTaskItem child;
        RFileIO::readBinary(inFile,child);
        problemTaskItem.addChild(child);
    }
}

void RFileIO::writeAscii(RSaveFile &outFile, const RProblemTaskItem &problemTaskItem, bool addNewLine)
{
    RFileIO::writeAscii(outFile,problemTaskItem.problemType,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,problemTaskItem.nIterations,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,uint(problemTaskItem.children.size()),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    for (uint i=0;i<problemTaskItem.children.size();i++)
    {
        RFileIO::writeAscii(outFile,problemTaskItem.children[i],addNewLine);
        if (i < problemTaskItem.children.size()-1 && !addNewLine)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
}

void RFileIO::writeBinary(RSaveFile &outFile, const RProblemTaskItem &problemTaskItem)
{
    RFileIO::writeBinary(outFile,problemTaskItem.problemType);
    RFileIO::writeBinary(outFile,problemTaskItem.nIterations);
    RFileIO::writeBinary(outFile,uint(problemTaskItem.children.size()));
    for (uint i=0;i<problemTaskItem.children.size();i++)
    {
        RFileIO::writeBinary(outFile,problemTaskItem.children[i]);
    }
}


/*********************************************************************
 *  RProblemSetup                                                    *
 *********************************************************************/

void RFileIO::readAscii(RFile &inFile, RProblemSetup &problemSetup)
{
    RFileIO::readAscii(inFile,problemSetup.restart);
    RFileIO::readAscii(inFile,problemSetup.radiationSetup);
    RFileIO::readAscii(inFile,problemSetup.modalSetup);
    if (inFile.getVersion() > RVersion(0,3,4))
    {
        RFileIO::readAscii(inFile,problemSetup.meshSetup);
    }
}

void RFileIO::readBinary(RFile &inFile, RProblemSetup &problemSetup)
{
    RFileIO::readBinary(inFile,problemSetup.restart);
    RFileIO::readBinary(inFile,problemSetup.radiationSetup);
    RFileIO::readBinary(inFile,problemSetup.modalSetup);
    if (inFile.getVersion() > RVersion(0,3,4))
    {
        RFileIO::readBinary(inFile,problemSetup.meshSetup);
    }
}

void RFileIO::writeAscii(RSaveFile &outFile, const RProblemSetup &problemSetup, bool addNewLine)
{
    RFileIO::writeAscii(outFile,problemSetup.restart,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,problemSetup.radiationSetup,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,problemSetup.modalSetup,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,problemSetup.meshSetup,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RProblemSetup &problemSetup)
{
    RFileIO::writeBinary(outFile,problemSetup.restart);
    RFileIO::writeBinary(outFile,problemSetup.radiationSetup);
    RFileIO::writeBinary(outFile,problemSetup.modalSetup);
    RFileIO::writeBinary(outFile,problemSetup.meshSetup);
}


/*********************************************************************
 *  RRadiationResolution                                             *
 *********************************************************************/

void RFileIO::readAscii(RFile &inFile, RRadiationResolution &radiationResolution)
{
    int iValue;
    inFile.getTextStream() >> iValue;
    if (inFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF, "Failed to read RRadiationResolution value.");
    }
    radiationResolution = RRadiationResolution(iValue);
}

void RFileIO::readBinary(RFile &inFile, RRadiationResolution &radiationResolution)
{
    inFile.read((char*)&radiationResolution,sizeof(RRadiationResolution));
    if (inFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_READ_FILE,R_ERROR_REF,"Failed to read RRadiationResolution value.");
    }
}

void RFileIO::writeAscii(RSaveFile &outFile, const RRadiationResolution &radiationResolution, bool addNewLine)
{
    if (!addNewLine)
    {
        outFile.getTextStream() << int(radiationResolution);
    }
    else
    {
        outFile.getTextStream() << int(radiationResolution) << RConstants::endl;
    }
    if (outFile.getTextStream().status() != QTextStream::Ok)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write RRadiationResolution value.");
    }
}

void RFileIO::writeBinary(RSaveFile &outFile, const RRadiationResolution &radiationResolution)
{
    outFile.write((char*)&radiationResolution,sizeof(RRadiationResolution));
    if (outFile.error() != RFile::NoError)
    {
        throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write RRadiationResolution value.");
    }
}


/*********************************************************************
 *  RRadiationSetup                                                  *
 *********************************************************************/

void RFileIO::readAscii(RFile &inFile, RRadiationSetup &radiationSetup)
{
    RFileIO::readAscii(inFile,radiationSetup.resolution);
    RFileIO::readAscii(inFile,radiationSetup.viewFactorMatrixFile);
}

void RFileIO::readBinary(RFile &inFile, RRadiationSetup &radiationSetup)
{
    RFileIO::readBinary(inFile,radiationSetup.resolution);
    RFileIO::readBinary(inFile,radiationSetup.viewFactorMatrixFile);
}

void RFileIO::writeAscii(RSaveFile &outFile, const RRadiationSetup &radiationSetup, bool addNewLine)
{
    RFileIO::writeAscii(outFile,radiationSetup.resolution,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,radiationSetup.viewFactorMatrixFile,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RRadiationSetup &radiationSetup)
{
    RFileIO::writeBinary(outFile,radiationSetup.resolution);
    RFileIO::writeBinary(outFile,radiationSetup.viewFactorMatrixFile);
}


/*********************************************************************
 *  RModalSetup                                                      *
 *********************************************************************/

void RFileIO::readAscii(RFile &inFile, RModalSetup &modalSetup)
{
    RFileIO::readAscii(inFile,modalSetup.method);
    RFileIO::readAscii(inFile,modalSetup.nIterations);
    RFileIO::readAscii(inFile,modalSetup.nModesToExtract);
    RFileIO::readAscii(inFile,modalSetup.convergenceValue);
    RFileIO::readAscii(inFile,modalSetup.mode);
    RFileIO::readAscii(inFile,modalSetup.frequency);
}

void RFileIO::readBinary(RFile &inFile, RModalSetup &modalSetup)
{
    RFileIO::readBinary(inFile,modalSetup.method);
    RFileIO::readBinary(inFile,modalSetup.nIterations);
    RFileIO::readBinary(inFile,modalSetup.nModesToExtract);
    RFileIO::readBinary(inFile,modalSetup.convergenceValue);
    RFileIO::readBinary(inFile,modalSetup.mode);
    RFileIO::readBinary(inFile,modalSetup.frequency);
}

void RFileIO::writeAscii(RSaveFile &outFile, const RModalSetup &modalSetup, bool addNewLine)
{
    RFileIO::writeAscii(outFile,modalSetup.method,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,modalSetup.nIterations,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,modalSetup.nModesToExtract,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,modalSetup.convergenceValue,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,modalSetup.mode,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,modalSetup.frequency,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RModalSetup &modalSetup)
{
    RFileIO::writeBinary(outFile,modalSetup.method);
    RFileIO::writeBinary(outFile,modalSetup.nIterations);
    RFileIO::writeBinary(outFile,modalSetup.nModesToExtract);
    RFileIO::writeBinary(outFile,modalSetup.convergenceValue);
    RFileIO::writeBinary(outFile,modalSetup.mode);
    RFileIO::writeBinary(outFile,modalSetup.frequency);
}


/*********************************************************************
 *  RMeshSetup                                                       *
 *********************************************************************/

void RFileIO::readAscii(RFile &inFile, RMeshSetup &meshSetup)
{
    meshSetup.variables.clear();

    uint nVariables = 0;
    RFileIO::readAscii(inFile,nVariables);
    for (uint i=0;i<nVariables;i++)
    {
        RVariableType variableType;
        RFileIO::readAscii(inFile,variableType);
        meshSetup.variables.insert(variableType);
    }
    RFileIO::readAscii(inFile,meshSetup.minEdgeLength);
    RFileIO::readAscii(inFile,meshSetup.maxEdgeLength);
}

void RFileIO::readBinary(RFile &inFile, RMeshSetup &meshSetup)
{
    meshSetup.variables.clear();

    int nVariables = 0;
    RFileIO::readBinary(inFile,nVariables);
    for (int i=0;i<nVariables;i++)
    {
        RVariableType variableType;
        RFileIO::readBinary(inFile,variableType);
        meshSetup.variables.insert(variableType);
    }
    RFileIO::readBinary(inFile,meshSetup.minEdgeLength);
    RFileIO::readBinary(inFile,meshSetup.maxEdgeLength);
}

void RFileIO::writeAscii(RSaveFile &outFile, const RMeshSetup &meshSetup, bool addNewLine)
{
    RFileIO::writeAscii(outFile,meshSetup.variables.size(),addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    foreach (RVariableType variableType, meshSetup.variables)
    {
        RFileIO::writeAscii(outFile,variableType,addNewLine);
        if (!addNewLine)
        {
            RFileIO::writeAscii(outFile,' ',false);
        }
    }
    RFileIO::writeAscii(outFile,meshSetup.minEdgeLength,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,meshSetup.maxEdgeLength,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RMeshSetup &meshSetup)
{
    RFileIO::writeBinary(outFile,meshSetup.variables.size());
    foreach (RVariableType variableType, meshSetup.variables)
    {
        RFileIO::writeBinary(outFile,variableType);
    }
    RFileIO::writeBinary(outFile,meshSetup.minEdgeLength);
    RFileIO::writeBinary(outFile,meshSetup.maxEdgeLength);
}


/*********************************************************************
 *  RBook                                                            *
 *********************************************************************/

void RFileIO::readAscii(RFile &inFile, RBook &book)
{
    RFileIO::readAscii(inFile,book.book,true);
}

void RFileIO::readBinary(RFile &inFile, RBook &book)
{
    RFileIO::readBinary(inFile,book.book,true);
}

void RFileIO::writeAscii(RSaveFile &outFile, const RBook &book, bool addNewLine)
{
    RFileIO::writeAscii(outFile,book.book,true,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RBook &book)
{
    RFileIO::writeBinary(outFile,book.book,true);
}


/*********************************************************************
 *  RPatch                                                           *
 *********************************************************************/

void RFileIO::readAscii(RFile &inFile, RPatch &patch)
{
    RFileIO::readAscii(inFile,patch.surfaceID);
    RFileIO::readAscii(inFile,patch.elementIDs,true);
}

void RFileIO::readBinary(RFile &inFile, RPatch &patch)
{
    RFileIO::readBinary(inFile,patch.surfaceID);
    RFileIO::readBinary(inFile,patch.elementIDs,true);
}

void RFileIO::writeAscii(RSaveFile &outFile, const RPatch &patch, bool addNewLine)
{
    RFileIO::writeAscii(outFile,patch.surfaceID,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,patch.elementIDs,true,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RPatch &patch)
{
    RFileIO::writeBinary(outFile,patch.surfaceID);
    RFileIO::writeBinary(outFile,patch.elementIDs,true);
}


/*********************************************************************
 *  RPatchBook                                                       *
 *********************************************************************/

void RFileIO::readAscii(RFile &inFile, RPatchBook &patchBook)
{
    RFileIO::readAscii(inFile,patchBook.patches);
    uint nElements = 0;
    RFileIO::readAscii(inFile,nElements);
    for (uint i=0;i<nElements;i++)
    {
        uint key = 0;
        uint value = 0;
        RFileIO::readAscii(inFile,key);
        RFileIO::readAscii(inFile,value);
        patchBook.elementPatchMap[key] = value;
    }
}

void RFileIO::readBinary(RFile &inFile, RPatchBook &patchBook)
{
    RFileIO::readBinary(inFile,patchBook.patches);
    uint nElements = 0;
    RFileIO::readBinary(inFile,nElements);
    for (uint i=0;i<nElements;i++)
    {
        uint key = 0;
        uint value = 0;
        RFileIO::readBinary(inFile,key);
        RFileIO::readBinary(inFile,value);
        patchBook.elementPatchMap[key] = value;
    }
}

void RFileIO::writeAscii(RSaveFile &outFile, const RPatchBook &patchBook, bool addNewLine)
{
    RFileIO::writeAscii(outFile,patchBook.patches,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    uint nElements = uint(patchBook.elementPatchMap.size());
    RFileIO::writeAscii(outFile,nElements,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    std::map<uint,uint>::const_iterator iter;
    uint i = 0;
    for (iter=patchBook.elementPatchMap.begin();iter!=patchBook.elementPatchMap.end();++iter)
    {
        if (i < nElements)
        {
            RFileIO::writeAscii(outFile,uint(iter->first),addNewLine);
            RFileIO::writeAscii(outFile,uint(iter->second),addNewLine);
            if (i < (nElements - 1) && !addNewLine)
            {
                RFileIO::writeAscii(outFile,' ',false);
            }
        }
        i++;
    }
}

void RFileIO::writeBinary(RSaveFile &outFile, const RPatchBook &patchBook)
{
    RFileIO::writeBinary(outFile,patchBook.patches);
    uint nElements = uint(patchBook.elementPatchMap.size());
    RFileIO::writeBinary(outFile,nElements);
    std::map<uint,uint>::const_iterator iter;
    uint i = 0;
    for (iter=patchBook.elementPatchMap.begin();iter!=patchBook.elementPatchMap.end();++iter)
    {
        if (i < nElements)
        {
            RFileIO::writeBinary(outFile,uint(iter->first));
            RFileIO::writeBinary(outFile,uint(iter->second));
        }
        i++;
    }
}


/*********************************************************************
 *  RPatchInput                                                      *
 *********************************************************************/

void RFileIO::readAscii(RFile &inFile, RPatchInput &patchInput)
{
    RFileIO::readAscii(inFile,patchInput.patchArea);
    RFileIO::readAscii(inFile,patchInput.patchSize);
    RFileIO::readAscii(inFile,patchInput.separationAngle);
    RFileIO::readAscii(inFile,patchInput.emitter);
    RFileIO::readAscii(inFile,patchInput.receiver);
}

void RFileIO::readBinary(RFile &inFile, RPatchInput &patchInput)
{
    RFileIO::readBinary(inFile,patchInput.patchArea);
    RFileIO::readBinary(inFile,patchInput.patchSize);
    RFileIO::readBinary(inFile,patchInput.separationAngle);
    RFileIO::readBinary(inFile,patchInput.emitter);
    RFileIO::readBinary(inFile,patchInput.receiver);
}

void RFileIO::writeAscii(RSaveFile &outFile, const RPatchInput &patchInput, bool addNewLine)
{
    RFileIO::writeAscii(outFile,patchInput.patchArea,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,patchInput.patchSize,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,patchInput.separationAngle,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,patchInput.emitter,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,patchInput.receiver,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RPatchInput &patchInput)
{
    RFileIO::writeBinary(outFile,patchInput.patchArea);
    RFileIO::writeBinary(outFile,patchInput.patchSize);
    RFileIO::writeBinary(outFile,patchInput.separationAngle);
    RFileIO::writeBinary(outFile,patchInput.emitter);
    RFileIO::writeBinary(outFile,patchInput.receiver);
}


/*********************************************************************
 *  RViewFactorMatrixHeader                                          *
 *********************************************************************/

void RFileIO::readAscii(RFile &inFile, RViewFactorMatrixHeader &viewFactorMatrixHeader)
{
    RFileIO::readAscii(inFile,viewFactorMatrixHeader.patchInput);
    RFileIO::readAscii(inFile,viewFactorMatrixHeader.hemicubeResolution);
    RFileIO::readAscii(inFile,viewFactorMatrixHeader.nElements);
}

void RFileIO::readBinary(RFile &inFile, RViewFactorMatrixHeader &viewFactorMatrixHeader)
{
    RFileIO::readBinary(inFile,viewFactorMatrixHeader.patchInput);
    RFileIO::readBinary(inFile,viewFactorMatrixHeader.hemicubeResolution);
    RFileIO::readBinary(inFile,viewFactorMatrixHeader.nElements);
}

void RFileIO::writeAscii(RSaveFile &outFile, const RViewFactorMatrixHeader &viewFactorMatrixHeader, bool addNewLine)
{
    RFileIO::writeAscii(outFile,viewFactorMatrixHeader.patchInput,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,viewFactorMatrixHeader.hemicubeResolution,addNewLine);
    if (!addNewLine)
    {
        RFileIO::writeAscii(outFile,' ',false);
    }
    RFileIO::writeAscii(outFile,viewFactorMatrixHeader.nElements,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RViewFactorMatrixHeader &viewFactorMatrixHeader)
{
    RFileIO::writeBinary(outFile,viewFactorMatrixHeader.patchInput);
    RFileIO::writeBinary(outFile,viewFactorMatrixHeader.hemicubeResolution);
    RFileIO::writeBinary(outFile,viewFactorMatrixHeader.nElements);
}


/*********************************************************************
 *  RViewFactorRow                                                   *
 *********************************************************************/

void RFileIO::readAscii(RFile &inFile, RViewFactorRow &viewFactorRow)
{
    RFileIO::readAscii(inFile,viewFactorRow.viewFactors);
}

void RFileIO::readBinary(RFile &inFile, RViewFactorRow &viewFactorRow)
{
    RFileIO::readBinary(inFile,viewFactorRow.viewFactors);
}

void RFileIO::writeAscii(RSaveFile &outFile, const RViewFactorRow &viewFactorRow, bool addNewLine)
{
    RFileIO::writeAscii(outFile,viewFactorRow.viewFactors,addNewLine);
}

void RFileIO::writeBinary(RSaveFile &outFile, const RViewFactorRow &viewFactorRow)
{
    RFileIO::writeBinary(outFile,viewFactorRow.viewFactors);
}
