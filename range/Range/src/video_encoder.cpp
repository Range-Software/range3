/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   video_encoder.cpp                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   27-th May 2022                                           *
 *                                                                   *
 *  DESCRIPTION: Video encoder class definition                      *
 *********************************************************************/

#include "video_encoder.h"

extern "C" {
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libavformat/avformat.h>
}

VideoEncoder::VideoEncoder()
    : oc(nullptr)
    , st(nullptr)
    , c(nullptr)
    , pkt(nullptr)
    , frame(nullptr)
    , swsContext(nullptr)
    , opt(nullptr)
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

    // allocate the output media context
    avformat_alloc_output_context2(&this->oc, NULL, NULL, fileName.toUtf8().constData());
    if (!this->oc)
    {
        RLogger::warning("Could not deduce output format from file extension: using MPEG.\n");
        avformat_alloc_output_context2(&this->oc, NULL, "mpeg", fileName.toUtf8().constData());
        if (!this->oc)
        {
            throw RError(R_ERROR_INVALID_INPUT, R_ERROR_REF, "Failed to allocate memory for AV format context\n");
        }
    }

    // Find encoder
    const AVCodec *codec = nullptr;
    if (this->oc->oformat->video_codec != AV_CODEC_ID_NONE)
    {
        // Find encoder
        codec = avcodec_find_encoder(this->oc->oformat->video_codec);
    }
    if (!codec)
    {
        throw RError(R_ERROR_INVALID_INPUT, R_ERROR_REF, "Could not find encoder for '%s'\n",avcodec_get_name(this->oc->oformat->video_codec));
    }

    // Allocate packet structure
    this->pkt = av_packet_alloc();
    if (this->pkt == nullptr)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Failed to allocate memory for av packet\n");
    }

    // Create new stream
    this->st = avformat_new_stream(this->oc,NULL);
    if (!st)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Could not allocate stream\n");
    }
    this->st->id = this->oc->nb_streams-1;

    // Allocate codec context
    this->c = avcodec_alloc_context3(codec);
    if (this->c == nullptr)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Could not allocate video codec context\n");
    }

    this->c->codec_id = codec->id;
    // put sample parameters
    this->c->bit_rate = 400000;
    // resolution must be a multiple of two
    this->c->width = int(width);
    this->c->height = int(height);
    // frames per second
    this->c->time_base = {1, int(frameRate)};
    this->c->framerate = {int(frameRate), 1};

    this->st->time_base = this->c->time_base;

    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    this->c->gop_size = 10;
    this->c->max_b_frames = 1;
    this->c->pix_fmt = AV_PIX_FMT_YUV420P;

    RLogger::info("Using codec '%s'\n",avcodec_get_name(codec->id));
    if (codec->id == AV_CODEC_ID_H264)
    {
        av_opt_set(this->c->priv_data, "preset", "slow", 0);
    }

    if (this->oc->oformat->flags & AVFMT_GLOBALHEADER)
    {
        c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    // open codec
    int ret = avcodec_open2(this->c, codec, &this->opt);
    if (ret < 0)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Could not open codec\n");
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
    ret = av_frame_get_buffer(this->frame, 0);
    if (ret < 0)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Failed to allocate memory for the video frame data\n");
    }

    ret = av_image_alloc(this->frame->data,this->frame->linesize, this->c->width, this->c->height, this->c->pix_fmt, 1);
    if (ret < 0)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Failed to allocate memory for the video frame image data\n");
    }

    ret = avcodec_parameters_from_context(this->st->codecpar, this->c);
    if (ret < 0)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Could not copy the stream parameters\n");
    }

    av_dump_format(this->oc, 0, fileName.toUtf8().constData(), 1);

    if (!(this->oc->oformat->flags & AVFMT_NOFILE))
    {
        // Close the output file.
        ret = avio_open(&this->oc->pb, fileName.toUtf8().constData(), AVIO_FLAG_WRITE);
        if (ret < 0)
        {
            throw RError(R_ERROR_OPEN_FILE,R_ERROR_REF,"Failed to open the file \'%s\'.",fileName.toUtf8().constData());
        }
    }

    ret = avformat_write_header(this->oc, &this->opt);
    if (ret < 0)
    {
        throw RError(R_ERROR_APPLICATION, R_ERROR_REF, "Failed to write header\n");
    }
}

void VideoEncoder::close()
{
    av_write_trailer(this->oc);

    avcodec_free_context(&this->c);
    av_free(this->frame->data[0]);
    av_frame_free(&this->frame);
    av_packet_free(&this->pkt);

    sws_freeContext(this->swsContext);
    this->swsContext = nullptr;

    if (!(this->oc->oformat->flags & AVFMT_NOFILE))
    {
        // Close the output file.
        avio_closep(&this->oc->pb);
    }

    // free the stream
    avformat_free_context(this->oc);
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

    VideoEncoder::encode(this->oc, this->c, this->st, this->frame, this->pkt);
}

void VideoEncoder::encode(AVFormatContext *fmt_ctx, AVCodecContext *enc_ctx, AVStream *st, AVFrame *frame, AVPacket *pkt)
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

        // rescale output packet timestamp values from codec to stream timebase
        av_packet_rescale_ts(pkt, enc_ctx->time_base, st->time_base);
        pkt->stream_index = st->index;

        // Write the compressed frame to the media file
        RLogger::info("Write packet %3" PRId64 " (size=%5d)\n", pkt->pts, pkt->size);

        ret = av_interleaved_write_frame(fmt_ctx, pkt);
        /* pkt is now blank (av_interleaved_write_frame() takes ownership of
         * its contents and resets pkt), so that no unreferencing is necessary.
         * This would be different if one used av_write_frame(). */
        if (ret < 0) {
            throw RError(R_ERROR_WRITE_FILE,R_ERROR_REF,"Failed to write paket data.");
        }
    }
}
