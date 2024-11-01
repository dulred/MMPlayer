#include "xxThread.h"
#include <thread>

int xxThread::start()
{
    if (t == nullptr)
    {
        stopFlag = 0;
        t = new std::thread(&xxThread::run,this);
    }
    return 0;
}


int xxThread::stop()
{
    if (t != nullptr)
    {
        stopFlag = 1;
        t->join();
        delete t;
        t = nullptr;
    }
    return 0;
}