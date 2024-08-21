#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item{

    public:
        Item(const std::string& _content);// : content(_content){}
        std::string GetContent() const; //{return content;}     
    
    private: 
        std::string content;
};   


#endif