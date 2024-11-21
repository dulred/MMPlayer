/*
 * @Author: dulred dulred@qq.com
 * @Date: 2024-10-28 15:36:01
 * @LastEditors: dulred dulred@qq.com
 * @LastEditTime: 2024-11-01 09:23:39
 * @FilePath: \xxPlayer\xxPlayer\xxPlayerDecoderThread.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "xxPlayer.h"

xxPlayerDecoderThread::xxPlayerDecoderThread(xxPlayerCtr* _playerCtr ,xxDecoderType _type){

    decoder = new xxAVDecoder();
    playerCtr = _playerCtr;
    type = _type;

}

xxPlayerDecoderThread::~xxPlayerDecoderThread(){
    if (decoder != nullptr)
    {
        delete decoder;
        decoder = nullptr;
    }
}


int xxPlayerDecoderThread::init(xxAVStream* avStream){
    return decoder->init(avStream);
}

int xxPlayerDecoderThread::putPacket(xxAVPacket* pkt){
    return packetQueue.push(pkt);
}


int xxPlayerDecoderThread::getPacketQueueSize(){
    return packetQueue.size();
}

void xxPlayerDecoderThread::run(){
    //进行解码操作
    while (!stopFlag)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));


        if (type == xxDecoderType::XXDECODER_TYPE_VIDEO)
            {
                if (playerCtr->getVideoQueueSize() > 50)
                {
                    continue; //自旋
                }
                
                
            }
        else if (type == xxDecoderType::XXDECODER_TYPE_AUDIO)
            {

                if (playerCtr->getAudioQueueSize() > 50)
                {
                    continue; //自旋
                }
                
            }


        xxAVPacket* pkt =nullptr;
        int ret = packetQueue.pop(&pkt);
        if (ret)
        {
            // 队列为空，取不到 packet
            continue;
        } 
        
        decoder->sendPacket(pkt);

        while (1)
        {
            xxAVFrame *frame = new xxAVFrame();
            ret = decoder->recvFrame(frame);
            if (ret)
            {
               break;
            }
            
            if (type == xxDecoderType::XXDECODER_TYPE_VIDEO)
            {
                playerCtr->pushFrameToVideoQueue(frame);
            }
            else if (type == xxDecoderType::XXDECODER_TYPE_AUDIO)
            {
                playerCtr->pushFrameToAudioQueue(frame);
            }
            
        }
        
    }
    
    
}
