/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   video_output.cpp                                         *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   6-th January 2018                                        *
 *                                                                   *
 *  DESCRIPTION: Video output class definition                       *
 *               Implements a Qt style wrapper class for some        *
 *               functions from the FFmpeg library                   *
 *********************************************************************/

#include <rblib.h>

#include "video_output.h"

// Macro redefinition since original does not compile in c++
#undef av_err2str
#define av_err2str(errnum) \
        av_make_error_string(reinterpret_cast<char*>(alloca(AV_ERROR_MAX_STRING_SIZE)),\
                             AV_ERROR_MAX_STRING_SIZE, errnum)

VideoOutput::VideoOutput(QObject *parent)
    : QObject(parent)
    , formatContext(0x0)
    , outputFormat(0x0)
    , videoStream(0x0)
    , videoCodec(0x0)
    , swsContext(0x0)
    , frame(0x0)
    , streamPixFmt(AV_PIX_FMT_YUV420P) // default pix_fmt
    , streamFrameRate(25)              // 25 images/s
    , swsFlags(SWS_BICUBIC)
    , width(640)
    , height(480)
{
    // Init FFmpeg
    av_register_all();
}

VideoOutput::~VideoOutput()
{
}

bool VideoOutput::openMediaFile(int width, int height, const QString &filename)
{
    // allocate the output media context
    avformat_alloc_output_context2(&this->formatContext, NULL, NULL, filename.toUtf8().constData());
    if (!this->formatContext)
    {
        RLogger::warning("Could not deduce output format from file extension: using MPEG.\n");
        avformat_alloc_output_context2(&this->formatContext, NULL, "mpeg", filename.toUtf8().constData());
    }
    if (!this->formatContext)
    {
        return false;
    }
    this->outputFormat = this->formatContext->oformat;
    // Add the video streams using the default format codecs
    // and initialize the codecs.
    this->videoStream = NULL;
    if (this->outputFormat->video_codec != AV_CODEC_ID_NONE)
    {
        this->videoStream = this->addStream(this->formatContext, &this->videoCodec, this->outputFormat->video_codec);
    }
    // Now that all the parameters are set, we can open the audio and
    // video codecs and allocate the necessary encode buffers.
    if (this->videoStream)
    {
        this->openVideo(this->videoCodec, this->videoStream);
    }
    av_dump_format(this->formatContext, 0, filename.toUtf8().constData(), 1);
    int ret = 0;
    // open the output file, if needed
    if (!(this->outputFormat->flags & AVFMT_NOFILE))
    {
       ret = avio_open(&this->formatContext->pb, filename.toUtf8().constData(), AVIO_FLAG_WRITE);
       if (ret < 0)
       {
           RLogger::error("Could not open '%s': %s\n", filename.toUtf8().constData(), av_err2str(ret));
           return false;
       }
    }
    // Write the stream header, if any.
    ret = avformat_write_header(this->formatContext, NULL);
    if (ret < 0)
    {
        RLogger::error("Error occurred when opening output file: %s\n", av_err2str(ret));
        return false;
    }
    if (this->frame)
    {
        this->frame->pts = 0;
    }
    return avpicture_alloc(&this->srcPicture, AV_PIX_FMT_RGBA, width, height) >= 0;
}

bool VideoOutput::closeMediaFile()
{
    av_free(this->srcPicture.data[0]);
    // Write the trailer, if any. The trailer must be written before you
    // close the CodecContexts open when you wrote the header; otherwise
    // av_write_trailer() may try to use memory that was freed on
    // av_codec_close().
    av_write_trailer(this->formatContext);
    // Close each codec.
    if (this->videoStream)
    {
        this->closeVideo(this->videoStream);
    }
    if (this->swsContext)
    {
        sws_freeContext(this->swsContext);
        this->swsContext = 0x0;
    }
    // Free the streams.
    for (unsigned int i = 0; i < this->formatContext->nb_streams; i++)
    {
        av_freep(&this->formatContext->streams[i]->codec);
        av_freep(&this->formatContext->streams[i]);
    }
    if (!(this->outputFormat->flags & AVFMT_NOFILE))
    {
        // Close the output file.
        avio_close(this->formatContext->pb);
    }
    // free the stream
    av_free(this->formatContext);
    return true;
}

void VideoOutput::setStreamRate(uint streamFrameRate)
{
    Q_ASSERT(streamFrameRate >= 24);
    Q_ASSERT(streamFrameRate <= 60);
    this->streamFrameRate = streamFrameRate;
}

void VideoOutput::setResolution(uint width, uint height)
{
    Q_ASSERT(width%2  == 0);
    Q_ASSERT(height%2 == 0);
    this->width  = width;
    this->height = height;
}

bool VideoOutput::newFrame(const QImage &image)
{
    const int width  = image.width();
    const int height = image.height();
    // write video frames
    for (int y=0;y<height;y++)
    {
        const uint8_t *scanline = image.scanLine(y);
        for (int x=0;x<width*4;x++)
        {
            this->srcPicture.data[0][y*this->srcPicture.linesize[0]+x] = scanline[x];
        }
    }
    this->writeVideoFrame(this->srcPicture, width, height, this->formatContext, this->videoStream);
    this->frame->pts += av_rescale_q(1,
                                     this->videoStream->codec->time_base,
                                     this->videoStream->time_base);
    return true;
}

