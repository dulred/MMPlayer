#include "xxPlayer.h"

xxplayer::xxplayer(std::string _path){
    path = _path;
}

xxplayer::~xxplayer(){

}

int xxplayer::open(double time){
    // if (readerThread == nullptr)
    // {
    //     readerThread = new xxPlayerReaderThread(path);
    //     readerThread->start();
    //     return 0;
    // }

    if (playerCtr == nullptr)
    {
        playerCtr = new xxPlayerCtr(path,time);
        playerCtr->start();
        return 0;
    }
    
    return -1;
}

int xxplayer::stop(){
    // if (readerThread != nullptr)
    // {
    //     readerThread->stop();
    //     delete readerThread;
    //     readerThread = nullptr;
    // }

     if (playerCtr != nullptr)
    {
        playerCtr->stop();
        delete playerCtr;
        playerCtr = nullptr;
    }
    
    return 0;
}

int xxplayer::play(){
    if (playerCtr != nullptr)
    {
        playerCtr->play();
    }
    return 0;
}
int xxplayer::pause(){
    if (playerCtr != nullptr)
    {
        playerCtr->pause();
    }
    return 0;
}

int xxplayer::seek(double time){
    stop();
    open(time);
    return 0;
}