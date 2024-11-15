#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.push_back(std::thread(&ThreadPool::workerThread, this));
        }
    }

    ~ThreadPool() {
        stop = true;
        condition.notify_all();  // Notify all threads to stop
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    template <typename Func>
    void enqueue(Func&& func) {
        std::lock_guard<std::mutex> lock(queueMutex);
        taskQueue.push(std::forward<Func>(func));
        condition.notify_one();
    }

private:
    void workerThread() {
        while (!stop) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                condition.wait(lock, [this] { return !taskQueue.empty() || stop; });

                if (stop && taskQueue.empty()) {
                    return;
                };
                task = std::move(taskQueue.front());
                taskQueue.pop();
            }
            task();

        }
    }

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};
