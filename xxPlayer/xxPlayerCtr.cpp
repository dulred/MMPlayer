#include "xxPlayer.h"

xxPlayerCtr::xxPlayerCtr(double _seekTime){
    seekTime = _seekTime;
}
xxPlayerCtr::~xxPlayerCtr(){

}

int xxPlayerCtr::play(){
    status = xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PLAYING;
    return 0;
}
int xxPlayerCtr::pause(){
    status = xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PAUSEING;
    return 0;
}

void xxPlayerCtr::run(){

    xxPlayerReaderThread readerThread("D:/Videos/ss.mp4",seekTime,this);
    readerThread.start();

    // 获取线程启动时候的时间 start_time
    long long startTime  = xxAVTime::getTime();

    xxAVFrame* videoFrame = nullptr;
    xxAVFrame* audioFrame = nullptr;

    long long sleepCountTime = 0;

    while(!stopFlag){
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        long long sleepTimeStart = xxAVTime::getTime();
        while (status  == xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PAUSEING)
        {
           std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        long long sleepTimeEnd = xxAVTime::getTime();

        long long sleepDtime = sleepTimeEnd - sleepTimeStart;
        sleepCountTime  = sleepCountTime + sleepDtime;
        
        
        //获取当前的时间 now_time
        long long nowTime = xxAVTime::getTime();
        
        //获取到当前时间和开始时间的差值d_time
        long long dTime = nowTime - startTime;

        dTime = dTime - sleepCountTime;
        dTime = dTime + (long long)(seekTime * 1000);


        // printf("DTime: %lld\n",dTime);

        // 从视频缓存队列中，获取一帧视频frame_pts
        if (videoFrame == nullptr)
        {
            // 尝试取一帧出来
            videoQueue.pop(&videoFrame);
        }

        if (videoFrame != nullptr)
        {
            if (videoFrame->getPts() < (long long)(seekTime * 1000))
            {
                delete videoFrame;
                videoFrame = nullptr;
            }
            
        }

        if (videoFrame != nullptr)
        {
            // 如果frame_pts <= d_time
            if (videoFrame->getPts() <= dTime)
            {
               //这帧视频，应该立即播放出来
               printf("video frame : %lld \n",videoFrame->getPts());
               delete videoFrame;
               videoFrame = nullptr;
            }
            else
            {
                 //这帧视频还不到播放的时候，程序进行自旋，或者去处理音频
            }
            
        }
        
        
        // 从音频缓存队列中，获取一帧音频frame_pts
        if (audioFrame == nullptr)
        {
            // 尝试取一帧出来
            audioQueue.pop(&audioFrame);
        }

        if (audioFrame != nullptr)
        {
            if (audioFrame->getPts() < (long long)(seekTime * 1000))
            {
                delete audioFrame;
                audioFrame = nullptr;
            }
            
        }

        if (audioFrame != nullptr)
        {
            // 如果frame_pts <= d_time
            if (audioFrame->getPts() <= dTime)
            {
               //这帧音频，应该立即播放出来
            //    printf("audio frame : %lld \n",audioFrame->getPts());
               delete audioFrame;
               audioFrame = nullptr;
            }
            else
            {
              //这帧音频还不到播放的时候，程序进行自旋
            }
            
        }
            

           


    }

    readerThread.stop();

}

int xxPlayerCtr::getVideoQueueSize()
{
    return videoQueue.size();
}
int xxPlayerCtr::getAudioQueueSize()
{
    return audioQueue.size();
}
int xxPlayerCtr::pushFrameToVideoQueue(xxAVFrame* frame)
{
    return videoQueue.push(frame);
}
int xxPlayerCtr::pushFrameToAudioQueue(xxAVFrame* frame)
{
    return audioQueue.push(frame);
}