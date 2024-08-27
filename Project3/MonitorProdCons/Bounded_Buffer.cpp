
/*************************************************************************************
 * Bounded_Buffer.cpp: my implimentation of a bounded buffer necessary for the babyyoda program
 * 
 * I referenced a lot of the structure of my bounded buffer from the link below.
 * https://stackoverflow.com/questions/15798222/is-this-a-correct-way-to-implement-a-bounded-buffer-in-c
 * Additionally, i utilized chatGPT for troubleshooting and general debugging
 *************************************************************************************/
#include "Bounded_Buffer.h"
#include <iostream>
#include <sstream>

/*************************************************************************************
 * BoundedBuffer (constructor) - constructs the bounded buffer with the given capacity
 *						
 *    Params:  capacity - maximum number of items that the buffer can hold
 *************************************************************************************/
BoundedBuffer::BoundedBuffer(int _capacity)
    : capacity(_capacity), nextin(0), nextout(0), 
        count(0), done(false), serialnum(1) {
    buffer.resize(_capacity, nullptr);
}


/*************************************************************************************
 * ~BoundedBuffer (destructor) - called when the class is destroyed. Clean up any 
 *                      dynamic memory. Also calls setDone and clears buffer.
 *************************************************************************************/
BoundedBuffer::~BoundedBuffer(){
    setDone();//sets done flag
    for (auto item : buffer){
        delete item; //clean allocated items
    }
    buffer.clear(); //clear buffer  
}


/*************************************************************************************
 * Deposit() - Deposits an item into the buffer. Waits if buffer is full and signals
 *      consumers when an item is added
 *						
 *    Params:  _item - an item that will be added to the buffer
 *************************************************************************************/
void BoundedBuffer::Deposit(){
    monitor.Enter();
    while (count == capacity){
        monitor.Wait();
    }
    

    if (done){
        monitor.Exit();
        return;
    }

    Item* item = new Item("Yoda #" + std::to_string(serialnum++));
    
    //buffer has space & own mutex: insert the item
    buffer[nextin] = item;
    nextin = (nextin + 1) % capacity;
    count++;

    std::cout <<"Producer put Yoda #" <<serialnum - 1<< " put on shelf.\n";
    //Notify one waiting that yoda is ready
    monitor.Signal(); //notify buffer not empty
    monitor.Exit();
}


/*************************************************************************************
 * Retrieve() - retrieves an item from the buffer
 *						
 *    Return:  Item - the item retrieved from buffor or nullptr if done is true
 *************************************************************************************/
Item* BoundedBuffer::Retrieve(int consumerID){
    monitor.Enter();
    while(count == 0 && !done){
        monitor.Wait(); //wait if buffer is empty
    }

    if (count == 0 && done) {
        monitor.Exit();
        return nullptr;
    }
   
    
    //sets and assigns next item for buffer to retrieve
    
    Item* item = buffer[nextout];
    buffer[nextout] = nullptr;
    nextout = (nextout + 1) % capacity;
    count--;
    monitor.Signal(); //Notify buffer not full
    
    std::cout << "Consumer#" << consumerID << " bought " << item->GetContent() << ".\n";
    monitor.Exit();
    return item;
    
}

/*************************************************************************************
 * setDone() - Sets the done flag for the buffer to indication production is complete
 *			and wakes up all waiting consumers and the producer
 *************************************************************************************/
void BoundedBuffer::setDone(){
    monitor.Enter(); //Lock buffer
    done = true;
    //notify all that production is done
    monitor.Broadcast(); //notify consumers

    monitor.Exit();
}