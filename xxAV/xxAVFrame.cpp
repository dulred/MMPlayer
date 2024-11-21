#include "xxAV.h"
#include "xxAVFramePrivate.h"

#include <iostream>
#include <vector>

#include <thread>
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



void yuv420p_to_rgb(unsigned char* y, unsigned char* u, unsigned char* v, 
                    unsigned char* rgb, int width, int height) {
    int frameSize = width * height;
    int halfWidth = width / 2;
    int halfHeight = height / 2;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int yIndex = j * width + i;       // Y 平面的索引

            // 计算 U 和 V 平面的索引，注意：每 2x2 像素共享一对 U 和 V
            int uIndex = (j / 2) * halfWidth + (i / 2);
            int vIndex = uIndex;

            // 获取 YUV 分量
            int Y = y[yIndex];
            int U = u[uIndex] - 128;  // U 分量平移到中心点 128
            int V = v[vIndex] - 128;  // V 分量平移到中心点 128

            // 使用公式计算 R, G, B 分量
            int R = Y + 1.402 * V;
            int G = Y - 0.344136 * U - 0.714136 * V;
            int B = Y + 1.772 * U;

            // 限制 RGB 范围在 [0, 255] 之间
            R = (R < 0) ? 0 : (R > 255) ? 255 : R;
            G = (G < 0) ? 0 : (G > 255) ? 255 : G;
            B = (B < 0) ? 0 : (B > 255) ? 255 : B;

            // 将 RGB 值存入目标数组
            int rgbIndex = yIndex * 3; // RGB 数组中每个像素占 3 个字节
            rgb[rgbIndex] = R;
            rgb[rgbIndex + 1] = G;
            rgb[rgbIndex + 2] = B;
        }
    }
}


void printRGBData(unsigned char* rgb, int width, int height) {
    // 依次遍历 RGB 数据并打印
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // 每个像素的 RGB 值位于 rgb[3 * (i * width + j)] (R)
            // rgb[3 * (i * width + j) + 1] (G)
            // rgb[3 * (i * width + j) + 2] (B)
            unsigned char R = rgb[3 * (i * width + j)];
            unsigned char G = rgb[3 * (i * width + j) + 1];
            unsigned char B = rgb[3 * (i * width + j) + 2];

            // 打印像素的 RGB 值
            std::cout << "Pixel (" << i << ", " << j << "): "
                      << "R = " << (int)R << ", "
                      << "G = " << (int)G << ", "
                      << "B = " << (int)B << std::endl;
        }
    }
}

int xxAVFrame::videoPrint(){
    int width = imp->frame->width;
    int height = imp->frame->height;
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

int xxAVFrame::getPictureData(unsigned char*& pictureData){
    int width = imp->frame->width;
    int height = imp->frame->height;
    // 结合OpenGL渲染
    unsigned char* y = new unsigned char[width * height];           // Y 平面数组
    unsigned char* u = new unsigned char[width * height / 4];       // U 平面数组
    unsigned char* v = new unsigned char[width * height / 4];       // V 平面数组
    unsigned char* rgb = new unsigned char[width * height * 3];     // RGB 数组，3 字节每个像素

    getY(y);
    getU(u);
    getV(v);

    // YUV 转换为 RGB
    yuv420p_to_rgb(y, u, v, rgb, width, height);

    pictureData = new unsigned char[width * height * 3];  // 分配内存

    // 确保 rgb 中的数据已经准备好，然后拷贝数据
    memcpy(pictureData, rgb, width * height * 3);


    // for (int i = 0; i < 5; ++i) {
    //      printf(" 0x%02X ",pictureData[i]);
    // }
    // rgb 数组现在包含了 RGB 数据，接下来可以用于保存或显示
    // printRGBData(pictureData, width, height);

    // 使用完成后释放内存
    delete[] y;
    delete[] u;
    delete[] v;
    delete[] rgb;

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
    int samples = imp->frame->nb_samples;
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


int xxAVFrame::getAudioInfo (int& channel,int& samples,int& sample_rate,int& flag,double& play_duration){
     
     channel = imp->frame->channels;
     samples = imp->frame->nb_samples;
     sample_rate = imp->frame->sample_rate;
     play_duration = (double)samples / sample_rate;
    //  AVSampleFormat format = (AVSampleFormat)(imp->frame->format);
    // 判断采样格式是平面格式还是交错格式
    // int planar = av_sample_fmt_is_planar(format); 

    if (imp->frame->format == AV_SAMPLE_FMT_S16P) {
        flag = 1;
    }else if (imp->frame->format == AV_SAMPLE_FMT_S16) {
        flag = 2;
    }


    return 0;
}

int xxAVFrame::getAudioData(uint8_t**& data){
    data = imp->frame->data;
    return 0;
}

int xxAVFrame::savePCMData(FILE* &pcmFile){
    uint8_t** data = imp->frame->data;
    // 将交错格式的 PCM 数据从 data[0] 写入文件
    int num_channels = imp->frame->channels;
    int num_samples = imp->frame->nb_samples;

    // 直接将交错的 PCM 数据写入文件
    fwrite(data[0], 1, sizeof(int16_t) * num_channels * num_samples, pcmFile);

}