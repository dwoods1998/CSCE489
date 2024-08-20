
/*************************************************************************************
 * Bounded_Buffer.cpp: my implimentation of a bounded buffer necessary for the babyyoda program
 * 
 * I referenced a lot of the structure of my bounded buffer from the link below.
 * https://stackoverflow.com/questions/15798222/is-this-a-correct-way-to-implement-a-bounded-buffer-in-c
 * Additionally, i utilized chatGPT for troubleshooting and general debugging
 *************************************************************************************/
#include "Bounded_Buffer.h"
#include <stdexcept>
#include <iostream>
#include <sstream>

/*************************************************************************************
 * BoundedBuffer (constructor) - constructs the bounded buffer with the given capacity
 *						
 *    Params:  capacity - maximum number of items that the buffer can hold
 *************************************************************************************/
BoundedBuffer::BoundedBuffer(int _capacity)
    : capacity(_capacity), nextin(0), nextout(0), 
        count(0), done(false), notfull(_capacity),
        notempty(0), mutex(1), serialnum(1) {
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
    notfull.wait(); //wait if buffer is full
    mutex.wait(); //locks buffer
    

    if (done){
        return;
    }

    Item* item = new Item("Yoda #" + std::to_string(serialnum++));
    
    //buffer has space & own mutex: insert the item
    buffer[nextin] = item;
    nextin = (nextin + 1) % capacity;
    count++;

    std::cout <<"Producer put Yoda #" <<serialnum - 1<< " put on shelf.\n";
    //Notify one waiting that yoda is ready
    notempty.signal(); //notify buffer not empty
    mutex.signal();
}


/*************************************************************************************
 * Retrieve() - retrieves an item from the buffer
 *						
 *    Return:  Item - the item retrieved from buffor or nullptr if done is true
 *************************************************************************************/
Item* BoundedBuffer::Retrieve(int consumerID){
    notempty.wait();

    if (count == 0 && done) {
        return nullptr;
    }
   
    
    //sets and assigns next item for buffer to retrieve
    
    Item* item = buffer[nextout];
    buffer[nextout] = nullptr;
    nextout = (nextout + 1) % capacity;
    count--;
    notfull.signal();
    
    std::cout << "Consumer#" << consumerID << " bought " << item->GetContent() << ".\n";
    mutex.signal();
    return item;
    
}

/*************************************************************************************
 * setDone() - Sets the done flag for the buffer to indication production is complete
 *			and wakes up all waiting consumers and the producer
 *************************************************************************************/
void BoundedBuffer::setDone(){
    mutex.wait(); //lock buffer
    done = true;
    //notify all that production is done
    notempty.isDone(); //notify consumers

    notfull.signal(); //notify producer
}