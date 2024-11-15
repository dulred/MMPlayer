#include "xxPlayer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "xxGL/xxGL.hpp"
#include <al.h>
#include <alc.h>
#include "xxThreadPool/xxThreadPool.h"

xxPlayerCtr::xxPlayerCtr(std::string _path,double _seekTime){
    seekTime = _seekTime;
    path = _path;
}

xxPlayerCtr::~xxPlayerCtr(){

}

int xxPlayerCtr::play(){
    status = xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PLAYING;
    return 0;
}
int xxPlayerCtr::pause(){
    status = xxPlayerCtrStatus::XXPLAYER_CTR_STATUS_PAUSEING;
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // 根据新的窗口宽高调整视口大小
    glViewport(0, 0, width, height);
}


void processVideoFrame(xxAVFrame*& videoFrame, long long& dTime, GLuint& texture, int& width, int& height,xxGLProgram*& program,
    unsigned int& VAO, GLFWwindow*& window) {

            glfwMakeContextCurrent(window);  // 将渲染线程绑定到窗口的上下文
            // 执行可能抛出异常的代码
            //这帧视频，应该立即播放出来
            unsigned char* pictureData = nullptr;

            videoFrame->getPictureData(pictureData);
            // for (int i = 0; i < 5; ++i) {
            //     printf(" 0x%02X ",pictureData[i]);
            // }

            glBindTexture(GL_TEXTURE_2D, texture);  // 绑定已生成的纹理对象
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pictureData);
            glGenerateMipmap(GL_TEXTURE_2D);  // 如果你需要更新多级纹理
            // TODO 绘制操作
            glClear(GL_COLOR_BUFFER_BIT);

            program->UseProgram();
            // 渲染矩形
            glBindVertexArray(VAO);


            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            // TODO
            glfwSwapBuffers(window);
            glfwPollEvents();


            delete[] pictureData;
            // delete videoFrame;
            // videoFrame = nullptr;


}

