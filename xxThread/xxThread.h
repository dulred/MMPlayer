#pragma once
#include <thread>
#include <iostream>
class xxThread 
{
    public:
        virtual void run() = 0;
        int start();
        int stop();
    public:
        std::thread* t = nullptr;
        int stopFlag = 0;
};