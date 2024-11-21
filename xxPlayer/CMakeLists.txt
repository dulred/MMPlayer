cmake_minimum_required(VERSION 3.29.3)
include_directories(../)
include_directories(D:/dulred/dev/3rdlibs/done/glfw-3.4/include)
include_directories(D:/dulred/dev/3rdlibs/done/glad-3.3/include)
include_directories(D:/dulred/dev/3rdlibs/done/OpenAL1.1/include)

link_directories(D:/dulred/dev/3rdlibs/done/OpenAL1.1/libs/Win64)
link_directories(D:/dulred/dev/3rdlibs/done/glfw-3.4/lib)

add_library(xxPlayer STATIC xxPLayer.cpp xxPlayer.h xxPlayerReaderThread.cpp xxPlayerDecoderThread.cpp xxPlayerCtr.cpp xxPlayerRenderThread.cpp xxPlayerAudioThread.cpp xxPlayerPushRenderThread.cpp xxPlayerPushAudioThread.cpp)
target_link_libraries(xxPlayer xxThread xxAV glfw3 xxGL OpenAL32)
