#ifndef MONITOR_H
#define MONITOR_H

#include <mutex>
#include <condition_variable>

/**
 * Monitor - A class that encapsulates a mutex and condition variables to manage
 * synchronization in a thread-safe manner. It provides methods to acquire and release
 * the mutex and to signal waiting threads.
 */
class Monitor {
public:
    /**
     * Constructor - Initializes the monitor with a mutex and condition variable.
     */
    Monitor();

    /**
     * Destructor - Cleans up resources used by the monitor.
     */
    ~Monitor();

    /**
     * Enter - Acquires the monitor's mutex, blocking if necessary until it is available.
     */
    void Enter();

    /**
     * Exit - Releases the monitor's mutex, allowing other threads to acquire it.
     */
    void Exit();

    /**
     * Wait - Releases the monitor's mutex and waits for a condition to be signaled.
     */
    void Wait();

    /**
     * Signal - Signals one waiting thread that a condition may be met.
     */
    void Signal();

    /**
     * Broadcast - Signals all waiting threads that a condition may be met.
     */
    void Broadcast();

private:
    std::mutex mtx;                   // Mutex for synchronizing access to the monitor
    std::condition_variable cv;      // Condition variable for waiting and notifying
};

#endif // MONITOR_H