#include "ThreadPool.h"
#include <stdexcept>

ThreadPool::ThreadPool(size_t numThreads) : stop(false), tasksRemaining(0){
    for (size_t i = 0; i < numThreads; ++i){
        workers.emplace_back([this]{
            while(true){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    this->condition.wait(lock, [this] {return this->stop || !this->tasks.empty(); });
                    if(this->stop && this->tasks.empty()){
                        return;
                    }
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                    --tasksRemaining;
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool(){
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers){
        worker.join();
    }
}
     

void ThreadPool::enqueue(std::function<void()> task){
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        if(stop){
            throw std::runtime_error("enqueue on stopped threadpool");
        }
        tasks.emplace(std::move(task));
        ++tasksRemaining;
        
    }
    condition.notify_one();
    
}