/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   video_output.h                                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   6-th January 2018                                        *
 *                                                                   *
 *  DESCRIPTION: Video output class declaration                      *
 *               Implements a Qt style wrapper class for some        *
 *               functions from the FFmpeg library                   *
 *********************************************************************/

#ifndef VIDEO_OUTPUT_H
#define VIDEO_OUTPUT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <QObject>
#include <QImage>

extern "C" {
#include <libavutil/mathematics.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class VideoOutput : public QObject
{

    protected:

        AVFormatContext *formatContext;
        AVOutputFormat *outputFormat;
        AVStream *videoStream;
        AVCodec *videoCodec;
        SwsContext *swsContext;
        AVFrame *frame;
        AVPicture srcPicture;
        AVPicture dstPicture;
        AVPixelFormat streamPixFmt;
        uint streamFrameRate;
        int swsFlags;
        uint width;
        uint height;
        int frameCount;
	
    public:

        //! Constructor.
        VideoOutput(QObject * parent=0);

        //! Destructor.
        virtual ~VideoOutput();

        //! Open file for writing.
        bool openMediaFile(int width,
                           int height,
                           const QString & filename);

        //! Close file and write the trailer, if any.
        bool closeMediaFile();

        //! Set stream rate.
        void setStreamRate(uint streamFrameRate);

        //! Set file resolution.
        void setResolution(uint width, uint height);
	   
	public slots:
	
        //! Add new frame to ouput stream.
        bool newFrame(const QImage &image);
	   
	protected:

        //! Add stream.
        AVStream *addStream(AVFormatContext *inFormatContext,
                            AVCodec **codec,
                            AVCodecID codecId) const;

        //! Open video.
        bool openVideo(AVCodec *codec, AVStream *st);

        //! Writes video frame.
        bool writeVideoFrame(const AVPicture &src,
                             int srcWidth,
                             int srcHeight,
                             AVFormatContext *inFormatContext,
                             AVStream *st);

        //! Close video.
        void closeVideo(AVStream *st);

};

#endif // VIDEO_OUTPUT_H
