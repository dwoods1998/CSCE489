/*****************************************************
 * https://stackoverflow.com/questions/15798222/is-this-a-correct-way-to-implement-a-bounded-buffer-in-c
 * 
 *****************************************************/
#ifndef BOUNDEDBUFFER_h
#define BOUNDEDBUFFER_h

#include "Item.h"
#include "Monitor.h"
#include <queue>

/**
 * BoundedBuffer - A class that implements a bounded buffer using a queue.
 * 
 * This class manages the production and consumption of items with synchronization
 * mechanisms to ensure thread safety. It provides methods to deposit and retrieve
 * items, and to mark the buffer as done.
 */
class BoundedBuffer{

public:
     /**
     * Constructor - Initializes the buffer with the specified capacity.
     * 
     * @param capacity The maximum number of items the buffer can hold.
     */
    explicit BoundedBuffer(int capacity);

    /**
     * Destructor - Cleans up resources used by the buffer.
     */
    ~BoundedBuffer();

     /**
     * Deposit - Adds an item to the buffer.
     * 
     * @param producerID The ID of the producer depositing the item.
     */
    void Deposit(int producerID); 

     /**
     * Retrieve - Retrieves an item from the buffer.
     * 
     * @param consumerID The ID of the consumer retrieving the item.
     * @return The retrieved item, or nullptr if the buffer is done.
     */
    Item* Retrieve(int consumerID);
          
    /**
     * setDone - Marks the buffer as done and notifies all waiting threads.
     */     
    void setDone();

     /**
     * isDone - Checks if the buffer is marked as done.
     * 
     * @return True if the buffer is done, false otherwise.
     */
    bool isDone();
        

private:
    std::queue<Item*> queue; // buffer storage
    int capacity; // size of the buffer
    int count; //number of items currently in buffer
    bool done; //flag indicating production is done
    Monitor monitor; //synchronization monitor
    int serialnum; //serial number for items
};

#endif // BOUNDEDBUFFER_H