#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include <atomic>

class ThreadPool{
    public:
        explicit ThreadPool(size_t numThreads);
        ~ThreadPool();

        void enqueue(std::function<void()> task);

    private:
        //workers
        std::vector<std::thread> workers;
        //Tasks
        std::queue<std::function<void()>> tasks;

        //Synchronization
        std::mutex queueMutex;
        std::condition_variable condition;
        bool stop;
        size_t tasksRemaining;

};

#endif