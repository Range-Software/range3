/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   video_encoder.cpp                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   27-th May 2022                                           *
 *                                                                   *
 *  DESCRIPTION: Video encoder class definition                      *
 *********************************************************************/

#include <rblib.h>

#include "video_encoder.h"

extern "C" {
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libavformat/avformat.h>
}

VideoEncoder::VideoEncoder()
    : c(nullptr)
    , pkt(nullptr)
    , frame(nullptr)
    , swsContext(nullptr)
    , f(nullptr)
{

}

void VideoEncoder::open(const QString &fileName, uint width, uint height, uint frameRate)
{
    if (width % 2 != 0)
    {
        throw RError(R_ERROR_INVALID_INPUT, R_ERROR_REF, "Invalid width '%u' (must be even number)\n", width);
    }
    if (height % 2 != 0)
    {
        throw RError(R_ERROR_INVALID_INPUT, R_ERROR_REF, "Invalid height '%u' (must be even number)\n", height);
    }
    if (frameRate < 24 || frameRate > 60)
    {
        throw RError(R_ERROR_INVALID_INPUT, R_ERROR_REF, "Invalid frame rate '%u' (min=24, max=60)\n", frameRate);
    }

    AVFormatContext *formatContext = nullptr;

    // allocate the output media context
    avformat_alloc_output_context2(&formatContext, NULL, NULL, fileName.toUtf8().constData());
    if (!formatContext)
    {
        RLogger::warning("Could not deduce output format from file extension: using MPEG.\n");
        avformat_alloc_output_context2(&formatContext, NULL, "mpeg", fileName.toUtf8().constData());
        if (!formatContext)
        {
            throw RError(R_ERROR_INVALID_INPUT, R_ERROR_REF, "Failed to allocate memory for AV format context\n");
        }
    }

    // Find encoder
    const AVCodec *codec = nullptr;
    if (formatContext->oformat->video_codec != AV_CODEC_ID_NONE)
    {
        // Find encoder
        codec = avcodec_find_encoder(formatContext->oformat->video_codec);
    }
    if (!codec)
    {
        throw RError(R_ERROR_INVALID_INPUT, R_ERROR_REF, "Could not find encoder for '%s'\n",avcodec_get_name(formatContext->oformat->video_codec));
    }

    this->c = avcodec_alloc_context3(codec);
    if (this->c == nullptr)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Could not allocate video codec context\n");
    }

    this->pkt = av_packet_alloc();
    if (this->pkt == nullptr)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Failed to allocate memory for av packet\n");
    }

    // put sample parameters
    this->c->bit_rate = 400000;
    // resolution must be a multiple of two
    this->c->width = int(width);
    this->c->height = int(height);
    // frames per second
    this->c->time_base = (AVRational){1, int(frameRate)};
    this->c->framerate = (AVRational){int(frameRate), 1};

    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    this->c->gop_size = 10;
    this->c->max_b_frames = 1;
    this->c->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec->id == AV_CODEC_ID_H264)
    {
        av_opt_set(this->c->priv_data, "preset", "slow", 0);
    }

    // open codec
    int ret = avcodec_open2(this->c, codec, NULL);
    if (ret < 0)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Could not open codec\n");
    }
    this->f = fopen(fileName.toUtf8().constData(), "wb");
    if (!f)
    {
        throw RError(R_ERROR_OPEN_FILE, R_ERROR_REF, "Could not open file '%s'\n", fileName.toUtf8().constData());
    }
    this->frame = av_frame_alloc();
    if (this->frame == nullptr)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Failed to allocate memory for video frame\n");
    }
    this->frame->format = this->c->pix_fmt;
    this->frame->width  = this->c->width;
    this->frame->height = this->c->height;
    this->frame->pts = 0;
    ret = av_frame_get_buffer(this->frame, 32);
    if (ret < 0)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Failed to allocate memory for the video frame data\n");
    }

    ret = av_image_alloc(this->frame->data,this->frame->linesize, this->c->width, this->c->height, this->c->pix_fmt, 1);
    if (ret < 0)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Failed to allocate memory for the video frame image data\n");
    }
}

void VideoEncoder::close()
{
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };

    // flush the encoder
    VideoEncoder::encode(this->c, NULL, this->pkt, this->f);

    // add sequence end code to have a real MPEG file
    fwrite(endcode, 1, sizeof(endcode), this->f);
    fclose(this->f);
    avcodec_free_context(&this->c);
    av_free(this->frame->data[0]);
    av_frame_free(&this->frame);
    av_packet_free(&this->pkt);

    sws_freeContext(this->swsContext);
    this->swsContext = nullptr;
}

void VideoEncoder::newFrame(const QImage &image)
{
    // make sure the frame data is writable
    int ret = av_frame_make_writable(this->frame);
    if (ret < 0)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Failed to make frame data writable\n");
    }

    struct ImageData
    {
        uint8_t *data[AV_NUM_DATA_POINTERS];
        int linesize[AV_NUM_DATA_POINTERS];
    } srcImage;

    ret = av_image_alloc(srcImage.data,srcImage.linesize, image.width(), image.height(), AV_PIX_FMT_RGBA, 1);
    if (ret < 0)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Failed to allocate memory for source image data\n");
    }

    // write video frames
    for (int y=0;y<image.height();y++)
    {
        const uint8_t *scanline = image.scanLine(y);
        for (int x=0;x<image.width()*4;x++)
        {
            uint8_t d = scanline[x];
            srcImage.data[0][y*srcImage.linesize[0]+x] = d;
        }
    }

    if (this->c->pix_fmt != AV_PIX_FMT_RGBA)
    {
        // as we only use RGBA picture, we must convert it
        // to the codec pixel format if needed
        if (!this->swsContext)
        {
            this->swsContext = sws_getContext(image.width(),
                                              image.height(),
                                              AV_PIX_FMT_BGRA,
                                              this->c->width,
                                              this->c->height,
                                              this->c->pix_fmt,
                                              SWS_BICUBIC,
                                              NULL,
                                              NULL,
                                              NULL);
            if (!this->swsContext)
            {
                throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Could not initialize conversion context\n");
            }
        }
        sws_scale(this->swsContext,
                  (const uint8_t * const *)srcImage.data,
                  srcImage.linesize,
                  0,
                  this->c->height,
                  this->frame->data,
                  this->frame->linesize);

        this->frame->pts ++;
    }

    av_free(srcImage.data[0]);

    VideoEncoder::encode(this->c, this->frame, this->pkt, this->f);
}

void VideoEncoder::encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt, FILE *outfile)
{
    int ret;
    // send the frame to the encoder
    if (frame)
    {
        RLogger::info("Send frame %3" PRId64 "\n", frame->pts);
    }
    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Error sending a frame for encoding\n");
    }
    while (ret >= 0)
    {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            return;
        }
        else if (ret < 0)
        {
            throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Error during encoding\n");
        }
        RLogger::info("Write packet %3" PRId64 " (size=%5d)\n", pkt->pts, pkt->size);
        fwrite(pkt->data, 1, pkt->size, outfile);
        av_packet_unref(pkt);
    }
}
