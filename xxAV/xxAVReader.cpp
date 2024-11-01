#include "xxAV.h"
#include "xxAVReaderPrivate.h"
#include "xxAVPacketPrivate.h"
#include "xxAVStreamPrivate.h"
xxAVReader::xxAVReader(){
    imp = new xxAVReaderPrivate();
    imp->formatCtx = avformat_alloc_context();
}

xxAVReader::~xxAVReader(){
    if(imp->formatCtx != nullptr){
        avformat_free_context(imp->formatCtx);
        imp->formatCtx = nullptr;
    }

}

int xxAVReader::open(const char* path){
    if (imp->formatCtx == nullptr)
    {
        return -1;
    }
    
    int ret = avformat_open_input(&imp->formatCtx,path,nullptr,nullptr);
    if(!ret){
        avformat_find_stream_info(imp->formatCtx,nullptr);
    }
    return ret;
}

int xxAVReader::close(){
    if(imp->formatCtx = nullptr){
        return -1;
    }
    avformat_close_input(&imp->formatCtx);
    return 0;
}

int xxAVReader::read(xxAVPacket* packet){
    if(imp->formatCtx == nullptr){
        return -1;
    }
    int ret = av_read_frame(imp->formatCtx,packet->imp->pkt);

    return ret;
}


int xxAVReader::getStreamCount(){
    return imp->formatCtx->nb_streams;
}
int xxAVReader::getStream(xxAVStream* avStream, int streamId){
    AVStream* ffmpegStream = imp->formatCtx->streams[streamId];

    // ffmpegStream->time_base
    // printf("Timebase num: %d\n",ffmpegStream->time_base.num);
    // printf("Timebase den: %d\n",ffmpegStream->time_base.den);

    avStream->timebaseNum = ffmpegStream->time_base.num;
    avStream->timebaseDen = ffmpegStream->time_base.den;
    

    avStream->streamIndex = ffmpegStream->index;
    avcodec_parameters_copy(avStream->imp->codecpar,ffmpegStream->codecpar);
    return 0;
}


int xxAVReader::getVideoStreamIndex(){
    return av_find_best_stream(imp->formatCtx,AVMediaType::AVMEDIA_TYPE_VIDEO,-1,-1,NULL,NULL);
}
int xxAVReader::getAudioStreamIndex(){
     return av_find_best_stream(imp->formatCtx,AVMediaType::AVMEDIA_TYPE_AUDIO,-1,-1,NULL,NULL);
}


int xxAVReader::seek(double time){
    if (imp->formatCtx == nullptr)
    {
        return -1;
    }
    // printf("fsajafji %d",time);
    int64_t timestamp = (int64_t)(time * AV_TIME_BASE);

    int aa = av_seek_frame(imp->formatCtx,-1,timestamp,AVSEEK_FLAG_BACKWARD);
    return 0;
    
}