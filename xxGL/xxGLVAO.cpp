#include "xxGL.hpp"

xxGLVAO::xxGLVAO(){
    glGenVertexArrays(1, &vao);
}
xxGLVAO::~xxGLVAO(){
    for (int i = 0; i < vboList.size(); i++)
    {
        GLuint vbo = vboList[i];
        glDeleteBuffers(1, &vbo);
    }
    vboList.clear();

    if (ebo != 0)
    {
        glDeleteBuffers(1, &ebo); //
        ebo = 0;
    }
    if (vao != NULL)
    {
        glDeleteVertexArrays(1, &vao); //
        vao = NULL;
    }

}


int xxGLVAO::addVertex3D(float * data,int vertexCount,int layout){
    glBindVertexArray(vao);
    GLuint vbo = 0;
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), data, GL_STATIC_DRAW);
    glVertexAttribPointer(layout,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(GLvoid *) 0);
    glEnableVertexAttribArray(layout);

    vboList.push_back(vbo);

    glBindVertexArray(0);

    return 0;
}





 int xxGLVAO::setIndex(unsigned int * indexData, int indexCount){
    glBindVertexArray(vao);
    glGenBuffers(1,&ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indexData, GL_STATIC_DRAW);

    glBindVertexArray(0);

    drawTime = indexCount;

    return 0;
 }

int xxGLVAO::bindVao(){
    glBindVertexArray(vao);
    return 0;
}


int xxGLVAO::draw(){

    bindVao();
    glDrawElements(GL_TRIANGLES,drawTime,GL_UNSIGNED_INT,0);
    return 0;
 }


