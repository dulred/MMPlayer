#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <queue>
#include <atomic>
#include <iostream>
#include <chrono>
#include <xxQueue/xxQueue.h>
#include "xxPlayer/xxPlayer.h"

std::atomic<bool> running(true);

// 纹理队列，用于主线程接收来自渲染线程的数据
std::queue<GLuint> videoTextureQueue;
std::queue<GLuint> uiTextureQueue;

// 创建共享上下文的另一个窗口
GLFWwindow* mainWindow = nullptr;
GLFWwindow* sharedWindow = nullptr;

// 主线程渲染使用的纹理
GLuint uiTexture = 0;
GLuint videoTexture = 0;


int windowWidth = 1280;
int windowHeight = 720;

// 视频渲染线程
void* videoThreadFunc() {
    // 绑定共享上下文
    glfwMakeContextCurrent(sharedWindow);

    while (running) {
        // 模拟生成视频纹理
        GLuint newVideoTexture;
        glGenTextures(1, &newVideoTexture);
        glBindTexture(GL_TEXTURE_2D, newVideoTexture);
        // 填充纹理数据（模拟视频帧）
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        // 将视频纹理传递给主线程
        videoTextureQueue.push(newVideoTexture);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // 模拟解码时间
    }
    
    glfwMakeContextCurrent(nullptr);  // 解绑上下文
    return nullptr;
}

// UI 渲染线程
void* uiThreadFunc() {
    // 绑定共享上下文
    glfwMakeContextCurrent(sharedWindow);
    
    while (running) {
        // 模拟生成 UI 纹理
        GLuint newUITexture;
        glGenTextures(1, &newUITexture);
        glBindTexture(GL_TEXTURE_2D, newUITexture);
        // 填充纹理数据（模拟UI更新）
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        // 将 UI 纹理传递给主线程
        uiTextureQueue.push(newUITexture);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // 模拟UI刷新时间
    }

    glfwMakeContextCurrent(nullptr);  // 解绑上下文
    return nullptr;
}

// 渲染函数
void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 渲染 UI 纹理
    if (!uiTextureQueue.empty()) {
        uiTexture = uiTextureQueue.front();
        uiTextureQueue.pop();
        glBindTexture(GL_TEXTURE_2D, uiTexture);
        // 绘制 UI 纹理（省略实际绘制代码）
    }

    // 渲染视频纹理
    if (!videoTextureQueue.empty()) {
        videoTexture = videoTextureQueue.front();
        videoTextureQueue.pop();
        glBindTexture(GL_TEXTURE_2D, videoTexture);
        // 绘制视频纹理（省略实际绘制代码）
    }

    glfwSwapBuffers(mainWindow);  // 交换缓冲区
}

// 主线程渲染和事件循环
void mainLoop() {
    while (!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();  // 处理事件

        render();  // 渲染 UI 和视频

        std::this_thread::sleep_for(std::chrono::milliseconds(16));  // 控制帧率，假设 60 FPS
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // 根据新的窗口宽高调整视口大小
    glViewport(0, 0, width, height);
}


// 键盘回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_UP) {
            std::cout << "Up arrow key pressed!" << std::endl;
        } else if (key == GLFW_KEY_DOWN) {
            std::cout << "Down arrow key pressed!" << std::endl;
        } else if (key == GLFW_KEY_LEFT) {
            std::cout << "Left arrow key pressed!" << std::endl;
        } else if (key == GLFW_KEY_RIGHT) {
            std::cout << "Right arrow key pressed!" << std::endl;
        }
    }
}

// 鼠标位置回调函数
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

    // 将窗口坐标转换为 NDC 坐标
    float x_ndc = (2.0f * xpos) / windowWidth - 1.0f;
    float y_ndc = 1.0f - (2.0f * ypos) / windowHeight;

    // 打印鼠标位置 (可选，调试用)
    std::cout << "Mouse Position: (" << x_ndc << ", " << y_ndc << ")" << std::endl;

    // std::cout << "Mouse moved to position (" << xpos << ", " << ypos << ")" << std::endl;
}

