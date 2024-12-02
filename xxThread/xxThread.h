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


// template<typename T>
// class ThreadSync {
// public:
//     ThreadSync() : done(false) {}

//     void notify(const T& result) {
//         std::lock_guard<std::mutex> lock(mutex);
//         this->result = result;
//         done = true;
//         cv.notify_one();
//     }

//     T waitForResult() {
//         std::unique_lock<std::mutex> lock(mutex);
//         cv.wait(lock, [this] { return done; });
//         return result;
//     }

// private:
//     bool done;
//     T result;
//     std::mutex mutex;
//     std::condition_variable cv;
// };



// class xxPlayerThreadA : public xxThread {
// public:
//     void run() override {
//         // 执行任务
//         result = someComputation();
//         sync.notify(result);  // 通知线程 B
//     }

// private:
//     ThreadSync<long long> sync;
//     long long result;
// };