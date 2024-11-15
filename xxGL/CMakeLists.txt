cmake_minimum_required(VERSION 3.30)

include_directories("D:/dulred/dev/3rdlibs/done/glfw-3.4/include")

include_directories("D:/dulred/dev/3rdlibs/done/glad-3.3/include")

link_directories("D:/dulred/dev/3rdlibs/done/glfw-3.4/lib")

add_library(xxGL "xxGL.hpp" "xxGLShader.cpp" "xxGLProgram.cpp" "xxGLVAO.cpp" "D:/dulred/dev/3rdlibs/done/glad-3.3/src/glad.c" )
