/*****************************************************
 * https://stackoverflow.com/questions/15798222/is-this-a-correct-way-to-implement-a-bounded-buffer-in-c
 * 
 *****************************************************/
#ifndef BOUNDED_BUFFER_h
#define BOUNDED_BUFFER_h

#include "Item.h"
#include "Monitor.h"
#include <vector>  
#include <string>
/**
 * BoundedBuffer - A class that implements a bounded buffer using a monitor for thread
 * synchronization. The buffer allows items to be produced and consumed in a thread-safe
 * manner.
 */
class BoundedBuffer{

public:
    /**
     * Constructor - Initializes the bounded buffer with a given capacity.
     * 
     * @param _capacity The maximum number of items that the buffer can hold.
     */
    explicit BoundedBuffer(int _capacity);

     /**
     * Destructor - Cleans up any dynamic memory and resources.
     */
    ~BoundedBuffer();

    /**
     * Deposit - Deposits an item into the buffer. Waits if the buffer is full and
     * signals consumers when an item is added.
     */
    void Deposit(); 

     /**
     * Retrieve - Retrieves an item from the buffer.
     * 
     * @param consumerID The ID of the consumer retrieving the item.
     * @return The item retrieved from the buffer, or nullptr if done is true.
     */
    Item* Retrieve(int consumerID);      

     /**
     * setDone - Sets the done flag to indicate that production is complete and
     * wakes up all waiting consumers and the producer.
     */
    void setDone();
        

private:
    int capacity;
    int nextin;
    int nextout;
    int count;
    bool done;

    Monitor monitor;
    std::vector<Item*> buffer;
    int serialnum; 
 
};

#endif