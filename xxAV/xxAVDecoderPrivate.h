#pragma once

extern "C"
{
    #include <libavformat/avformat.h>
}

class xxAVDecoderPrivate
{
    public:
        AVCodecContext *codecContext = nullptr;
};