
#include "xxGL.hpp"
#include <stdlib.h>
#include <stdio.h>

xxGLShader::xxGLShader(char * shaderStr, xxGLShaderType type){
    if (type == xxGLShaderType::XXGL_SHADER_VERTEX)
    {
        shader = glCreateShader(GL_VERTEX_SHADER);
    }else if (type == xxGLShaderType::XXGL_SHADER_FRAGMENT)
    {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    }    
    else{
        shader = glCreateShader(GL_VERTEX_SHADER);
    }

    glShaderSource(shader,1,&shaderStr,NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&status);

    GLint logLen = 0;
    glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&logLen);
    if (logLen>0) 
    {
        char *log = (char *)malloc(logLen);
        glGetShaderInfoLog(shader,logLen,NULL,log);
        printf("vertex shader compile error:\n%s\n",log);
        free(log);
    }
    
}

xxGLShader::~xxGLShader(){
    if (shader != 0)
    {
        glDeleteShader(shader);
        shader = 0;
    }
    
}