void processAudioFrame(xxAVFrame*& audioFrame,ALuint& bufferID) {

                
               //这帧音频，应该立即播放出来
            //   audioFrame->audioPrint();
            uint8_t** data  = nullptr;
            int channel = 0;
            int samples = 0;
            int sample_rate = 0;
            int flag = 0; // 1 是 AV_SAMPLE_FMT_S16P  2是 AV_SAMPLE_FMT_S16
            audioFrame->getAudioData(data,channel,samples,sample_rate,flag);
            
            // printf("channel=%d samples=%d sample_rate=%d flag=%d", channel, samples, sample_rate,flag);
            // 确定音频数据格式
            ALenum alFormat = (channel == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;


            // 如果数据格式是平面格式（每个声道分开存储）
            if (flag == 1) {
                // 为每个声道分配缓冲区并填充数据
                std::vector<int16_t> pcmData(samples * channel);
                
                // 将每个声道的数据组合成一个连续的数组（交错存储）
                for (int i = 0; i < samples; ++i) {
                    for (int ch = 0; ch < channel; ++ch) {
                        pcmData[i * channel + ch] = ((int16_t*)data[ch])[i];  // 16 位整型样本数据
                    }
                }

                // 将 PCM 数据加载到 OpenAL 缓冲区
                alBufferData(bufferID, alFormat, pcmData.data(), pcmData.size() * sizeof(int16_t), sample_rate);
            } else if (flag == 2) {             
                // 非平面格式，直接将数据加载到 OpenAL 缓冲区
                alBufferData(bufferID, alFormat, data[0], samples * channel * sizeof(int16_t), sample_rate);
                // printf(" 0x%04X ",data[0][0]);
            }

            // 创建 OpenAL 源
            ALuint sourceID;
            alGenSources(1, &sourceID);
            alSourcei(sourceID, AL_BUFFER, bufferID);

            alSourcePlay(sourceID);


            // 播放音频，等待播放结束
            ALint state;
            do {
                alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
            } while (state == AL_PLAYING);
            delete audioFrame;   
            audioFrame = nullptr;
            alDeleteSources(1, &sourceID);

}



void xxPlayerCtr::run(){

    // -----------------OPENGL
   // 打开输出文件
    // FILE* pcmFile = fopen("D:/videos/output.pcm", "wb");
    // if (!pcmFile) {
    //     printf( "Could not open output file\n");
    // }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);   
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    int width  = 1280;
    int height = 720;

    int windowWidth = width;
    int windowHeight = height;


    // 主线程（创建窗口和共享上下文）
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Shared Context", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create shared OpenGL window!" << std::endl;
        exit(EXIT_FAILURE);
    }


    // OpenGL 运行模式--单线程
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // 注册窗口大小变化的回调
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);


    char * vertexShader = (char *)R"(
        #version 330
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec2 aTexCoord;
        out vec2 TexCoord;

        void main(){
            gl_Position = vec4(aPos,1.0); //将顶点位置传递到 GLSL
            TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
        }
    
    )";
    
    char * fragmentShader = (char *)R"(
            #version 330
            out vec4 fragColor;
            in vec2 TexCoord;

            uniform sampler2D texture1;

            void main(){
                fragColor = texture(texture1, TexCoord);
            }
    )";

    xxGLProgram* program = new xxGLProgram(vertexShader,fragmentShader);

    float vertices[] = {
        // 位置              // 纹理坐标
        1.0f,  1.0f, 0.0f,    1.0f, 1.0f,  // 右上角
        1.0f, -1.0f, 0.0f,    1.0f, 0.0f,  // 右下角
        -1.0f, -1.0f, 0.0f,    0.0f, 0.0f,  // 左下角
        -1.0f,  1.0f, 0.0f,    0.0f, 1.0f   // 左上角
    };

    unsigned int indices[] = {
        0, 1, 3,  // 第一个三角形
        1, 2, 3   // 第二个三角形
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 纹理坐标属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    xxPlayerReaderThread readerThread(path,seekTime,this);
    readerThread.start();

    // 获取线程启动时候的时间 start_time
    long long startTime  = xxAVTime::getTime();

    xxAVFrame* videoFrame = nullptr;
    xxAVFrame* audioFrame = nullptr;

    long long sleepCountTime = 0;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // 设置纹理的环绕和过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载纹理前设置对齐方式
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // 加载纹理数据到 OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glGenerateMipmap(GL_TEXTURE_2D);  // 生成多级纹理



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
               

    // 假设已经在音频初始化时创建了缓冲区和源
    ALuint bufferID;
    // 只需要创建一次缓冲区和源
    alGenBuffers(1, &bufferID);
    // 播放音频，等待播放结束
    ALint state;
    ALuint sourceID;

    // ThreadPool videoThreadPool(1);  // 为视频渲染分配两个线程
    // ThreadPool audioThreadPool(1);  // 为音频播放分配两个线程


    std::vector<uint8_t*> audioBufferQueue; // 用于缓存音频数据
    const int MAX_BUFFER_SIZE = 200; // 每次缓存的最大帧数（可以根据需要调整）

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
        
        
        //获取当前的时间 now_time
        long long nowTime = xxAVTime::getTime();
        
        //获取到当前时间和开始时间的差值d_time
        long long dTime = nowTime - startTime;

        dTime = dTime - sleepCountTime;
        dTime = dTime + (long long)(seekTime * 1000);


        // printf("DTime: %lld\n",dTime);

        // 从视频缓存队列中，获取一帧视频frame_pts
        // if (videoFrame == nullptr)
        // {
        //     // 尝试取一帧出来
        //     videoQueue.pop(&videoFrame);
        // }

        // if (videoFrame != nullptr)
        // {
        //     if (videoFrame->getPts() < (long long)(seekTime * 1000))
        //     {
        //         delete videoFrame;
        //         videoFrame = nullptr;
        //     }
            
        // }

        // if (videoFrame != nullptr)
        // {
         
        //     // 如果frame_pts <= d_time
        //     if (videoFrame->getPts() <= dTime)
        //     {


        //         // videoThreadPool.enqueue([&videoFrame, &dTime,&texture,&width,&height,&program,&VAO,&window]() -> void {
        //         //     processVideoFrame(videoFrame,dTime,texture,width,height,program,VAO,window);
        //         // });

        //                 //这帧视频，应该立即播放出来
        //                 unsigned char* pictureData = nullptr;

        //                 videoFrame->getPictureData(pictureData);
        //                 // for (int i = 0; i < 5; ++i) {
        //                 //     printf(" 0x%02X ",pictureData[i]);
        //                 // }

        //                 glBindTexture(GL_TEXTURE_2D, texture);  // 绑定已生成的纹理对象
        //                 glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pictureData);
        //                 glGenerateMipmap(GL_TEXTURE_2D);  // 如果你需要更新多级纹理
        //                 // TODO 绘制操作
        //                 glClear(GL_COLOR_BUFFER_BIT);

        //                 program->UseProgram();
        //                 // 渲染矩形
        //                 glBindVertexArray(VAO);


        //                 glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //                 // TODO
        //                 glfwSwapBuffers(window);
        //                 glfwPollEvents();


        //                 delete[] pictureData;
        //                 delete videoFrame;
        //                 videoFrame = nullptr;

        //     }
        //     else
        //     {
        //          //这帧视频还不到播放的时候，程序进行自旋，或者去处理音频
        //     }
            
        // }
        
        
        // 从音频缓存队列中，获取一帧音频frame_pts
        if (audioFrame == nullptr)
        {
            // 尝试取一帧出来
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

        if (audioFrame != nullptr)
        {
            // 如果frame_pts <= d_time
            if (audioFrame->getPts() <= dTime)
            {

            // 将获取到的每一帧 PCM 数据保存到文件中
                // audioFrame->savePCMData(pcmFile);
                // audioThreadPool.enqueue([&audioFrame,&bufferID]() -> void {
                //     processAudioFrame(audioFrame,bufferID);
                // });
            // audioFrame->audioPrint();
            //这帧音频，应该立即播放出来
            uint8_t** data  = nullptr;
            int channel = 0;
            int samples = 0;
            int sample_rate = 0;
            int flag = 0; // 1 是 AV_SAMPLE_FMT_S16P  2是 AV_SAMPLE_FMT_S16
      

            audioFrame->getAudioData(data,channel,samples,sample_rate,flag);


            // printf("channel=%d samples=%d sample_rate=%d flag=%d", channel, samples, sample_rate,flag);
            // 确定音频数据格式
            ALenum alFormat = (channel == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
            // 生成 OpenAL 缓冲区并填充 PCM 数据

            const int bufferSize = samples * channel * sizeof(int16_t);  // 计算缓冲区大小

            // 如果数据格式是平面格式（每个声道分开存储）
            if (flag == 1) {
                // 为每个声道分配缓冲区并填充数据
                std::vector<int16_t> pcmData(samples * channel);
                
                // 将每个声道的数据组合成一个连续的数组（交错存储）
                for (int i = 0; i < samples; ++i) {
                    for (int ch = 0; ch < channel; ++ch) {
                        pcmData[i * channel + ch] = ((int16_t*)data[ch])[i];  // 16 位整型样本数据
                    }
                }

                printf("hello");

                // 将 PCM 数据加载到 OpenAL 缓冲区
                alBufferData(bufferID, alFormat, pcmData.data(), pcmData.size() * sizeof(int16_t), sample_rate);
            } else if (flag == 2) {             


                // alBufferData(bufferID, alFormat, data[0], samples * channel * sizeof(int16_t), sample_rate);
                  // 如果是非平面格式，直接将数据添加到队列
                if (data[0] != nullptr) {
                    audioBufferQueue.push_back(data[0]);
                }
                // printf(" 0x%04X ",data[0][0]);
            }

            // 将数据缓冲区加入队列
            audioBufferQueue.push_back(data[0]);
            
            // 如果缓冲区内的数据已经足够，可以进行播放

           // 如果队列中的数据已经足够播放（大于 bufferThreshold）
            if (audioBufferQueue.size() >= MAX_BUFFER_SIZE) {
                // 计算缓冲区的总大小
                int totalBufferSize = 0;
                for (auto& buffer : audioBufferQueue) {
                    totalBufferSize += bufferSize;
                }
                

                // 分配一个大缓冲区来保存所有音频数据
                std::vector<uint8_t> combinedData(totalBufferSize  / sizeof(uint8_t));

                // 将队列中的所有数据合并到一个大缓冲区中
                int offset = 0;
                for (auto& buffer : audioBufferQueue) {
                    memcpy(combinedData.data() + offset, buffer, bufferSize);
                    offset += bufferSize;
                }

         
                // 将合并后的数据一次性加载到 OpenAL 缓冲区
                alBufferData(bufferID, alFormat, combinedData.data(), combinedData.size() * sizeof(uint8_t), sample_rate);

                // 生成 OpenAL 源并播放
                alGenSources(1, &sourceID);
                alSourcei(sourceID, AL_BUFFER, bufferID);
                alSourcePlay(sourceID);
        


                do {
                    alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
                } while (state == AL_PLAYING);

                // 清理资源
                alDeleteSources(1, &sourceID);
                // 清空队列，准备下一次加载
                audioBufferQueue.clear();
            }

            // 清理当前的音频帧 
            delete audioFrame;
            audioFrame = nullptr;
            }
            else
            {
              //这帧音频还不到播放的时候，程序进行自旋
            }
            
        }

    }

    readerThread.stop();
    
    // 清理OPENGL资源
    delete program;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture);  // 删除旧纹理
    glfwTerminate();




    
    // 清理OPENAL资源
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
    alDeleteBuffers(1, &bufferID);

  // 关闭文件
    // fclose(pcmFile);

}

int xxPlayerCtr::getVideoQueueSize()
{
    return videoQueue.size();
}
int xxPlayerCtr::getAudioQueueSize()
{
    return audioQueue.size();
}
int xxPlayerCtr::pushFrameToVideoQueue(xxAVFrame* frame)
{
    return videoQueue.push(frame);
}
int xxPlayerCtr::pushFrameToAudioQueue(xxAVFrame* frame)
{
    return audioQueue.push(frame);
}