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
    private:
        xxQueue<xxAVFrame> videoQueue;
        xxQueue<xxAVFrame> audioQueue;
        xxPlayerCtrStatus status = xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PLAYING;
        std::string path;
        double seekTime = 0.0;
};


class xxPlayerReaderThread : public xxThread
{
    public:
        xxPlayerReaderThread(std::string path,double _seekTime,xxPlayerCtr* _playerCtr);
        ~xxPlayerReaderThread();

        virtual void run();
    private:
        std::string path;
        xxPlayerCtr * playerCtr = nullptr;
        double seekTime = 0.0;
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
   private:
        std::string path;
        xxPlayerReaderThread* readerThread = nullptr;
        xxPlayerCtr* playerCtr = nullptr;

};
