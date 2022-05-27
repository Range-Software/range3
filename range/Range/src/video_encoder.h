/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   video_encoder.h                                          *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   27-th May 2022                                           *
 *                                                                   *
 *  DESCRIPTION: Video encoder class declaration                     *
 *********************************************************************/

#ifndef VIDEO_ENCODER_H
#define VIDEO_ENCODER_H

#include <QString>
#include <QImage>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

class VideoEncoder
{

    protected:

        AVCodecContext *c;
        AVPacket *pkt;
        AVFrame *frame;
        SwsContext *swsContext;
        FILE *f;

    public:

        //! Constructor.
        VideoEncoder();

        //! Open new video file.
        void open(const QString &fileName, uint width, uint height, uint frameRate);

        //! Close video file.
        void close();

        //! Add frame to video.
        void newFrame(const QImage &image);

    private:

        //! Encode video frame.
        static void encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt, FILE *outfile);

};

#endif // VIDEO_ENCODER_H
