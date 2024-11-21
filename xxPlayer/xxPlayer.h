#pragma once

#include <iostream>
#include "xxThread/xxThread.h"
#include <xxAV/xxAV.h>
#include <xxQueue/xxQueue.h>

enum xxDecoderType
{
    XXDECODER_TYPE_VIDEO = 0,
    XXDECODER_TYPE_AUDIO = 1
};


enum xxPlayerCtrStatus
{
    XXPLAYER_CTR_STATUS_PLAYING = 0,
    XXPLAYER_CTR_STATUS_PAUSEING = 1
};

class xxPlayerCtr : public xxThread
{
    public:
        xxPlayerCtr(std::string path ,double _seekTime = 0.0);
        ~xxPlayerCtr();

        virtual void run();

        int play();
        int pause();

        int getVideoQueueSize();
        int getAudioQueueSize();
        int pushFrameToVideoQueue(xxAVFrame* frame);
        int pushFrameToAudioQueue(xxAVFrame* frame);

        int getVideoQueue(xxAVFrame** frame );

        int getAudioQueue(xxAVFrame** frame );

        long long getVideoDuration();
    private:
        xxQueue<xxAVFrame> videoQueue;
        xxQueue<xxAVFrame> audioQueue;
        xxPlayerCtrStatus status = xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PAUSEING;
        std::string path;
        double seekTime = 0.0;
        long long duration = 0;
};


class xxPlayerReaderThread : public xxThread
{
    public:
        xxPlayerReaderThread(std::string path,double _seekTime,xxPlayerCtr* _playerCtr);
        ~xxPlayerReaderThread();
        long long getVideoDuration();
        virtual void run();
    private:
        std::string path;
        xxPlayerCtr * playerCtr = nullptr;
        double seekTime = 0.0;
        long long duration = 0;
};

class xxPlayerDecoderThread: public xxThread
{
    public:
        xxPlayerDecoderThread(xxPlayerCtr* _playerCtr ,xxDecoderType _type);
        ~xxPlayerDecoderThread();
        
        virtual void run();

        int init(xxAVStream* avStream);

        int putPacket(xxAVPacket* pkt);

        int getPacketQueueSize();

    private:
        xxAVDecoder* decoder = nullptr;
        xxQueue<xxAVPacket> packetQueue;
        xxPlayerCtr * playerCtr = nullptr;
        xxDecoderType type;    
};


class xxplayer
{
    public:
        xxplayer(std::string path);
        ~xxplayer();

        int open(double time = 0.0);
        int stop();
        
        int play();
        int pause();

        int seek(double time);

        long long getVideoDuration();
   private:
        std::string path;
        xxPlayerReaderThread* readerThread = nullptr;
        xxPlayerCtr* playerCtr = nullptr;

};


class xxPlayerRenderThread: public xxThread
{
    public:
        xxPlayerRenderThread();
        ~xxPlayerRenderThread();

        int pushFrameToVideoQueue(xxAVFrame* frame);

        int getVideoQueueSize();

        int setStartTIme(long long startTime);

        
        virtual void run();

    private:
        xxQueue<xxAVFrame> videoQueue;
        long long startTime = 0;
        double seekTime = 0.0;
        xxPlayerCtrStatus status = xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PLAYING;
};




class xxPlayerAudioThread: public xxThread
{
    public:
        xxPlayerAudioThread();
        ~xxPlayerAudioThread();

        int pushFrameToAudioQueue(xxAVFrame* frame);

        int getAudioQueueSize();
        
        int setStartTIme(long long startTime);

        int loadAudioData(uint8_t** data, const int frameSize,const int numFrames,long long frame_duration,int flag);

        virtual void run();


    private:
        xxQueue<xxAVFrame> audioQueue;
        long long startTime = 0;
        double seekTime = 0.0;
        xxPlayerCtrStatus status = xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PLAYING;
};




class xxPlayerPushAudioThread: public xxThread
{
    public:
        xxPlayerPushAudioThread(xxPlayerCtr* playerCtr,xxPlayerAudioThread* audioThread);
        ~xxPlayerPushAudioThread();
        
        virtual void run();


    private:
        xxPlayerCtr* playerCtr = nullptr;
        xxPlayerAudioThread* audioThread = nullptr;

};


class xxPlayerPushRenderThread: public xxThread
{
    public:
        xxPlayerPushRenderThread(xxPlayerCtr* playerCtr,xxPlayerRenderThread* renderThread);
        ~xxPlayerPushRenderThread();
        
        virtual void run();

    private:
        xxPlayerCtr* playerCtr = nullptr;
        xxPlayerRenderThread* renderThread = nullptr;

};