// 鼠标滚轮回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    std::cout << "Mouse wheel scroll - xoffset: " << xoffset << ", yoffset: " << yoffset << std::endl;
}

// 鼠标按键回调函数
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            std::cout << "Left mouse button pressed" << std::endl;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            std::cout << "Right mouse button pressed" << std::endl;
        } else if (button == GLFW_MOUSE_BUTTON_4) {
            std::cout << "Mouse button 4 pressed!" << std::endl;
        }else if (button == GLFW_MOUSE_BUTTON_5) {
            std::cout << "Mouse button 5 pressed!" << std::endl;
        }
    } else if (action == GLFW_RELEASE) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            std::cout << "Left mouse button released" << std::endl;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            std::cout << "Right mouse button released" << std::endl;
        }
    }

}


int main() {
    // -----------------OPENGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);   
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);      
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    // 主线程（创建窗口和共享上下文）
    GLFWwindow* mainWindow = glfwCreateWindow(windowWidth, windowHeight, "Shared Context", NULL, NULL);
    if (!mainWindow) {
        std::cerr << "Failed to create shared OpenGL window!" << std::endl;
        glfwTerminate();
        return -1;
    }


    // OpenGL 运行模式--单线程
    glfwMakeContextCurrent(mainWindow); 
    glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);  // 注册窗口大小变化的回调
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);


    // 设置键盘回调函数
    glfwSetKeyCallback(mainWindow, key_callback);

     // 设置鼠标回调函数
    glfwSetMouseButtonCallback(mainWindow, mouse_button_callback);
    glfwSetCursorPosCallback(mainWindow, cursor_position_callback);
    glfwSetScrollCallback(mainWindow, scroll_callback);



// 顶点着色器源码（用于左矩形）
const char* vertexShaderSourceLeft = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;  // 输入顶点位置

    void main() {
        gl_Position = vec4(aPos, 1.0);  // 输出顶点位置到裁剪坐标系
    }
)";

// 片段着色器源码（用于左矩形）
const char* fragmentShaderSourceLeft = R"(
    #version 330 core
    out vec4 FragColor;  // 输出片段颜色

    uniform sampler2D ourTexture;

    void main() {
        FragColor = texture(ourTexture, vec2(0.5, 0.5));  // 获取纹理颜色
    }
)";

// 顶点着色器源码（用于视频拦）
const char* vertexShaderSourceVideo = R"(


    #version 330
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec2 aTexCoord;
    out vec2 TexCoord;

    void main(){
        gl_Position = vec4(aPos,1.0); //将顶点位置传递到 GLSL
        TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
    }
)";

// 片段着色器源码（用于视频栏）
const char* fragmentShaderSourceVideo = R"(

    #version 330
    out vec4 fragColor;
    in vec2 TexCoord;

    uniform sampler2D texture1;

    void main(){
        fragColor = texture(texture1, TexCoord);
    }

)";




// 顶点着色器源码（用于右矩形）
const char* vertexShaderSourceRight = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;  // 输入顶点位置

    void main() {
        gl_Position = vec4(aPos, 1.0);  // 输出顶点位置到裁剪坐标系
    }
)";

// 片段着色器源码（用于右矩形）
const char* fragmentShaderSourceRight = R"(
    #version 330 core
    out vec4 FragColor;  // 输出片段颜色

    uniform sampler2D ourTexture;

    void main() {
        FragColor = texture(ourTexture, vec2(0.5, 0.5));  // 获取纹理颜色
    }
)";

// 创建和编译左侧矩形的着色器
unsigned int vertexShaderLeft = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexShaderLeft, 1, &vertexShaderSourceLeft, NULL);
glCompileShader(vertexShaderLeft);

unsigned int fragmentShaderLeft = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShaderLeft, 1, &fragmentShaderSourceLeft, NULL);
glCompileShader(fragmentShaderLeft);

