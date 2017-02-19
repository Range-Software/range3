/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_display_properties.cpp                                *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   6-th April 2016                                          *
 *                                                                   *
 *  DESCRIPTION: OpenGL display properties class definition          *
 *********************************************************************/

#include "gl_display_properties.h"
#include "main_settings.h"

void GLDisplayProperties::_init(const GLDisplayProperties *pDisplayProperties)
{
    if (pDisplayProperties)
    {
        this->fileName = pDisplayProperties->fileName;
    }
}

GLDisplayProperties::GLDisplayProperties()
{
    this->_init();
    this->bgColor = ApplicationSettings::getDefaultBackgroundColor(MainSettings::getInstance().getApplicationSettings()->getStyle());
}

GLDisplayProperties::GLDisplayProperties(const GLDisplayProperties &displayProperties)
    : RGLDisplayProperties(displayProperties)
{
    this->_init(&displayProperties);
}

GLDisplayProperties::~GLDisplayProperties()
{

}

GLDisplayProperties &GLDisplayProperties::operator =(const GLDisplayProperties &displayProperties)
{
    this->RGLDisplayProperties::operator =(displayProperties);
    this->_init(&displayProperties);
    return (*this);
}

const QString &GLDisplayProperties::getFileName(void) const
{
    return this->fileName;
}

void GLDisplayProperties::setFileName(const QString &fileName)
{
    this->fileName = fileName;
}

void GLDisplayProperties::load(const QString &fileName)
{
    this->fileName = fileName;

    if (!RFile::exists(this->fileName))
    {
        return;
    }

    RLogger::info("Reading display properties file \'%s\'.\n",this->fileName.toUtf8().constData());
    RLogger::indent();
    try
    {
        this->read(this->fileName);
    }
    catch (const RError &rError)
    {
        RLogger::error("Failed to read the file \'%s\': %s\n",this->fileName.toUtf8().constData(),rError.getMessage().toUtf8().constData());
    }
    RLogger::unindent();
}

void GLDisplayProperties::store(void) const
{
    RLogger::info("Writing display properties file \'%s\'.\n",this->fileName.toUtf8().constData());
    RLogger::indent();
    try
    {
        this->write(this->fileName);
    }
    catch (const RError &rError)
    {
        RLogger::error("Failed to write the file \'%s\': %s\n",this->fileName.toUtf8().constData(),rError.getMessage().toUtf8().constData());
    }
    RLogger::unindent();
}
