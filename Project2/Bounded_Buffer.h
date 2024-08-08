/*****************************************************
 * https://stackoverflow.com/questions/15798222/is-this-a-correct-way-to-implement-a-bounded-buffer-in-c
 * 
 *****************************************************/
#ifndef BOUNDED_BUFFER_h
#define BOUNDED_BUFFER_h

#include "Semaphore.h"
#include <vector>  
#include <string>


class Item{

    public:
        explicit Item(const std::string& _content);// : content(_content){}
        std::string GetContent() const; //{return content;}     
    
    private: 
        std::string content;
};   

class BoundedBuffer{

    public:
        explicit BoundedBuffer(int _capacity);
        ~BoundedBuffer();

        void Deposit(Item* _item); 
        Item* Retrieve();        
        void setDone();
        

    private:
        int capacity; 
        std::vector<Item*> buffer; 
        int nextin; 
        int nextout; 
        int count;
        bool done; 

        Semaphore notfull;
        Semaphore notempty;
        Semaphore mutex;  
 
};

#endif