unsigned int shaderProgramLeft = glCreateProgram();
glAttachShader(shaderProgramLeft, vertexShaderLeft);
glAttachShader(shaderProgramLeft, fragmentShaderLeft);
glLinkProgram(shaderProgramLeft);


// 创建和编译视频栏的着色器
unsigned int vertexShaderVideo = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexShaderVideo, 1, &vertexShaderSourceVideo, NULL);
glCompileShader(vertexShaderVideo);

unsigned int fragmentShaderVideo = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShaderVideo, 1, &fragmentShaderSourceVideo, NULL);
glCompileShader(fragmentShaderVideo);

unsigned int shaderProgramVideo = glCreateProgram();
glAttachShader(shaderProgramVideo, vertexShaderVideo);
glAttachShader(shaderProgramVideo, fragmentShaderVideo);
glLinkProgram(shaderProgramVideo);


// 创建和编译右侧矩形的着色器
unsigned int vertexShaderRight = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexShaderRight, 1, &vertexShaderSourceRight, NULL);
glCompileShader(vertexShaderRight);

unsigned int fragmentShaderRight = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShaderRight, 1, &fragmentShaderSourceRight, NULL);
glCompileShader(fragmentShaderRight);

unsigned int shaderProgramRight = glCreateProgram();
glAttachShader(shaderProgramRight, vertexShaderRight);
glAttachShader(shaderProgramRight, fragmentShaderRight);
glLinkProgram(shaderProgramRight);




// 顶点数据：包含两个矩形的坐标
float verticesLeft[] = {
    -1.0f, 1.0f, 0.0f,  // 左上
    -1.0f, 0.9f, 0.0f,  // 左下
    1.0f, 1.0f, 0.0f,  // 右上
    1.0f, 0.9f, 0.0f  // 右下
};

float verticesVideo[] = {
     -1.0f,  0.9f, 0.0f,  0.0f, 1.0f,  // 左上角
     -1.0f, -0.75f, 0.0f, 0.0f, 0.0f,  // 左下角
     1.0f, 0.9f, 0.0f,    1.0f, 1.0f,  // 右上角
     1.0f, -0.75f, 0.0f,  1.0f, 0.0f,  // 右下角
};

float verticesRight[] = {
     -1.0f,  -0.75f, 0.0f,  // 左上
     -1.0f, -1.0f, 0.0f,  // 左下
     1.0f, -0.75f, 0.0f,   // 右上
     1.0f, -1.0f, 0.0f  // 右下
};


// 索引数据：每个矩形单独的索引
unsigned int indicesLeft[] = {
    0, 1, 3,  0, 2, 3   // 左侧矩形
};

unsigned int indicesVideo[] = {
     0, 1, 3,  0, 2, 3  // 右侧小矩形
};

unsigned int indicesRight[] = {
     0, 1, 3,  0, 2, 3  // 右侧小矩形
};


// 创建 VAO, VBO 和 EBO
unsigned int VAOLeft, VAORight,VAOVideo, VBOLeft, VBORight,VBOVideo, EBOLeft, EBORight,EBOVideo;
glGenVertexArrays(1, &VAOLeft);
glGenVertexArrays(1, &VAORight);
glGenVertexArrays(1, &VAOVideo);
glGenBuffers(1, &VBOLeft);
glGenBuffers(1, &VBORight);
glGenBuffers(1, &VBOVideo);
glGenBuffers(1, &EBOLeft);
glGenBuffers(1, &EBORight);
glGenBuffers(1, &EBOVideo);

// 绘制左侧矩形
glBindVertexArray(VAOLeft);
glBindBuffer(GL_ARRAY_BUFFER, VBOLeft);
glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLeft), verticesLeft, GL_STATIC_DRAW);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOLeft);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesLeft), indicesLeft, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// 绘制视频栏矩形
glBindVertexArray(VAOVideo);
glBindBuffer(GL_ARRAY_BUFFER, VBOVideo);
glBufferData(GL_ARRAY_BUFFER, sizeof(verticesVideo), verticesVideo, GL_STATIC_DRAW);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOVideo);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesVideo), indicesVideo, GL_STATIC_DRAW);

