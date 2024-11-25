#include "xxPlayer.h"

xxPlayerCtr::xxPlayerCtr(std::string _path,double _seekTime){
    seekTime = _seekTime;
    path = _path;
}

xxPlayerCtr::~xxPlayerCtr(){

}

long long xxPlayerCtr::getVideoDuration(){
    return this->duration;
}

int xxPlayerCtr::play(){
    status = xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PLAYING;
    return 0;
}
int xxPlayerCtr::pause(){
    status = xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PAUSEING;
    return 0;
}

int xxPlayerCtr::getVideoQueue(xxAVFrame** frame ){
// 尝试取一帧出来
    videoQueue.pop(frame);
    return 0;
}


int xxPlayerCtr::getAudioQueue(xxAVFrame** frame ){
// 尝试取一帧出来
    audioQueue.pop(frame);
    return 0;
}

void xxPlayerCtr::run(){

// 启动解码线程
    xxPlayerReaderThread readerThread(path,seekTime,this);
    readerThread.start();

    duration = readerThread.getVideoDuration();

    // 启动渲染线程
    xxPlayerRenderThread renderThread;

    // 启动音频播放线程
    xxPlayerAudioThread audioThread;


    // 预加载视频帧
    while (renderThread.getVideoQueueSize() < 60)
    {

            xxAVFrame *videoFrame = nullptr;
            // 从视频缓存队列中，获取一帧视频frame_pts
            if (videoFrame == nullptr)
            {
                // 尝试取一帧出来
                videoQueue.pop(&videoFrame);
            }

            if (videoFrame!=nullptr)
            {

                renderThread.pushFrameToVideoQueue(videoFrame);
            }
               
    }


    // 预加载音频帧
    while (audioThread.getAudioQueueSize() < 60)
    {

            xxAVFrame *audioFrame = nullptr;
            // 从视频缓存队列中，获取一帧视频frame_pts
            if (audioFrame == nullptr)
            {
                // 尝试取一帧出来
                audioQueue.pop(&audioFrame);
            }

            if (audioFrame!=nullptr)
            {

                audioThread.pushFrameToAudioQueue(audioFrame);
            }
               
    }
    

    // 获取线程启动时候的时间 start_time
    long long startTime  = xxAVTime::getTime();

    // 动态填补视频帧线程
    xxPlayerPushRenderThread pushRenderThread(this,&renderThread);
    pushRenderThread.start();

    // 动态填补音频帧线程
    xxPlayerPushAudioThread pushAudioThread(this,&audioThread);
    pushAudioThread.start();


    // 启动渲染线程
    renderThread.setStartTIme(startTime);
    renderThread.start();

    // 启动音频线程
    audioThread.setStartTIme(startTime);
    audioThread.start();

 
    while (!stopFlag)   
    {
        if (status == xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PLAYING)
        {
           renderThread.setStatus(true);
           audioThread.setStatus(true);
        }else{
            renderThread.setStatus(false);
            audioThread.setStatus(false);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    

    readerThread.stop();
    renderThread.stop();
    audioThread.stop();
    pushRenderThread.stop();
    pushAudioThread.stop();

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