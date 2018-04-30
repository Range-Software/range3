/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rml_view_factor_matrix.cpp                               *
 *  GROUP:  RML                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   15-th July 2014                                          *
 *                                                                   *
 *  DESCRIPTION: View-Factor matrix class definition                 *
 *********************************************************************/

#include "rml_view_factor_matrix.h"
#include "rml_file_manager.h"
#include "rml_file_io.h"


static const RVersion _version = RVersion(FILE_MAJOR_VERSION,FILE_MINOR_VERSION,FILE_RELEASE_VERSION);

void RViewFactorMatrix::_init(const RViewFactorMatrix *pViewFactorMatrix)
{
    if (pViewFactorMatrix)
    {
        this->header = pViewFactorMatrix->header;
        this->patchBook = pViewFactorMatrix->patchBook;
        this->rows = pViewFactorMatrix->rows;
    }
}

RViewFactorMatrix::RViewFactorMatrix()
{
    this->_init();
}

RViewFactorMatrix::RViewFactorMatrix(const RViewFactorMatrix &viewFactorMatrix)
{
    this->_init(&viewFactorMatrix);
}

RViewFactorMatrix::~RViewFactorMatrix()
{

}

RViewFactorMatrix &RViewFactorMatrix::operator =(const RViewFactorMatrix &viewFactorMatrix)
{
    this->_init(&viewFactorMatrix);
    return (*this);
}

const RViewFactorMatrixHeader &RViewFactorMatrix::getHeader(void) const
{
    return this->header;
}

RViewFactorMatrixHeader &RViewFactorMatrix::getHeader(void)
{
    return this->header;
}

void RViewFactorMatrix::setHeader(const RViewFactorMatrixHeader &header)
{
    this->header = header;
}

const RPatchBook &RViewFactorMatrix::getPatchBook(void) const
{
    return this->patchBook;
}

RPatchBook &RViewFactorMatrix::getPatchBook(void)
{
    return this->patchBook;
}

uint RViewFactorMatrix::size(void) const
{
    return uint(this->rows.size());
}

void RViewFactorMatrix::resize(uint nRows)
{
    this->rows.resize(nRows);
}

const RViewFactorRow &RViewFactorMatrix::getRow(uint rowID) const
{
    return this->rows[rowID];
}

RViewFactorRow &RViewFactorMatrix::getRow(uint rowID)
{
    return this->rows[rowID];
}

void RViewFactorMatrix::clear(void)
{
    this->header.clear();
    this->patchBook.clear();
    this->rows.clear();
}

void RViewFactorMatrix::read(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QString targetFileName(fileName);

    while (!targetFileName.isEmpty())
    {
        QString ext = RFileManager::getExtension(targetFileName);

        try
        {
            if (ext == RViewFactorMatrix::getDefaultFileExtension(false))
            {
                targetFileName = this->readAscii(targetFileName);
            }
            else if (ext == RViewFactorMatrix::getDefaultFileExtension(true))
            {
                targetFileName = this->readBinary(targetFileName);
            }
            else
            {
                throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF, "Unknown extension \"" + ext + "\".");
            }
        }
        catch (RError &error)
        {
            throw error;
        }
        catch (std::bad_alloc&)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "Memory allocation failed.");
        }
        catch (const std::exception& x)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "%s.", typeid(x).name());
        }
        catch (...)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "Unknown exception.");
        }
    }
}

void RViewFactorMatrix::write(const QString &fileName, const QString &linkFileName) const
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QString ext = RFileManager::getExtension(fileName);

    try
    {
        if (!linkFileName.isEmpty())
        {
            RViewFactorMatrix::writeLink(linkFileName,fileName);
        }

        if (ext == RViewFactorMatrix::getDefaultFileExtension(false))
        {
            this->writeAscii(fileName);
        }
        else if (ext == RViewFactorMatrix::getDefaultFileExtension(true))
        {
            this->writeBinary(fileName);
        }
        else
        {
            throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF, "Unknown extension \"" + ext + "\".");
        }
    }
    catch (RError &error)
    {
        throw error;
    }
    catch (std::bad_alloc&)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "Memory allocation failed.");
    }
    catch (const std::exception& x)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "%s.", typeid(x).name());
    }
    catch (...)
    {
        throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "Unknown exception.");
    }
}

