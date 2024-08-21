
/*************************************************************************************
 * Bounded_Buffer.cpp: my implimentation of a bounded buffer necessary for the babyyoda program
 * 
 * I referenced a lot of the structure of my bounded buffer from the link below.
 * https://stackoverflow.com/questions/15798222/is-this-a-correct-way-to-implement-a-bounded-buffer-in-c
 * Additionally, i utilized chatGPT for troubleshooting and general debugging
 *************************************************************************************/
#include "Bounded_Buffer.h"
#include <iostream>
#include <stdexcept>
#include <memory>

/*************************************************************************************
 * BoundedBuffer (constructor) - constructs the bounded buffer with the given capacity
 *						
 *    Params:  capacity - maximum number of items that the buffer can hold
 *************************************************************************************/
BoundedBuffer::BoundedBuffer(int _capacity)
    : capacity(_capacity), count(0), done(false), serialnum(1){
}



/*************************************************************************************
 * ~BoundedBuffer (destructor) - called when the class is destroyed. Clean up any 
 *                      dynamic memory. Also calls setDone and clears buffer.
 *************************************************************************************/
BoundedBuffer::~BoundedBuffer(){
    //setDone();//sets done flag
    std::unique_lock<std::mutex> lock(mutex);
    while (!queue.empty()){
        delete queue.front(); //clean allocated items
        queue.pop();
    }
     
}


/*************************************************************************************
 * Deposit() - Deposits an item into the buffer. Waits if buffer is full and signals
 *      consumers when an item is added
 *						
 *    Params:  _item - an item that will be added to the buffer
 *************************************************************************************/
void BoundedBuffer::Deposit(int producerID){
    std::unique_lock<std::mutex> lock(mutex);
    notfull.wait(lock, [this]{return count < capacity || done; });
    

    if (done){
        return;
    }

    Item* item = new Item("Yoda #" + std::to_string(serialnum));
    ++serialnum;

    queue.push(item);
    ++count;
    

    std::cout <<"Producer #" <<producerID<<" put Yoda #" <<serialnum - 1<< " on shelf.\n";
    //Notify one waiting that yoda is ready
    notempty.notify_one();
    
}


/*************************************************************************************
 * Retrieve() - retrieves an item from the buffer
 *						
 *    Return:  Item - the item retrieved from buffor or nullptr if done is true
 *************************************************************************************/
Item* BoundedBuffer::Retrieve(int consumerID){
    std::unique_lock<std::mutex> lock(mutex);
    notempty.wait(lock, [this]{return count>0 || done;});


    if (count == 0 && done) {
        return nullptr;
    }
   
    
    //sets and assigns next item for buffer to retrieve
    
    Item* item = queue.front();
    queue.pop();
    count--;
    notfull.notify_one();
    
    std::cout << "Consumer #" << consumerID << " bought " << item->GetContent() << ".\n";
    return item;
    
}

/*************************************************************************************
 * setDone() - Sets the done flag for the buffer to indication production is complete
 *			and wakes up all waiting consumers and the producer
 *************************************************************************************/
void BoundedBuffer::setDone(){
    std::unique_lock<std::mutex> lock(mutex);
    done = true;
    //notify all that production is done
    notempty.notify_all(); //notify consumers
    notfull.notify_all(); //notify producer
}

bool BoundedBuffer::isDone() {
    std::unique_lock<std::mutex> lock(mutex);
    return done;
}