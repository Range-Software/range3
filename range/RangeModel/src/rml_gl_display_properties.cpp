/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   rbl_gl_display_properties.cpp                            *
 *  GROUP:  RBL                                                      *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   22-nd February 2012                                      *
 *                                                                   *
 *  DESCRIPTION: OpenGL display properties class definition          *
 *********************************************************************/

#include <QColor>
#include <QOpenGLFunctions>

#include "rml_gl_display_properties.h"
#include "rml_file.h"
#include "rml_file_header.h"
#include "rml_file_io.h"
#include "rml_file_manager.h"

static int lightNums[8] = {
    GL_LIGHT0,
    GL_LIGHT1,
    GL_LIGHT2,
    GL_LIGHT3,
    GL_LIGHT4,
    GL_LIGHT5,
    GL_LIGHT6,
    GL_LIGHT7
};

static const RVersion _version = RVersion(FILE_MAJOR_VERSION,FILE_MINOR_VERSION,FILE_RELEASE_VERSION);

void RGLDisplayProperties::_init(const RGLDisplayProperties *pDisplayProperties)
{
    if (pDisplayProperties)
    {
        this->drawGlobalAxis = pDisplayProperties->drawGlobalAxis;
        this->drawLocalAxis = pDisplayProperties->drawLocalAxis;
        this->showModelEdges = pDisplayProperties->showModelEdges;
        this->showModelDimensions = pDisplayProperties->showModelDimensions;
        this->showErrors = pDisplayProperties->showErrors;
        this->bgColor = pDisplayProperties->bgColor;
        this->bgGradient = pDisplayProperties->bgGradient;
        this->lights = pDisplayProperties->lights;
    }
}

RGLDisplayProperties::RGLDisplayProperties()
    : drawGlobalAxis(true)
    , drawLocalAxis(true)
    , showModelEdges(true)
    , showModelDimensions(true)
    , showErrors(true)
    , bgGradient(true)
{
    this->bgColor = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0).darker();
    this->lights.resize(2);
    this->lights[0].setEnabled(true);
    this->lights[0].setAmbient(QColor("#191919"));
    this->lights[0].setDiffuse(QColor("#DFDFDF"));
    this->lights[0].setSpecular(QColor("#FFFFFF"));
    this->lights[0].setDirection(RR3Vector(0.6,0.6,0.3));
    this->lights[0].setPosition(RR3Vector(-1.0,1.0,10));
    this->lights[0].setLightNumber(lightNums[0]);
    this->lights[0].setLightNumber(0);
    this->lights[1].setEnabled(false);
    this->lights[1].setAmbient(QColor("#191919"));
    this->lights[1].setDiffuse(QColor("#DFDFDF"));
    this->lights[1].setSpecular(QColor("#FFFFFF"));
    this->lights[1].setDirection(RR3Vector(-0.6,0.6,0.3));
    this->lights[1].setPosition(RR3Vector(1.0,1.0,10));
    this->lights[1].setLightNumber(lightNums[1]);
    this->lights[1].setLightNumber(1);
    this->_init();
}

RGLDisplayProperties::RGLDisplayProperties(const RGLDisplayProperties &displayProperties)
{
    this->_init(&displayProperties);
}

RGLDisplayProperties::~RGLDisplayProperties()
{
}

RGLDisplayProperties & RGLDisplayProperties::operator =(const RGLDisplayProperties &displayProperties)
{
    this->_init(&displayProperties);
    return (*this);
}

bool RGLDisplayProperties::getDrawGlobalAxis() const
{
    return this->drawGlobalAxis;
}

void RGLDisplayProperties::setDrawGlobalAxis(bool drawGlobalAxis)
{
    this->drawGlobalAxis = drawGlobalAxis;
}

bool RGLDisplayProperties::getDrawLocalAxis(void) const
{
    return this->drawLocalAxis;
}

void RGLDisplayProperties::setDrawLocalAxis(bool drawLocalAxis)
{
    this->drawLocalAxis = drawLocalAxis;
}

bool RGLDisplayProperties::getShowModelEdges(void) const
{
    return this->showModelEdges;
}

void RGLDisplayProperties::setShowModelEdges(bool showModelEdges)
{
    this->showModelEdges = showModelEdges;
}

bool RGLDisplayProperties::getShowModelDimensions(void) const
{
    return this->showModelDimensions;
}

void RGLDisplayProperties::setShowModelDimensions(bool showModelDimensions)
{
    this->showModelDimensions = showModelDimensions;
}

bool RGLDisplayProperties::getShowErrors(void) const
{
    return this->showErrors;
}

