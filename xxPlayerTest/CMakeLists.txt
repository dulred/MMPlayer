cmake_minimum_required(VERSION 3.29.3)
include_directories(../)
link_directories(D:/dev/3rdlibs/done/ffmpeg-n4.4.3-4-gd585a0a1cc-win64-gpl-shared-4.4/lib)

add_executable(xxPlayerTest main.cpp)
target_link_libraries(xxPlayerTest xxThread xxAV xxPlayer avcodec avdevice avformat avutil swresample swscale)
