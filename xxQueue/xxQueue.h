#pragma once
#include<queue>
#include<mutex>
template<typename T>
class xxQueue
{
private:
    std::queue<T *> queue;
    std::mutex mut;
public:
    xxQueue(/* args */){

    }
    ~xxQueue(){
        std::lock_guard<std::mutex> lck(mut);
        while(size()>0){
            queue.pop();
        }
    }
    int push(T *t){
        std::lock_guard<std::mutex> lck(mut);
        queue.push(t);
        return 0;
    }
    int pop(T **t){
       std::lock_guard<std::mutex> lck(mut);
        int _size = size();
        if (_size>0){
            *t = queue.front();
            queue.pop();
            return 0;
        }
        return -1; 
    }

     int popOne(T **t){
       std::lock_guard<std::mutex> lck(mut);
        int _size = size();
        if (_size>0){
            *t = queue.front();
            return 0;
        }
        return -1; 
    }
    int size (){
        return queue.size();
    }
};