// 位置属性
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// 纹理坐标属性
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);

// 绘制右侧矩形
glBindVertexArray(VAORight);
glBindBuffer(GL_ARRAY_BUFFER, VBORight);
glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRight), verticesRight, GL_STATIC_DRAW);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBORight);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesRight), indicesRight, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// 解绑 VAO, VBO 和 EBO
glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);

// 加载纹理前设置对齐方式
// glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

// 创建纹理
unsigned int textureLeft, textureRight,textureVideo;
glGenTextures(1, &textureLeft);
glGenTextures(1, &textureRight);
glGenTextures(1, &textureVideo);


// 视频栏的红色纹理
glBindTexture(GL_TEXTURE_2D, textureLeft);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
unsigned char redTexture[] = {255, 0, 0, 255};  // 红色
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, redTexture);


// 菜单栏的红色纹理
glBindTexture(GL_TEXTURE_2D, textureVideo);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

// 控制栏的蓝色纹理
glBindTexture(GL_TEXTURE_2D, textureRight);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
unsigned char blueTexture[] = {0, 0, 255, 255};  // 蓝色
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, blueTexture);

// 解绑纹理
glBindTexture(GL_TEXTURE_2D, 0);

xxQueue<unsigned char> renderQueue;

unsigned char* pic = nullptr;

xxplayer player("D:/videos/ss.mp4");

int ret = player.open((double)0.0,renderQueue);

if (ret)
{
    printf("Player open fail \n");
    return -1;
}

// 渲染循环
while (!glfwWindowShouldClose(mainWindow)) {

    // 处理输入事件
    glfwPollEvents();

    // 清空颜色缓冲
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 使用左侧矩形的着色器程序
    glUseProgram(shaderProgramLeft);
    glBindTexture(GL_TEXTURE_2D, textureLeft);  // 绑定红色纹理
    glBindVertexArray(VAOLeft);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);  // 绘制左边的矩形

    // 使用视频栏的着色器程序
    glUseProgram(shaderProgramVideo);
    glBindTexture(GL_TEXTURE_2D, textureVideo);  // 绑定已生成的纹理对象
    if (renderQueue.size() > 0)
    {
        renderQueue.pop(&pic);
    }
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, pic);
    glGenerateMipmap(GL_TEXTURE_2D);  // 如果你需要更新多级纹理
    glBindVertexArray(VAOVideo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);  // 绘制视频栏的小矩形


    // 使用右侧矩形的着色器程序
    glUseProgram(shaderProgramRight);
    glBindTexture(GL_TEXTURE_2D, textureRight);  // 绑定蓝色纹理
    glBindVertexArray(VAORight);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);  // 绘制右边的小矩形


    // 交换缓冲区
    glfwSwapBuffers(mainWindow);

// 保证有一帧数据在
    if (renderQueue.size() > 0)
    {
        if (pic!=nullptr) {
            delete[] pic;
            pic = nullptr;
        }
    }

}


// 清理资源
glDeleteVertexArrays(1, &VAOLeft);
glDeleteVertexArrays(1, &VAORight);
glDeleteVertexArrays(1, &VAOVideo);
glDeleteBuffers(1, &VBOLeft);
glDeleteBuffers(1, &VBORight);
glDeleteBuffers(1, &VBOVideo);
glDeleteBuffers(1, &EBOLeft);
glDeleteBuffers(1, &EBORight);
glDeleteBuffers(1, &EBOVideo);
glDeleteTextures(1, &textureLeft);
glDeleteTextures(1, &textureRight);
glDeleteTextures(1, &textureVideo);
glDeleteProgram(shaderProgramLeft);
glDeleteProgram(shaderProgramRight);
glDeleteProgram(shaderProgramVideo);
glfwDestroyWindow(mainWindow);
glfwDestroyWindow(sharedWindow);
glfwTerminate();

return 0;

}
