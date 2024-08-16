#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <mutex>
#include <condition_variable>

class Semaphore {
public:

	explicit Semaphore(int initial_count = 0);
	~Semaphore();

	void wait();
	void signal();
	void setDone();

private:
	std::mutex mtx;
	std::condition_variable cv;
	int count;
	bool done;
};

#endif
