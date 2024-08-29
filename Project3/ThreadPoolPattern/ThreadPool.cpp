/*************************************************************************************
 * ThreadPool.cpp - Implementation of the ThreadPool class
 *************************************************************************************/
#include "ThreadPool.h"
#include <stdexcept>


/*************************************************************************************
 * ThreadPool (constructor) - Initializes the thread pool with a specified number of threads
 *
 * Parameters:
 *   numThreads - Number of threads to create
 *************************************************************************************/
ThreadPool::ThreadPool(size_t numThreads) : stop(false), tasksRemaining(0){
    for (size_t i = 0; i < numThreads; ++i){
        workers.emplace_back([this]{
            while(true){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    //wait until there are tasks or the pool is stopped
                    this->condition.wait(lock, [this] {return this->stop || !this->tasks.empty(); });
                    if(this->stop && this->tasks.empty()){
                        return; //exit if stopping and no tasks
                    }
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                    --tasksRemaining;
                }
                task(); //execute the task

                 // Notify when a task is completed
                {
                    std::lock_guard<std::mutex> lock(completionMutex);
                    completionCondition.notify_all();
                }
            }
        });
    }
}

/*************************************************************************************
 * ~ThreadPool (destructor) - Cleans up the thread pool by stopping all threads
 *************************************************************************************/
ThreadPool::~ThreadPool(){
    threadpoolEnd();
}
     
/*************************************************************************************
 * enqueue - Adds a new task to the thread pool
 *
 * Parameters:
 *   task - Function to be executed by a thread
 *************************************************************************************/
void ThreadPool::enqueue(std::function<void()> task){
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        if(stop){
            throw std::runtime_error("enqueue on stopped threadpool");
        }
        tasks.emplace(std::move(task));
        ++tasksRemaining;
        
    }
    condition.notify_one(); // notify a thread that a new tasks is available
    
}

/*************************************************************************************
 * getTasksRemaining - Returns the number of tasks remaining in the queue
 *
 * Returns: Number of remaining tasks
 *************************************************************************************/
size_t ThreadPool::getTasksRemaining(){
    std::unique_lock<std::mutex> lock(queueMutex);
    return tasksRemaining;
}

/*************************************************************************************
 * waitForCompletion - Waits until all tasks have been completed
 *************************************************************************************/
void ThreadPool::waitForCompletion() {
    std::unique_lock<std::mutex> lock(completionMutex);
    completionCondition.wait(lock, [this] {
        return tasksRemaining == 0;
    });
}

/*************************************************************************************
 * threadpoolEnd - Stops all threads and joins them
 *************************************************************************************/
void ThreadPool::threadpoolEnd(){
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all(); //notify all threads to stop
    for (std::thread &worker : workers){
        if(worker.joinable()){
            worker.join(); //join each thread
        }
    }
}