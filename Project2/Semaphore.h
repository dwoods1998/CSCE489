#ifndef SEMAPHORE_H
#define SEMAPHORE_H

class Semaphore 
{
public:

	Semaphore(int count);
	~Semaphore();

	void wait();
	void signal();

private:
	std::mutex mtx;
	std::condition_variable cv;
	int count;
};

#endif
