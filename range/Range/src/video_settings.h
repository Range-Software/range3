/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   video_settings.h                                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   7-th January 2018                                        *
 *                                                                   *
 *  DESCRIPTION: Video settings class declaration                    *
 *********************************************************************/

#ifndef VIDEO_SETTINGS_H
#define VIDEO_SETTINGS_H

#include <QString>

class VideoSettings
{

    protected:

        unsigned int width;
        unsigned int height;
        unsigned int fps;
        unsigned int fpp;
        QString format;

    public:

        static const unsigned int minFps;
        static const unsigned int maxFps;
        static const unsigned int minFpp;
        static const unsigned int maxFpp;

    private:

        //! Internal initialization function.
        void _init(const VideoSettings *pVideoSettings = nullptr);

    public:

        //! Constructor.
        VideoSettings();

        //! Copy constructor.
        VideoSettings(const VideoSettings &videoSettings);

        //! Destructor.
        ~VideoSettings();

        //! Assignment operator.
        VideoSettings &operator =(const VideoSettings &videoSettings);

        //! Return width.
        unsigned int getWidth(void) const;

        //! Set width.
        void setWidth(unsigned int value);

        //! Return height.
        unsigned int getHeight(void) const;

        //! Set height.
        void setHeight(unsigned int value);

        //! Return frames per second.
        unsigned int getFps(void) const;

        //! Set frames per second.
        void setFps(unsigned int value);

        //! Return frames per picture.
        unsigned int getFpp(void) const;

        //! Set frames per picture.
        void setFpp(unsigned int value);

        //! Get video format.
        QString getFormat(void) const;

        //! Set video format.
        void setFormat(const QString &value);
};

#endif // VIDEO_SETTINGS_H