void RViewFactorMatrix::readHeader(const QString &fileName, RViewFactorMatrixHeader &header)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QString targetFileName(fileName);


    while (!targetFileName.isEmpty())
    {
        QString ext = RFileManager::getExtension(targetFileName);

        try
        {
            if (ext == RViewFactorMatrix::getDefaultFileExtension(false))
            {
                targetFileName = RViewFactorMatrix::readAsciiHeader(targetFileName,header);
            }
            else if (ext == RViewFactorMatrix::getDefaultFileExtension(true))
            {
                targetFileName = RViewFactorMatrix::readBinaryHeader(targetFileName,header);
            }
            else
            {
                throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF, "Unknown extension \"" + ext + "\".");
            }
        }
        catch (RError &error)
        {
            throw error;
        }
        catch (std::bad_alloc&)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "Memory allocation failed.");
        }
        catch (const std::exception& x)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "%s.", typeid(x).name());
        }
        catch (...)
        {
            throw RError(R_ERROR_APPLICATION,R_ERROR_REF, "Unknown exception.");
        }
    }
}

QString RViewFactorMatrix::getDefaultFileExtension(bool binary)
{
    return binary ? "rbv" : "rtv";
}

void RViewFactorMatrix::writeLink(const QString &linkFileName, const QString &targetFileName)
{
    if (linkFileName.isEmpty() || targetFileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    bool binary = true;
    QString ext = RFileManager::getExtension(linkFileName);

    if (ext == RViewFactorMatrix::getDefaultFileExtension(false))
    {
        binary = false;
    }
    else if (ext == RViewFactorMatrix::getDefaultFileExtension(true))
    {
        binary = true;
    }
    else
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF, "Unknown extension \"" + ext + "\".");
    }

    RFileManager::writeLink(linkFileName,targetFileName,_version,binary);
}

QString RViewFactorMatrix::readAscii(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RLogger::info("Reading ascii file \'%s\'\n",fileName.toUtf8().constData());

    RFile file(fileName,RFile::ASCII);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    RFileHeader fileHeader;

    RFileIO::readAscii(file,fileHeader);
    if (fileHeader.getType() == R_FILE_TYPE_LINK)
    {
        QString targetFileName(RFileManager::findLinkTargetFileName(fileName,fileHeader.getInformation()));
        RLogger::info("File \'%s\' is a link file pointing to \'%s\'\n",fileName.toUtf8().constData(),targetFileName.toUtf8().constData());
        return targetFileName;
    }
    if (fileHeader.getType() != R_FILE_TYPE_VIEW_FACTOR_MATRIX)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF,"File type of the file \'" + fileName + "\' is not VIEW FACTOR MATRIX.");
    }

    // Set file version
    file.setVersion(fileHeader.getVersion());

    RFileIO::readAscii(file,this->header);
    RFileIO::readAscii(file,this->patchBook);

    uint nRows = 0;
    RFileIO::readAscii(file,nRows);
    this->rows.resize(nRows);
    for (uint i=0;i<this->rows.size();i++)
    {
        RFileIO::readAscii(file,this->rows[i]);
    }

    file.close();

    return QString();
}

QString RViewFactorMatrix::readBinary(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RLogger::info("Reading binary file \'%s\'\n",fileName.toUtf8().constData());

    RFile file(fileName,RFile::BINARY);

    if (!file.open(QIODevice::ReadOnly))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    RFileHeader fileHeader;

    RFileIO::readBinary(file,fileHeader);
    if (fileHeader.getType() == R_FILE_TYPE_LINK)
    {
        QString targetFileName(RFileManager::findLinkTargetFileName(fileName,fileHeader.getInformation()));
        RLogger::info("File \'%s\' is a link file pointing to \'%s\'\n",fileName.toUtf8().constData(),targetFileName.toUtf8().constData());
        return targetFileName;
    }
    if (fileHeader.getType() != R_FILE_TYPE_VIEW_FACTOR_MATRIX)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF,"File type of the file \'" + fileName + "\' is not VIEW FACTOR MATRIX.");
    }

    // Set file version
    file.setVersion(fileHeader.getVersion());

    RFileIO::readBinary(file,this->header);
    RFileIO::readBinary(file,this->patchBook);

    uint nRows = 0;
    RFileIO::readBinary(file,nRows);
    this->rows.resize(nRows);
    for (uint i=0;i<this->rows.size();i++)
    {
        RFileIO::readBinary(file,this->rows[i]);
    }

    file.close();

    return QString();
}

