#ifndef ITEM_H
#define ITEM_H

#include <string>

/**
 * Item - A class that represents an item with content.
 * 
 * This class stores a string content and provides a method to retrieve it.
 */
class Item{
    
public:
     /**
     * Constructor - Initializes the item with the specified content.
     * 
     * @param _content The content of the item.
     */
    Item(const std::string& _content);

     /**
     * GetContent - Retrieves the content of the item.
     * 
     * @return The content of the item.
     */
    std::string GetContent() const;   
    
private: 
    std::string content; // the content of the item
};   


#endif // ITEM_H