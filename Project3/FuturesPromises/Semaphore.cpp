#include "Semaphore.h"


/*************************************************************************************
 * Semaphore (constructor) - this should take count and place it into a local variable.
 *						Here you can do any other initialization you may need.
 *
 *    Params:  count - initialization count for the semaphore
 *
 *************************************************************************************/
Semaphore::Semaphore(int initial_count) : count(initial_count), done(false){}

/*************************************************************************************
 * ~Semaphore (destructor) - called when the class is destroyed. Clean up any dynamic
 *						memory.
 *
 *************************************************************************************/
Semaphore::~Semaphore(){}

/*************************************************************************************
 * wait - implement a standard wait Semaphore method here
 *
 *************************************************************************************/
void Semaphore::wait(){
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]() {return count > 0 || done;});
    if (count > 0){
        --count;
    }
}

/*************************************************************************************
 * signal - implement a standard signal Semaphore method here
 *
 *************************************************************************************/
void Semaphore::signal(){
    std::lock_guard<std::mutex> lock(mtx);
    if(!done){
        ++count;
        cv.notify_one();
    }
}

/*************************************************************************************
 * isDone - implement a standard signal for when production is done
 *
 *************************************************************************************/
void Semaphore::isDone(){
    std::lock_guard<std::mutex> lock(mtx);
    done = true;
    cv.notify_all();
}
