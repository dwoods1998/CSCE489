#include <pthread.h>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include "Semaphore.h"


/*************************************************************************************
 * Semaphore (constructor) - this should take count and place it into a local variable.
 *						Here you can do any other initialization you may need.
 *
 *    Params:  count - initialization count for the semaphore
 *
 *************************************************************************************/
Semaphore::Semaphore(int count) : count(count){}


/*************************************************************************************
 * ~Semaphore (destructor) - called when the class is destroyed. Clean up any dynamic
 *						memory.
 *
 *************************************************************************************/

Semaphore::~Semaphore() {
}


/*************************************************************************************
 * wait - implement a standard wait Semaphore method here
 *
 *************************************************************************************/

void Semaphore::wait() {
    std::unique_lock<std::mutex> lock(mtx);
    while (count <= 0) {
        cv.wait(lock); //waits until > 0
    }
    --count;
}


/*************************************************************************************
 * signal - implement a standard signal Semaphore method here
 *
 *************************************************************************************/

void Semaphore::signal() {
    std::lock_guard<std::mutex> lock(mtx);
    ++count;
    cv.notify_one(); //notify a waiting thread
}