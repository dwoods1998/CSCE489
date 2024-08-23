/*****************************************************
 * https://stackoverflow.com/questions/15798222/is-this-a-correct-way-to-implement-a-bounded-buffer-in-c
 * 
 *****************************************************/
#ifndef BOUNDED_BUFFER_h
#define BOUNDED_BUFFER_h

#include "Item.h"
#include <mutex>
#include <queue> 
#include <condition_variable>

class BoundedBuffer{

    public:
        explicit BoundedBuffer(int capacity);
        ~BoundedBuffer();

        void Deposit(int producerID); 
        Item* Retrieve(int consumerID);        
        void setDone();
        bool isDone();
        

    private:
        std::queue<Item*> queue;
        int capacity;
        int count;
        bool done;
        std::mutex mutex;
        std::mutex coutMutex;
        std::condition_variable notfull;
        std::condition_variable notempty;
        int serialnum;

 
};

#endif