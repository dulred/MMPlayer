#include "xxPlayer.h"

xxPlayerPushRenderThread::xxPlayerPushRenderThread(xxPlayerCtr* _playerCtr,xxPlayerRenderThread* _renderThread){
        playerCtr = _playerCtr;
        renderThread = _renderThread;
}
xxPlayerPushRenderThread::~xxPlayerPushRenderThread(){
    
}

void xxPlayerPushRenderThread::run(){


        while (!stopFlag)
        {
                if (renderThread->getVideoQueueSize() < 60)
                {
                        // 获取一帧视频
                        xxAVFrame* videoFrame = nullptr;
                        playerCtr->getVideoQueue(&videoFrame);

                        if (videoFrame != nullptr)
                        {
                                // 推送到渲染线程
                                renderThread->pushFrameToVideoQueue(videoFrame);
                                
                        }      
                
                }
        
        }
        

}