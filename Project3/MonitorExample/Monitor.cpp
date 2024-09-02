#include "Monitor.h"

/*************************************************************************************
 * Monitor (constructor) - Initializes the monitor with a mutex and condition variable.
 *************************************************************************************/
Monitor::Monitor() {}
/*************************************************************************************
 * ~Monitor (destructor) - Cleans up resources used by the monitor.
 *************************************************************************************/
Monitor::~Monitor() {}
/*************************************************************************************
 * Enter - Acquires the monitor's mutex, blocking if necessary until it is available.
 *************************************************************************************/
void Monitor::Enter() {
    mtx.lock();
}
/*************************************************************************************
 * Exit - Releases the monitor's mutex, allowing other threads to acquire it.
 *************************************************************************************/
void Monitor::Exit() {
    mtx.unlock();
}
/*************************************************************************************
 * Wait - Releases the monitor's mutex and waits for a condition to be signaled.
 *************************************************************************************/
void Monitor::Wait() {
    std::unique_lock<std::mutex> lock(mtx, std::adopt_lock);
    cv.wait(lock);
}
/*************************************************************************************
 * Signal - Signals one waiting thread that a condition may be met.
 *************************************************************************************/
void Monitor::Signal() {
    cv.notify_one();
}
/*************************************************************************************
 * Broadcast - Signals all waiting threads that a condition may be met.
 *************************************************************************************/
void Monitor::Broadcast() {
    cv.notify_all();
}