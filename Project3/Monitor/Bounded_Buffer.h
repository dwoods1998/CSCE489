/*****************************************************
 * https://stackoverflow.com/questions/15798222/is-this-a-correct-way-to-implement-a-bounded-buffer-in-c
 * 
 *****************************************************/
#ifndef BOUNDED_BUFFER_h
#define BOUNDED_BUFFER_h

#include "Item.h"
#include "Semaphore.h"
#include <vector>  
#include <string>
#include <unordered_map>
#include <condition_variable>






class BoundedBuffer{

    public:
        explicit BoundedBuffer(int _capacity);
        ~BoundedBuffer();

        void Deposit(); 
        Item* Retrieve(int consumerID);        
        void setDone();
        

    private:
        int capacity;
        int nextin;
        int nextout;
        int count;
        bool done;

        Semaphore notfull;
        Semaphore notempty;
        Semaphore mutex;
        std::vector<Item*> buffer;
        int serialnum; 
 
};

#endif