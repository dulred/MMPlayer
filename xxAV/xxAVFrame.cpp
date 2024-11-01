#include "xxAV.h"
#include "xxAVFramePrivate.h"

extern "C"
{
    #include <libavutil/pixdesc.h>
}

xxAVFrame::xxAVFrame(){
    imp = new xxAVFramePrivate();
    imp->frame = av_frame_alloc();
}
xxAVFrame::~xxAVFrame(){
    if (imp->frame != nullptr)
    {
        av_frame_free(&imp->frame);
        imp->frame = nullptr;
    }
    if (imp != nullptr)
    {
        delete imp;
        imp = nullptr;
    }
    
    
}

int xxAVFrame::videoPrint(){

    printf("Width:%d\n",imp->frame->width);
    printf("Height:%d\n",imp->frame->height);

    AVPixelFormat format = (AVPixelFormat)(imp->frame->format);
    
    char* str = (char *)malloc(128);
    str = av_get_pix_fmt_string(str,128,format);
    
    printf("Pixel Format: %s\n",str);

    free(str);
    for (int i = 0; i < AV_NUM_DATA_POINTERS; i++)
    {
        printf("Linesize[%d]: %d \n",i,imp->frame->linesize[i]);
    }
    
    return 0;

}

int xxAVFrame::getW(){
    return imp->frame->width;
}
int xxAVFrame::getH(){
    return imp->frame->height;
}
int xxAVFrame::getY(unsigned char* y){
    int width = getW();
    int height = getH();
    for (int i = 0; i < height; i++)
    {
        memcpy(y + i*width,imp->frame->data[0] + i*imp->frame->linesize[0],width);
    }

    return 0;
    
}
int xxAVFrame::getU(unsigned char* u){
    int width = getW()/2;
    int height = getH()/2;
    for (int i = 0; i < height; i++)
    {
        memcpy(u + i*width,imp->frame->data[1] + i*imp->frame->linesize[1],width);
    }

    return 0;
}
int xxAVFrame::getV(unsigned char* v){
    int width = getW()/2;
    int height = getH()/2;
    for (int i = 0; i < height; i++)
    {
        memcpy(v + i*width,imp->frame->data[2] + i*imp->frame->linesize[2],width);
    }

    return 0;
}

long long xxAVFrame::getPts(){
    // printf("pts: %lld",imp->frame->pts);
    // printf("pts Sec:%f \n",imp->ptsSec);
    return (long long)(imp->ptsSec * 1000);
}

int xxAVFrame::audioPrint(){

    int channel = imp->frame->channels;
    printf("Channel: %d\n",channel);
    printf("nb_samples:%d\n",imp->frame->nb_samples);
    printf("sample_rate: %d\n",imp->frame->sample_rate);

    AVSampleFormat format = (AVSampleFormat)(imp->frame->format);
    
    char* str = (char *)malloc(128);
    str = av_get_sample_fmt_string(str,128,format);
    
    printf("Sample Format: %s\n",str);

    free(str);
    for (int i = 0; i < AV_NUM_DATA_POINTERS; i++)
    {
        printf("Linesize[%d]: %d \n",i,imp->frame->linesize[i]);
    }
    
    return 0;
}