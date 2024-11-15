cmake_minimum_required(VERSION 3.29.3)

include_directories(..)
include_directories(D:/dulred/dev/3rdlibs/done/ffmpeg-n4.4.2-95-ga8f16d4eb4-win64-gpl-shared-4.4/include)


add_library(xxAV STATIC 
xxAVReader.cpp
xxAVPacket.cpp 
xxAVDecoder.cpp
xxAVStream.cpp
xxAVFrame.cpp
xxAVTime.cpp
)

target_link_libraries(xxAV avcodec avdevice avformat avutil swresample swscale ) 
