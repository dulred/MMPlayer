#pragma once
#include <stdint.h>
#include <stdio.h>
class xxAVPacketPrivate;
class xxAVStreamPrivate;

class xxAVStream;

class xxAVPacket
{
    public:
        xxAVPacket();
        ~xxAVPacket();

        int getIndex();
    public:
        xxAVPacketPrivate* imp = nullptr;
};


class xxAVReaderPrivate;

class xxAVReader
{
    public:
        xxAVReader();
        ~xxAVReader();

        int open(const char * path);
        int close();
        int getStreamCount();
        int getStream(xxAVStream* stream, int streamId);

        int getVideoStreamIndex();
        int getAudioStreamIndex();

        int read(xxAVPacket * packet);

        int seek(double time);
        long long getVideoDuration();
    private:
        xxAVReaderPrivate* imp = nullptr;
};

class xxAVStreamPrivate;

class xxAVStream
{
    public:
        xxAVStream();
        ~xxAVStream();
    public:
        int streamIndex = -1;
        int timebaseNum = 0;
        int timebaseDen = 0;
        xxAVStreamPrivate* imp =nullptr;

};

class xxAVFramePrivate;

class xxAVFrame
{
    public:
        xxAVFrame();
        ~xxAVFrame();

        int videoPrint();
        int audioPrint();
        int getW();
        int getH();
        int getY(unsigned char* y);
        int getU(unsigned char* u);
        int getV(unsigned char* v);

        int getPictureData(unsigned char*& pictureData);

        int getAudioInfo (int& channel,int& samples,int& sample_rate,int& flag,double& play_duration);
        int getAudioData(uint8_t**& data);
        int savePCMData(FILE* &pcmFile);

        long long getPts();


    public:
        xxAVFramePrivate* imp =nullptr;
};

class xxAVDecoderPrivate;

class xxAVDecoder 
{
    public:
        xxAVDecoder();
        ~xxAVDecoder();
        int init(xxAVStream* stream);

        int sendPacket(xxAVPacket *pkt);

        int recvFrame(xxAVFrame* frame);

        int close();
    private:
        xxAVDecoderPrivate* imp = nullptr;
        int timebaseNum = 0;
        int timebaseDen = 0;
};

class xxAVTime
{
    public:
        static long long getTime();
};