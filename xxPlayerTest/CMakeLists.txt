cmake_minimum_required(VERSION 3.29.3)
include_directories(../)
link_directories(D:/dulred/dev/3rdlibs/done/ffmpeg-n4.4.2-95-ga8f16d4eb4-win64-gpl-shared-4.4/lib)
link_directories(D:/dulred/dev/3rdlibs/done/OpenAL1.1/libs/Win64)
link_directories(D:/dulred/dev/3rdlibs/done/glfw-3.4/lib)

add_executable(xxPlayerTest main.cpp)
target_link_libraries(xxPlayerTest xxThread xxAV xxPlayer avcodec avdevice avformat avutil swresample swscale)
