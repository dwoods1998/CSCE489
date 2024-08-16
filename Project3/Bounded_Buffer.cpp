
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

/*************************************************************************************
 * Item(constructor) - constructs an Item with the passed content
 *
 *    Params:  content - the content of the desired item
 *************************************************************************************/
Item::Item(const std::string& _content) : content(_content){}


/*************************************************************************************
 * GetContent - gets the content of the item
 *					
 *    Return:  content - content of the the item
 *************************************************************************************/
std::string Item::GetContent() const{return content;}


/*************************************************************************************
 * BoundedBuffer (constructor) - constructs the bounded buffer with the given capacity
 *						
 *    Params:  capacity - maximum number of items that the buffer can hold
 *************************************************************************************/
BoundedBuffer::BoundedBuffer(int _capacity)
    : capacity(_capacity), nextin(0), nextout(0), 
        count(0), done(false), notfull(_capacity),
        notempty(0), mutex(1){
    buffer.resize(_capacity, nullptr);
}


/*************************************************************************************
 * ~BoundedBuffer (destructor) - called when the class is destroyed. Clean up any 
 *                      dynamic memory. Also calls setDone and clears buffer.
 *************************************************************************************/
BoundedBuffer::~BoundedBuffer(){
    setDone();//notify all threads
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
void BoundedBuffer::Deposit(Item* _item){
    //std::cout << "  Producer waiting to deposit\n";
    notfull.wait(); //wait if buffer is full
    //std::cout << "  roducer locked buffer and dopositing item\n";
    mutex.wait(); //lock buffer

    if (done){
        delete _item; //avoid memory leak
        mutex.signal();//unlock buffer
        notfull.signal(); // signal buffer not full
        return; //if production is done
    }
    //buffer has space & own mutex: insert the item
    buffer[nextin] = _item;
    nextin = (nextin + 1) % capacity;
    count++;

    // std::cout <<"  Yoda # put on shelf.\n"; fixes the flip flop on the display
    //Notify one waiting that yoda is ready
    notempty.signal(); //notify buffer not empty
    mutex.signal(); //unlock buffer
    //std::cout << "  Producer deposited item\n";
}


/*************************************************************************************
 * Retrieve() - retrieves an item from the buffer
 *						
 *    Return:  Item - the item retrieved from buffor or nullptr if done is true
 *************************************************************************************/
Item* BoundedBuffer::Retrieve(){
    mutex.wait();//lock buffer
    //std::cout << "  Consumer waiting to retrieve\n";
    
    while(count == 0 && !done){
        mutex.signal();//unlock buffer
        notempty.wait();//wait for buffer to be not empty
        mutex.wait();//lock buffer
    }
   
    Item* item = nullptr;
    //sets and assigns next item for buffer to retrieve
    if(count > 0){
        item = buffer[nextout];
        buffer[nextout] = nullptr;
        nextout = (nextout + 1) % capacity;
        count--;
        notfull.signal();
        //std::cout << "  Consumer retrieved an item\n";
    }
    // std::cout <<"bought yoda\n"; fixes the flip flop on the display
    mutex.signal();//unlock the buffer
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
    notempty.setDone(); //notify consumers

    notfull.signal(); //notify producer
    
    mutex.signal(); //unlock buffer
}