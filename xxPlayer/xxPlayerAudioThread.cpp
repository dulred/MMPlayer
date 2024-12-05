#include "xxPlayer.h"
#include <al.h>
#include <alc.h>

xxPlayerAudioThread::xxPlayerAudioThread(){

}
xxPlayerAudioThread::~xxPlayerAudioThread(){

}


int xxPlayerAudioThread::setStartTIme(long long _startTime){
       startTime = _startTime;
       return 0;
}


int xxPlayerAudioThread::pushFrameToAudioQueue(xxAVFrame* frame)
{
    return audioQueue.push(frame);
}


int xxPlayerAudioThread::getAudioQueueSize()
{
    return audioQueue.size();
}

int xxPlayerAudioThread::setStatus(bool _status){
    if (_status)
    {
        status = xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PLAYING;
    }else{
        status = xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PAUSEING;
    }

    return 0;
    
}


int xxPlayerAudioThread::loadAudioData(uint8_t** data, const int frameSize,const int numFrames,long long frame_duration,int flag){

    int totalBufferSize = frameSize * numFrames;
    
    // 使用 new 动态分配内存
    uint8_t* dynamicBuffer = (uint8_t*)malloc(totalBufferSize);

    xxAVFrame* audioFrame = nullptr;
    uint8_t** dataTemp = nullptr;
    int count = 0;
    int offset = 0;
    // FILE * pcmFile = fopen("D:/videos/output2.pcm", "ab"); 
    while (count < numFrames) {
        if (getAudioQueueSize() > 0) {
            long long nowTime = xxAVTime::getTime();
            long long dTime = nowTime - startTime + frame_duration + (long long)(seekTime * 1000) - sleepCountTime;

            if (audioFrame == nullptr)
            {
                audioQueue.pop(&audioFrame);
            }

            if (audioFrame != nullptr)
            {
                if (audioFrame->getPts() < (long long)(seekTime * 1000))
                {
                    delete audioFrame;
                    audioFrame = nullptr;
                }
            }
            
    
            if (audioFrame != nullptr && flag == 1){ 
                if ( audioFrame->getPts() <= dTime)
                {
                    // printf("audioFrame->getPts() %lld \n",audioFrame->getPts());
                    // printf("xxAVTime::getTime() - startTime + frame_duration : %lld \n",xxAVTime::getTime() - startTime + frame_duration);
                    audioFrame->getAudioData(dataTemp);
                    // audioFrame->savePCMData(pcmFile);
                    memcpy(dynamicBuffer + offset, dataTemp[0], frameSize);
                    offset += frameSize;

                    delete audioFrame;
                    audioFrame = nullptr;

                    if (dataTemp != nullptr) {
                        ++count;
                        dataTemp = nullptr;
                    }
                }
            }else if (audioFrame != nullptr && flag == 0){
                    audioFrame->getAudioData(dataTemp);
                    memcpy(dynamicBuffer + offset, dataTemp[0], frameSize);
                    offset += frameSize;
                    

                    delete audioFrame;
                    audioFrame = nullptr;

                    if (dataTemp != nullptr) {
                        ++count;
                        dataTemp = nullptr;
                    }
            }
        }
    }

    *data = dynamicBuffer;  // 返回动态分配的内存
    return 0;
}

void xxPlayerAudioThread::run(){

    int channel = 0;
    int samples = 0;
    int sample_rate = 0;
    int flag = 0; // 1 是 AV_SAMPLE_FMT_S16P  2是 AV_SAMPLE_FMT_S16
    double play_duration = 0.0;

    // 获取1帧来查询 帧字节大小，channel，位深度，采样频率
    xxAVFrame* audioFrameOne = nullptr;
    if (getAudioQueueSize()>0)
    {
        audioQueue.popOne(&audioFrameOne);
        audioFrameOne->getAudioInfo(channel,samples,sample_rate,flag,play_duration);

        // 内存空间不清楚，后面的要用，只要重置地址就可
        audioFrameOne = nullptr;
    }

    ALenum alFormat = (channel == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

    const int frameSize = samples * channel * sizeof(int16_t);  // 取每帧音频的字节数，可能根据音频格式而不同
    const int numFrames = 1;            // 每个缓冲区期望缓存的帧数，可根据需要调整
    const int BUFFER_SIZE = frameSize * numFrames; // 动态计算缓冲区大小
    const int NUM_BUFFERS = 2;
    const long long frame_duration = (long long)(play_duration * 1000);

    printf("frameSize: %d, numFrames: %d, channel: %d, samples: %d,sample_rate: %d , flags: %d play_duration : %lld",frameSize,numFrames,channel,samples,sample_rate,flag,frame_duration);


    // ----------------------OPENAL
    // 初始化 OpenAL 设备
    ALCdevice* device = alcOpenDevice(nullptr);  // 打开默认设备
    if (!device) {
        std::cerr << "Failed to open OpenAL device!" << std::endl;
    }

    ALCcontext* context = alcCreateContext(device, nullptr);
    if (!context) {
        std::cerr << "Failed to create OpenAL context!" << std::endl;
        alcCloseDevice(device);
    }

    alcMakeContextCurrent(context);

    // 创建源和缓冲区
    ALuint source;
    alGenSources(1, &source);

    ALuint buffers[NUM_BUFFERS];
    alGenBuffers(NUM_BUFFERS, buffers);

    for (int i = 0; i < NUM_BUFFERS; ++i) {

        uint8_t* data = nullptr;  // 根据动态计算的缓冲区大小分配内存
        
        loadAudioData(&data,frameSize,numFrames,frame_duration,0);          // 加载音频数据

        alBufferData(buffers[i], alFormat, data, BUFFER_SIZE, sample_rate);
 
        alSourceQueueBuffers(source, 1, &buffers[i]);

       // 数据已经复制到 OpenAL 缓冲区，安全释放数据
       free(data);  // 释放 data 内存
    }

    // 开始播放音频
    alSourcePlay(source);


    while(!stopFlag){
        
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));

        long long sleepTimeStart = xxAVTime::getTime();
        while (status  == xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PAUSEING)
        {
           std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        long long sleepTimeEnd = xxAVTime::getTime();

        long long sleepDtime = sleepTimeEnd - sleepTimeStart;
        sleepCountTime  = sleepCountTime + sleepDtime;
        

        ALint processed;
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

        // 处理已播放完的缓冲区
        while (processed > 0) {
            
            ALuint buffer;
            alSourceUnqueueBuffers(source, 1, &buffer);
 
            // 填充新的音频数据
            uint8_t* data = nullptr;  // 根据动态计算的缓冲区大小分配内存
            loadAudioData(&data,frameSize,numFrames,frame_duration,1);          // 加载音频数据
            alBufferData(buffer, alFormat, data, BUFFER_SIZE, sample_rate);

            // 将缓冲区重新入队
            alSourceQueueBuffers(source, 1, &buffer);
            --processed;
        }

        
        // 确保源处于播放状态
        ALint state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        if (state != AL_PLAYING) {
            alSourcePlay(source); // 如果源停止了，重新播放
        }

        // 控制更新频率，避免过于频繁
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
    }


    // 结束时清理资源
    alDeleteSources(1, &source);
    alDeleteBuffers(NUM_BUFFERS, buffers);
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);

}