AVStream *VideoOutput::addStream(AVFormatContext *inFormatContext,
                                 AVCodec **codec,
                                 AVCodecID codecId) const
{
    AVCodecContext *c;
    AVStream *st;
    // find the encoder
    *codec = avcodec_find_encoder(codecId);
    if (!(*codec))
    {
        RLogger::error("Could not find encoder for '%s'\n",avcodec_get_name(codecId));
        return 0x0;
    }
    st = avformat_new_stream(inFormatContext, *codec);
    if (!st)
    {
        RLogger::error("Could not allocate stream\n");
        return 0x0;
    }
    st->id = inFormatContext->nb_streams-1;
    c = st->codec;
    switch ((*codec)->type)
    {
        case AVMEDIA_TYPE_AUDIO:
        {
            st->id = 1;
            c->sample_fmt  = AV_SAMPLE_FMT_S16;
            c->bit_rate    = 64000;
            c->sample_rate = 44100;
            c->channels    = 2;
            break;
        }
        case AVMEDIA_TYPE_VIDEO:
        {
            avcodec_get_context_defaults3(c, *codec);
            c->codec_id = codecId;
            c->bit_rate = 400000;
            c->width    = this->width;
            c->height   = this->height;
            // timebase: This is the fundamental unit of time (in seconds) in terms
            // of which frame timestamps are represented. For fixed-fps content,
            // timebase should be 1/framerate and timestamp increments should be
            // identical to 1.
            c->time_base.den = this->streamFrameRate;
            c->time_base.num = 1;
            c->gop_size      = 12; // emit one intra frame every twelve frames at most
            c->pix_fmt       = this->streamPixFmt;
            if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO)
            {
                // just for testing, we also add B frames
                c->max_b_frames = 2;
            }
            if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO)
            {
                // Needed to avoid using macroblocks in which some coeffs overflow.
                // This does not happen with normal video, it just happens here as
                // the motion of the chroma plane does not match the luma plane.
                c->mb_decision = 2;
            }
            break;
        }
        default:
        {
            break;
        }
    }
    // Some formats want stream headers to be separate.
    if (inFormatContext->oformat->flags & AVFMT_GLOBALHEADER)
    {
        c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    return st;
}

bool VideoOutput::openVideo(AVCodec *codec, AVStream *stream)
{
    int ret;
    AVCodecContext *c = stream->codec;
    // open the codec
    ret = avcodec_open2(c, codec, NULL);
    if (ret < 0)
    {
        RLogger::error("Could not open video codec: %s\n", av_err2str(ret));
        return false;
    }
    // allocate and init a re-usable frame
    this->frame = av_frame_alloc();
    if (!this->frame)
    {
        RLogger::error("Could not allocate video frame\n");
        return false;
    }
    // Allocate the encoded raw picture.
    ret = avpicture_alloc(&this->dstPicture, c->pix_fmt, c->width, c->height);
    if (ret < 0)
    {
        RLogger::error("Could not allocate picture: %s\n", av_err2str(ret));
        return false;
    }
    // copy data and linesize picture pointers to frame
    *((AVPicture *)this->frame) = this->dstPicture;
    return true;
}

bool VideoOutput::writeVideoFrame(const AVPicture &src,
                                  int srcWidth,
                                  int srcHeight,
                                  AVFormatContext *inFormatContext,
                                  AVStream *stream)
{
    int ret;
    AVCodecContext *codec = stream->codec;
    if (codec->pix_fmt != AV_PIX_FMT_RGBA)
    {
        // as we only use RGBA picture, we must convert it
        // to the codec pixel format if needed
        if (!this->swsContext)
        {
            this->swsContext = sws_getContext(srcWidth,
                                              srcHeight,
                                              AV_PIX_FMT_BGRA,
                                              codec->width,
                                              codec->height,
                                              codec->pix_fmt,
                                              this->swsFlags,
                                              NULL,
                                              NULL,
                                              NULL);
            if (!this->swsContext)
            {
                RLogger::error("Could not initialize the conversion context\n");
                return false;
            }
        }
        sws_scale(this->swsContext,
                  (const uint8_t * const *)src.data,
                  src.linesize,
                  0,
                  codec->height,
                  this->dstPicture.data,
                  this->dstPicture.linesize);
    }
    if (inFormatContext->oformat->flags & AVFMT_NOFILE)
    {
        // Raw video case - directly store the picture in the packet
        AVPacket pkt;
        av_init_packet(&pkt);
        pkt.flags        |= AV_PKT_FLAG_KEY;
        pkt.stream_index  = stream->index;
        pkt.data          = this->dstPicture.data[0];
        pkt.size          = sizeof(AVPicture);
        ret = av_interleaved_write_frame(inFormatContext, &pkt);
    }
    else
    {
        // encode the image
        AVPacket pkt;
        int gotOutput;
        av_init_packet(&pkt);
        pkt.data = NULL;    // packet data will be allocated by the encoder
        pkt.size = 0;
        ret = avcodec_encode_video2(codec, &pkt, this->frame, &gotOutput);
        if (ret < 0)
        {
            RLogger::error("Error encoding video frame: %s\n", av_err2str(ret));
            return false;
        }
        // If size is zero, it means the image was buffered.
        if (gotOutput)
        {
            if (codec->coded_frame->key_frame)
            {
                pkt.flags |= AV_PKT_FLAG_KEY;
            }
            pkt.stream_index = stream->index;
            // Write the compressed frame to the media file.
            ret = av_interleaved_write_frame(inFormatContext, &pkt);
        }
        else
        {
            ret = 0;
        }
    }
    if (ret != 0)
    {
        RLogger::error("Error while writing video frame: %s\n",av_err2str(ret));
        return false;
    }
    this->frameCount++;
    return true;
}

void VideoOutput::closeVideo(AVStream *stream)
{
    avcodec_close(stream->codec);
    av_free(this->dstPicture.data[0]);
    av_free(this->frame);
}
