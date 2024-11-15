#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
enum xxGLShaderType
{
    XXGL_SHADER_VERTEX = 1,
    XXGL_SHADER_FRAGMENT = 2
};

class xxGLShader 
{
    public:
        xxGLShader(char * shaderStr, xxGLShaderType type);
        ~xxGLShader();
    public:
        GLuint shader = 0;
};

class xxGLProgram
{
    public:
        xxGLProgram(char * vertexShaderStr, char * fragmentShaderStr);
        ~xxGLProgram();

        int UseProgram();
    public:
        GLuint program = 0;
};


class xxGLVAO
{
    public:
        xxGLVAO();
        ~xxGLVAO();
        int addVertex3D(float * data,int vertexCount,int layout);

        int setIndex(unsigned int * indexData, int indexCount);
        int bindVao();

        int draw();

    public:
        GLuint vao = 0;
        GLuint ebo = 0;
        int drawTime = 0;
        std::vector<GLuint> vboList;
};