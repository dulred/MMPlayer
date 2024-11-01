#include "xxPlayer.h"
#include "xxThread/xxThread.h"
#include <thread>
class Mythread : public xxThread
{
    private:
        int a = 0;
    public:
        Mythread(int _a){
            a = _a;
        }
        virtual void run(){
            printf("hee  %d: ",a);
        }

};

// int main (){
//     Mythread t(10);
//     t.start();
//     std::this_thread::sleep_for(std::chrono::seconds(2));
//     return 0;
// }

#include "xxAV/xxAV.h"
#include <vector>
#include <xxQueue/xxQueue.h>
int main (){
    xxQueue<xxAVPacket> packetQueue;
    xxAVReader reader;
    int ret = reader.open("D:/Videos/ss.mp4");
    if(ret){
        printf("Open File Fail!!!\n");
        return -1;
    }   

    int videoStreamIndex =reader.getVideoStreamIndex();
    int audioStreamIndex = reader.getAudioStreamIndex();
    printf("videoStreamIndex: %d\n",videoStreamIndex);
    printf("audioStreamIndex: %d\n",audioStreamIndex);

    std::vector<xxAVDecoder*> decoderList;

    int streamCount = reader.getStreamCount();
    for (int i = 0; i < streamCount; i++)
    {
        xxAVStream avStream;
        reader.getStream(&avStream, i);
        printf("StreamIndex:%d \n",avStream.streamIndex);
        xxAVDecoder* decoder = new xxAVDecoder();
        int ret = decoder->init(&avStream);
        if (ret)
        {
            printf("init decoder fail \n");
        }
        decoderList.push_back(decoder);
    }
    FILE* f = fopen("D:/Videos/test.yuv","wb");
    
    while (1)
    {
       xxAVPacket *pkt = new xxAVPacket();
       ret = reader.read(pkt);
       if(ret){
        break;
       }

       packetQueue.push(pkt);

       int streamIndex = pkt->getIndex();
       
       xxAVDecoder* decoder = decoderList[streamIndex];
       
       ret = decoder->sendPacket(pkt);
       if(ret){
        continue;
       }
       while (1)
       {
            xxAVFrame frame;
            ret = decoder->recvFrame(&frame);
            if (ret)
            {
                break;
            }       
                   
            //    Recv Success
            if (streamIndex == videoStreamIndex)
            {
                frame.videoPrint();

                int width = frame.getW();
                int height = frame.getH();
                unsigned char* y = (unsigned char *)malloc(width * height);
                unsigned char* u = (unsigned char *)malloc(width/2 * height/2);
                unsigned char* v = (unsigned char *)malloc(width/2 * height/2);

                frame.getY(y);
                frame.getU(u);
                frame.getV(v);

                fwrite(y,width * height,1,f);
                fwrite(u,width/2 * height/2,1,f);
                fwrite(v,width/2 * height/2,1,f);

                free(y);
                free(u);
                free(v);
            }
            if (streamIndex == audioStreamIndex)
            {
                // frame.audioPrint();
            }     
       }

    }

    for (int i = 0; i < decoderList.size(); i++)
    {
        xxAVDecoder* decoder = decoderList[i];
        
        ret = decoder->sendPacket(nullptr);

        while (1)
        {
            xxAVFrame frame;
            ret = decoder->recvFrame(&frame);
            if (ret)
            {
                break;
            }
              //Recv Success
        }
      
        
    }

    while (packetQueue.size()>0)
    {
        xxAVPacket* pkt = nullptr;
        packetQueue.pop(&pkt);
        printf("Packet size(): %d \n",packetQueue.size());

        if (pkt!=nullptr)
        {
            delete pkt;
            pkt=nullptr;
        }
        
    }
    
    
    reader.close();

    for (int i = 0; i < decoderList.size(); i++)
    {
        xxAVDecoder* decoder = decoderList[i];
        decoder->close();
        delete decoder;
    }
    decoderList.clear();
    fclose(f);
    return 0;
}