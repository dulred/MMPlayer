#include "xxPlayer.h"

xxPlayerReaderThread::xxPlayerReaderThread(std::string _path,double _seekTime,xxPlayerCtr* _playerCtr){
    path=_path;
    playerCtr = _playerCtr;
    seekTime = _seekTime;
}
xxPlayerReaderThread::~xxPlayerReaderThread(){

}

long long xxPlayerReaderThread::getVideoDuration(){
    return this->duration;
}
void xxPlayerReaderThread::run(){
    xxAVReader reader;
    int ret = reader.open(path.c_str());
    if (ret)
    {
        return;
    }

    reader.seek(seekTime);

    this->duration = reader.getVideoDuration();

    int videoStreamIndex =reader.getVideoStreamIndex();
    int audioStreamIndex = reader.getAudioStreamIndex();
    
    //TODO 初始化解码器
    xxPlayerDecoderThread* videoDecoderThread = new xxPlayerDecoderThread(playerCtr,xxDecoderType::XXDECODER_TYPE_VIDEO);
    xxPlayerDecoderThread* audioDecoderThread = new xxPlayerDecoderThread(playerCtr,xxDecoderType::XXDECODER_TYPE_AUDIO);

    xxAVStream videoStream;
    reader.getStream(&videoStream,videoStreamIndex);
    videoDecoderThread->init(&videoStream);

    xxAVStream audioStream;
    reader.getStream(&audioStream,audioStreamIndex);
    audioDecoderThread->init(&audioStream);


    videoDecoderThread->start();
    audioDecoderThread->start();

    while (!stopFlag)
    {
        if (videoDecoderThread->getPacketQueueSize() > 60 && audioDecoderThread->getPacketQueueSize() > 80)
        {
            continue;
        }
        
        xxAVPacket* pkt =new xxAVPacket();
        int ret = reader.read(pkt);
        if (ret)
        {
           delete pkt;
           pkt = nullptr;
           continue;
        }

        if (pkt->getIndex() == videoStreamIndex)
        {
            videoDecoderThread->putPacket(pkt);
        }

        if (pkt->getIndex() == audioStreamIndex)
        {
            audioDecoderThread->putPacket(pkt);
        }
        
    }
    videoDecoderThread->stop();
    audioDecoderThread->stop();

    reader.close();
        
   
    

}