void RViewFactorMatrix::writeAscii(const QString &fileName) const
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RLogger::info("Writing ascii file \'%s\'\n",fileName.toUtf8().constData());

    RSaveFile file(fileName,RSaveFile::ASCII);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    RFileIO::writeAscii(file,RFileHeader(R_FILE_TYPE_VIEW_FACTOR_MATRIX,_version));
    RFileIO::writeAscii(file,this->header);
    RFileIO::writeAscii(file,this->patchBook);
    RFileIO::writeAscii(file,uint(this->rows.size()));
    for (uint i=0;i<this->rows.size();i++)
    {
        RFileIO::writeAscii(file,this->rows[i],false);
        RFileIO::writeNewLineAscii(file);
    }

    file.commit();
}

void RViewFactorMatrix::writeBinary(const QString &fileName) const
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RLogger::info("Writing binary file \'%s\'\n",fileName.toUtf8().constData());

    RSaveFile file(fileName,RSaveFile::BINARY);

    if (!file.open(QIODevice::WriteOnly))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    RFileIO::writeBinary(file,RFileHeader(R_FILE_TYPE_VIEW_FACTOR_MATRIX,_version));
    RFileIO::writeBinary(file,this->header);
    RFileIO::writeBinary(file,this->patchBook);
    RFileIO::writeBinary(file,uint(this->rows.size()));
    for (uint i=0;i<this->rows.size();i++)
    {
        RFileIO::writeBinary(file,this->rows[i]);
    }

    file.commit();
}

QString RViewFactorMatrix::readAsciiHeader(const QString &fileName, RViewFactorMatrixHeader &header)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RLogger::info("Reading ascii file \'%s\'\n",fileName.toUtf8().constData());

    RFile file(fileName,RFile::ASCII);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    RFileHeader fileHeader;

    RFileIO::readAscii(file,fileHeader);
    if (fileHeader.getType() == R_FILE_TYPE_LINK)
    {
        QString targetFileName(RFileManager::findLinkTargetFileName(fileName,fileHeader.getInformation()));
        RLogger::info("File \'%s\' is a link file pointing to \'%s\'\n",fileName.toUtf8().constData(),targetFileName.toUtf8().constData());
        return targetFileName;
    }
    if (fileHeader.getType() != R_FILE_TYPE_VIEW_FACTOR_MATRIX)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF,"File type of the file \'" + fileName + "\' is not VIEW FACTOR MATRIX.");
    }

    // Set file version
    file.setVersion(fileHeader.getVersion());

    RFileIO::readAscii(file,header);

    file.close();

    return QString();
}

QString RViewFactorMatrix::readBinaryHeader(const QString &fileName, RViewFactorMatrixHeader &header)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RLogger::info("Reading binary file \'%s\'\n",fileName.toUtf8().constData());

    RFile file(fileName,RFile::BINARY);

    if (!file.open(QIODevice::ReadOnly))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    RFileHeader fileHeader;

    RFileIO::readBinary(file,fileHeader);
    if (fileHeader.getType() == R_FILE_TYPE_LINK)
    {
        QString targetFileName(RFileManager::findLinkTargetFileName(fileName,fileHeader.getInformation()));
        RLogger::info("File \'%s\' is a link file pointing to \'%s\'\n",fileName.toUtf8().constData(),targetFileName.toUtf8().constData());
        return targetFileName;
    }
    if (fileHeader.getType() != R_FILE_TYPE_VIEW_FACTOR_MATRIX)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF,"File type of the file \'" + fileName + "\' is not VIEW FACTOR MATRIX.");
    }

    // Set file version
    file.setVersion(fileHeader.getVersion());

    RFileIO::readBinary(file,header);

    file.close();

    return QString();
}
