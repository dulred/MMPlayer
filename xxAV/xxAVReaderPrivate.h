#pragma once

extern "C"
{
    #include <libavformat/avformat.h>
}

class xxAVReaderPrivate
{
    public:
        AVFormatContext* formatCtx = nullptr;
};