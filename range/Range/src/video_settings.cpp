/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   video_settings.cpp                                       *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   7-th January 2018                                        *
 *                                                                   *
 *  DESCRIPTION: Video settings class definition                     *
 *********************************************************************/

#include "main_settings.h"
#include "video_settings.h"

const unsigned int VideoSettings::minFps(24);
const unsigned int VideoSettings::maxFps(60);
const unsigned int VideoSettings::minFpp(1);
const unsigned int VideoSettings::maxFpp(100);

void VideoSettings::_init(const VideoSettings *pVideoSettings)
{
    if (pVideoSettings)
    {
        this->width = pVideoSettings->width;
        this->height = pVideoSettings->height;
        this->fps = pVideoSettings->fps;
        this->fpp = pVideoSettings->fpp;
    }
}

VideoSettings::VideoSettings()
    : width(1920)
    , height(1080)
    , fps(24)
    , fpp(1)
    , format(MainSettings::getDefaultVideoFormat())
{
    this->_init();
}

VideoSettings::VideoSettings(const VideoSettings &videoSettings)
{
    this->_init(&videoSettings);
}

VideoSettings::~VideoSettings()
{

}

VideoSettings &VideoSettings::operator =(const VideoSettings &videoSettings)
{
    this->_init(&videoSettings);
    return (*this);
}

unsigned int VideoSettings::getWidth(void) const
{
    return this->width;
}

void VideoSettings::setWidth(unsigned int value)
{
    this->width = value;
    this->width += this->width-2*this->width%2;
}

unsigned int VideoSettings::getHeight(void) const
{
    return this->height;
}

void VideoSettings::setHeight(unsigned int value)
{
    this->height = value;
    this->height += this->height-2*this->height%2;
}

unsigned int VideoSettings::getFps(void) const
{
    return this->fps;
}

void VideoSettings::setFps(unsigned int value)
{
    this->fps = value;
}

unsigned int VideoSettings::getFpp(void) const
{
    return this->fpp;
}

void VideoSettings::setFpp(unsigned int value)
{
    this->fpp = value;
}

QString VideoSettings::getFormat(void) const
{
    return this->format;
}

void VideoSettings::setFormat(const QString &value)
{
    this->format = value;
}