void RGLDisplayProperties::setShowErrors(bool showErrors)
{
    this->showErrors = showErrors;
}

const QColor & RGLDisplayProperties::getBgColor(void) const
{
    return this->bgColor;
}

void RGLDisplayProperties::setBgColor(const QColor &bgColor)
{
    this->bgColor = bgColor;
}

bool RGLDisplayProperties::getBgGradient(void) const
{
    return this->bgGradient;
}

void RGLDisplayProperties::setBgGradient(bool bgGradient)
{
    this->bgGradient = bgGradient;
}

uint RGLDisplayProperties::getNLights(void) const
{
    return uint(this->lights.size());
}

void RGLDisplayProperties::setNLights(uint nLights)
{
    R_ERROR_ASSERT(nLights < 8);

    this->lights.resize(int(nLights));
    for (uint i=0;i<nLights;i++)
    {
        this->lights[int(i)].setLightNumber(lightNums[i]);
    }
}

const RGLLight &RGLDisplayProperties::getLight(uint lightID) const
{
    R_ERROR_ASSERT(lightID < this->getNLights());

    return this->lights[int(lightID)];
}

RGLLight &RGLDisplayProperties::getLight(uint lightID)
{
    R_ERROR_ASSERT(lightID < this->getNLights());

    return this->lights[int(lightID)];
}

void RGLDisplayProperties::setLight(uint lightID, const RGLLight &glLight)
{
    R_ERROR_ASSERT(lightID < this->getNLights());

    this->lights[int(lightID)] = glLight;
}

void RGLDisplayProperties::read(const QString &fileName)
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
            if (ext == RGLDisplayProperties::getDefaultFileExtension(false))
            {
                targetFileName = this->readAscii(targetFileName);
            }
            else if (ext == RGLDisplayProperties::getDefaultFileExtension(true))
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

void RGLDisplayProperties::write(const QString &fileName) const
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    QString ext = RFileManager::getExtension(fileName);

    try
    {
        if (ext == RGLDisplayProperties::getDefaultFileExtension(false))
        {
            this->writeAscii(fileName);
        }
        else if (ext == RGLDisplayProperties::getDefaultFileExtension(true))
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

QString RGLDisplayProperties::getDefaultFileExtension(bool binary)
{
    if (binary)
    {
        return "rbd";
    }
    else
    {
        return "rad";
    }
}

QString RGLDisplayProperties::readAscii(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RLogger::info("Reading ascii display properties file \'%s\'\n",fileName.toUtf8().constData());

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
    if (fileHeader.getType() != R_FILE_TYPE_DISPLAY_PROPERTIES)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF,"File type of the file \'" + fileName + "\' is not DISPLAY PROPERTIES.");
    }

    // Set file version
    file.setVersion(fileHeader.getVersion());

    RFileIO::readAscii(file,(*this));

    file.close();

    return QString();
}

QString RGLDisplayProperties::readBinary(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RLogger::info("Reading binary display properties file \'%s\'\n",fileName.toUtf8().constData());

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
    if (fileHeader.getType() != R_FILE_TYPE_DISPLAY_PROPERTIES)
    {
        throw RError(R_ERROR_INVALID_FILE_FORMAT,R_ERROR_REF,"File type of the file \'" + fileName + "\' is not DISPLAY PROPERTIES.");
    }

    // Set file version
    file.setVersion(fileHeader.getVersion());

    RFileIO::readBinary(file,(*this));

    file.close();

    return QString();
}

void RGLDisplayProperties::writeAscii(const QString &fileName) const
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RLogger::info("Writing ascii display properties file \'%s\'\n",fileName.toUtf8().constData());

    RSaveFile file(fileName,RSaveFile::ASCII);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    RFileIO::writeAscii(file,RFileHeader(R_FILE_TYPE_DISPLAY_PROPERTIES,_version));
    RFileIO::writeAscii(file,(*this));

    file.commit();
}

void RGLDisplayProperties::writeBinary(const QString &fileName) const
{
    if (fileName.isEmpty())
    {
        throw RError(R_ERROR_INVALID_FILE_NAME,R_ERROR_REF,"No file name was provided.");
    }

    RLogger::info("Writing binary display properties file \'%s\'\n",fileName.toUtf8().constData());

    RSaveFile file(fileName,RSaveFile::BINARY);

    if (!file.open(QIODevice::WriteOnly))
    {
        throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
    }

    RFileIO::writeBinary(file,RFileHeader(R_FILE_TYPE_DISPLAY_PROPERTIES,_version));
    RFileIO::writeBinary(file,(*this));

    file.commit();
}
