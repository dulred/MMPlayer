#pragma once

extern "C"
{
    #include <libavformat/avformat.h>
}

class xxAVStreamPrivate
{
    public:
        AVCodecParameters *codecpar = nullptr;
};