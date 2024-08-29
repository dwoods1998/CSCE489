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

/**
 * ThreadPool - A class that manages a pool of worker threads to execute tasks.
 * 
 * This class provides methods to enqueue tasks, wait for completion, and clean up
 * resources. It handles thread synchronization and task distribution.
 */
class ThreadPool{
public:
    /**
     * Constructor - Initializes the thread pool with the specified number of threads.
     * 
     * @param numThreads The number of worker threads in the pool.
     */
    explicit ThreadPool(size_t numThreads);

     /**
     * Destructor - Stops all threads and cleans up resources.
     */
    ~ThreadPool();

     /**
     * enqueue - Adds a new task to the pool's task queue.
     * 
     * @param task The task to be executed by a worker thread.
     */
    void enqueue(std::function<void()> task);

     /**
     * getTasksRemaining - Retrieves the number of tasks remaining in the queue.
     * 
     * @return The number of remaining tasks.
     */
    size_t getTasksRemaining();

    /**
     * waitForCompletion - Waits until all tasks in the queue have been completed.
     */
    void waitForCompletion();

     /**
     * threadpoolEnd - Explicitly stops all threads and cleans up resources.
     */
    void threadpoolEnd();// explicit method for threadpool destruction

private:
    std::vector<std::thread> workers; //vector of worker threads
    std::queue<std::function<void()>> tasks; //queue of tasks
    std::mutex queueMutex; //mutex for synchronization access to task queue
    std::condition_variable condition; //condition variable for waiting and notifying
    bool stop; //flag indicating if the pool is stopping
    size_t tasksRemaining; //number of remaining tasks
    std::mutex completionMutex; //mutex for synchonizing task completion
    std::condition_variable completionCondition; //condition variable for task completion
};

#endif // THREADPOOL_H