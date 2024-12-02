#pragma once

#include <iostream>
#include "xxThread/xxThread.h"
#include <xxAV/xxAV.h>
#include <xxQueue/xxQueue.h>
#include <condition_variable>
#include <atomic>
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
        xxPlayerCtr(std::string path ,double _seekTime,xxQueue<unsigned char>& _renderQueue);
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
        xxPlayerCtrStatus status = xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PLAYING;
        std::string path;
        double seekTime = 0.0;
        long long duration = 0;

        xxQueue<unsigned char>& renderQueue;
};


class xxPlayerReaderThread : public xxThread
{
    public:
        xxPlayerReaderThread(std::string path,double _seekTime,xxPlayerCtr* _playerCtr);
        ~xxPlayerReaderThread();
        long long getVideoDuration();
        virtual void run();
        // 主线程调用该函数等待解码线程完成并获取duration
        long long waitForDuration();
    private:
        std::string path;
        xxPlayerCtr * playerCtr = nullptr;
        double seekTime = 0.0;

    private:
        long long duration = 0;
        std::atomic<bool> done;
        std::mutex mutex;
        std::condition_variable cv;
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

        int open(double time,xxQueue<unsigned char>& renderQueue);
        int stop();
        
        int play();
        int pause();

        int seek(double time,xxQueue<unsigned char>& renderQueue);

        long long getVideoDuration();
   private:
        std::string path;
        xxPlayerReaderThread* readerThread = nullptr;
        xxPlayerCtr* playerCtr = nullptr;

};


class xxPlayerRenderThread: public xxThread
{
    public:
        xxPlayerRenderThread(xxQueue<unsigned char>& _renderQueue);
        ~xxPlayerRenderThread();

        int pushFrameToVideoQueue(xxAVFrame* frame);

        int getVideoQueueSize();

        int setStartTIme(long long startTime);

        int setStatus(bool _status);

        
        virtual void run();

    private:
        xxQueue<xxAVFrame> videoQueue;
        long long startTime = 0;
        double seekTime = 0.0;
        xxPlayerCtrStatus status = xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PLAYING;
        xxQueue<unsigned char>& renderQueue;
};




class xxPlayerAudioThread: public xxThread
{
    public:
        xxPlayerAudioThread();
        ~xxPlayerAudioThread();

        int pushFrameToAudioQueue(xxAVFrame* frame);

        int getAudioQueueSize();
        
        int setStartTIme(long long startTime);

        int setStatus(bool _status);

        int loadAudioData(uint8_t** data, const int frameSize,const int numFrames,long long frame_duration,int flag);

        virtual void run();


    private:
        xxQueue<xxAVFrame> audioQueue;
        long long startTime = 0;
        double seekTime = 0.0;
        xxPlayerCtrStatus status = xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PLAYING;
        long long sleepCountTime = 0;
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