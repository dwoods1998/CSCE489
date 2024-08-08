
 //https://stackoverflow.com/questions/15798222/is-this-a-correct-way-to-implement-a-bounded-buffer-in-c
//

#include "Bounded_Buffer.h"
#include <stdexcept>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>

Item::Item(const std::string& _content) : content(_content){}

std::string Item::GetContent() const{return content;}

BoundedBuffer::BoundedBuffer(int _capacity)
    : capacity(_capacity), nextin(0), nextout(0), 
        count(0), done(false), notfull(_capacity),
        notempty(0), mutex(1){
    if (_capacity <= 0){
        throw std::invalid_argument("Capacity must be positive");
    }
    buffer.resize(_capacity, nullptr);
}

BoundedBuffer::~BoundedBuffer(){
    
    setDone();
    for (auto item : buffer){
        delete item; //clean allocated items
    }
    buffer.clear(); //clear buffer
    
}

void BoundedBuffer::Deposit(Item* _item){
    // std::cout << "producer waiting to deposit...\n";
    notfull.wait(); //wait if buffer is full
    // std::cout << "producer locked buffer and dopositing item\n";
    mutex.wait(); //lock buffer

    if (done){
        delete _item; //avoids memory leak
        mutex.signal();//unlock buffer
        notfull.signal(); // signal buffer not full
        // std::cout <<"producer found done, exiting..\n";
        return; //if production is done
    }
    /* buffer has space and we own the mutex: insert the item */
    buffer[nextin] = _item;
    nextin = (nextin + 1) % capacity;
    count++;

    //Notify one waiting that yoda is ready
    notempty.signal(); //notify buffer is not empty
    mutex.signal(); //unlock the buffer
    // std::cout << "Producer deposited item and unlocked buffer...\n";
}

Item* BoundedBuffer::Retrieve(){
    mutex.wait();
    // std::cout << "Consumer waiting to retrieve...\n";
    
    while(count == 0 && !done){
        mutex.signal();
        notempty.wait();
        mutex.wait();
    }


    Item* item = nullptr;
    if(count > 0){
        item = buffer[nextout];
        buffer[nextout] = nullptr;
        nextout = (nextout + 1) % capacity;
        count--;
        notfull.signal();
    }else if (done){
        // std::cout << "Consumer found done and buffer empty, exiting....\n";
    }

    mutex.signal();
    return item;
    
}


//set flag to indicate production complete
void BoundedBuffer::setDone(){
    mutex.wait(); //lock buffer
    done = true;

    //notify all that production is done
    notempty.signal(); //release waiting consumer
    notfull.signal(); //release waiting producer
    
    mutex.signal(); //unlock buffer
}