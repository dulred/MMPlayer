
#include "xxGL.hpp"
#include <stdlib.h>
#include <stdio.h>
xxGLProgram::xxGLProgram(char * vertexShaderStr, char * fragmentShaderStr){
    program = glCreateProgram();
    xxGLShader vertexShader(vertexShaderStr, xxGLShaderType::XXGL_SHADER_VERTEX);
    xxGLShader fragmentShader(fragmentShaderStr, xxGLShaderType::XXGL_SHADER_FRAGMENT);

    glAttachShader(program, vertexShader.shader);
    glAttachShader(program, fragmentShader.shader);
    glLinkProgram(program);



    GLint logLen = 0;
    glGetProgramiv(program,GL_INFO_LOG_LENGTH,&logLen);
    if (logLen>0) 
    {
        char *log = (char *)malloc(logLen);
        glGetProgramInfoLog(program,logLen,NULL,log);

        printf("vertex program compile error:\n%s\n",log);
        free(log);
    }


}
int xxGLProgram::UseProgram(){
    glUseProgram(program);
    return 0;
}


xxGLProgram::~xxGLProgram(){
    if (program != NULL)
    {
        glDeleteProgram(program);
        program = 0;
    }
}
