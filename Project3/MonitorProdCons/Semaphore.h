#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <mutex>
#include <condition_variable>

class Semaphore{
    public:
    Semaphore(int intial_count);
    ~Semaphore();
    void wait();
    void signal();
    void isDone(); //was setDone

    private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
    bool done;
};

#endif