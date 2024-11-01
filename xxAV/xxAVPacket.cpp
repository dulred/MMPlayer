#include "xxAV.h"
#include "xxAVPacketPrivate.h"

xxAVPacket::xxAVPacket(){
    imp = new xxAVPacketPrivate();
    imp->pkt = av_packet_alloc();
}
xxAVPacket::~xxAVPacket(){
    if(imp->pkt != nullptr){
        av_packet_free(&imp->pkt);
        imp->pkt = nullptr;
    }

    if (imp != nullptr)
    {
        delete imp;
        imp = nullptr;
    }
    
}

int xxAVPacket::getIndex(){
    return imp->pkt->stream_index;
}