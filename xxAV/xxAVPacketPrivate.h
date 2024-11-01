#pragma once

extern "C"
{
    #include <libavformat/avformat.h>
}

class xxAVPacketPrivate
{
    public:
        AVPacket *pkt = nullptr;
};