#pragma once

extern "C"
{
    #include <libavformat/avformat.h>
}

class xxAVFramePrivate
{
    public:
        AVFrame* frame = nullptr;
        double ptsSec = 0.0;
};