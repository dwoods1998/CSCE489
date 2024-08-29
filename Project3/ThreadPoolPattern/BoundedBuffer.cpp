
/*************************************************************************************
 * BoundedBuffer.cpp: my implimentation of a bounded buffer necessary for the babyyoda program
 * 
 * I referenced a lot of the structure of my bounded buffer from the link below.
 * https://stackoverflow.com/questions/15798222/is-this-a-correct-way-to-implement-a-bounded-buffer-in-c
 * Additionally, i utilized chatGPT for troubleshooting and general debugging
 *************************************************************************************/
#include "BoundedBuffer.h"
#include <iostream>
#include <stdexcept>
#include <memory>



/*************************************************************************************
 * BoundedBuffer (constructor) - Initializes the bounded buffer with a specified capacity
 *
 * Parameters:
 *   _capacity - Maximum number of items the buffer can hold
 *************************************************************************************/
BoundedBuffer::BoundedBuffer(int _capacity)
    : capacity(_capacity), count(0), done(false), serialnum(1){
}

/*************************************************************************************
 * ~BoundedBuffer (destructor) - Cleans up dynamic memory and clears the buffer
 *************************************************************************************/
BoundedBuffer::~BoundedBuffer(){
    monitor.Enter();
    while (!queue.empty()){
        delete queue.front();//free memory for each item
        queue.pop();
    }
    monitor.Exit();
}

/*************************************************************************************
 * Deposit - Adds an item to the buffer. Waits if the buffer is full and signals
 *            consumers when an item is added
 *
 * Parameters:
 *   producerID - ID of the producer depositing the item
 *************************************************************************************/
void BoundedBuffer::Deposit(int producerID){
    monitor.Enter();

    //wait if buffer is full and production is not done
    while(count >= capacity && !done){
        monitor.Wait();
    }
    
    if (done){
        monitor.Exit();
        return; //exit if production is done
    }

    //create and add new item into the buffer
    Item* item = new Item("Yoda #" + std::to_string(serialnum));
    ++serialnum;
    queue.push(item);
    ++count;
    
    std::cout <<"Producer #" <<producerID<<" put Yoda #" <<serialnum - 1<< " on shelf.\n";
    
    //Notify one waiting consumer that yoda is ready
    monitor.Signal();
    monitor.Exit();
    
}


/*************************************************************************************
 * Retrieve - Retrieves an item from the buffer. Waits if the buffer is empty
 *             and signals consumers when production is done
 *
 * Parameters:
 *   consumerID - ID of the consumer retrieving the item
 *
 * Returns: Pointer to the retrieved Item, or nullptr if done
 *************************************************************************************/
Item* BoundedBuffer::Retrieve(int consumerID){
    monitor.Enter();

    //wait if buffer is empty and production is not done
    while(count == 0 && !done){
        monitor.Wait();
    }

    if (count == 0 && done) {
        monitor.Exit();
        return nullptr; //return nullptr if buffer is done
    }
   
    //retrieve and remove item from buffer
    Item* item = queue.front();
    queue.pop();
    --count;
    monitor.Signal();//notify one waiting producer there is space
    monitor.Exit();
    
    std::cout << "Consumer #" << consumerID << " bought " << item->GetContent() << ".\n";

    return item; 
}

/*************************************************************************************
 * setDone - Sets the buffer as done producing and wakes up all waiting consumers
 *************************************************************************************/
void BoundedBuffer::setDone(){
    monitor.Enter();
    done = true;
    //notify all that production is done
    monitor.Broadcast();
    monitor.Exit();
}

/*************************************************************************************
 * isDone - Checks if production is complete
 *
 * Returns: True if production is done, false otherwise
 *************************************************************************************/
bool BoundedBuffer::isDone() {
    std::lock_guard<std::mutex> lock(monitor.getMutex());
    return done;
}