#include "xxPlayer.h"


xxPlayerPushAudioThread::xxPlayerPushAudioThread(xxPlayerCtr* _playerCtr,xxPlayerAudioThread* _audioThread){
    playerCtr = _playerCtr;
    audioThread = _audioThread;
}
xxPlayerPushAudioThread::~xxPlayerPushAudioThread(){
    
}

void xxPlayerPushAudioThread::run(){

         while (!stopFlag)
        {   
                if (audioThread->getAudioQueueSize()<60)
                {
                        // 获取一帧视频
                        xxAVFrame* audioFrame = nullptr;
                        playerCtr->getAudioQueue(&audioFrame);

                        if (audioFrame != nullptr)
                        {
                                // 推送到渲染线程
                                audioThread->pushFrameToAudioQueue(audioFrame);
                                
                        }      
                
                }
        
        }

}