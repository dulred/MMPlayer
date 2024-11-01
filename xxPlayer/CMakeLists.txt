cmake_minimum_required(VERSION 3.29.3)
include_directories(../)
add_library(xxPlayer STATIC xxPLayer.cpp xxPlayer.h xxPlayerReaderThread.cpp xxPlayerDecoderThread.cpp xxPlayerCtr.cpp)
target_link_libraries(xxPlayer xxThread xxAV)
