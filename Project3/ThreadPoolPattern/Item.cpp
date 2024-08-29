/*************************************************************************************
 * Item.cpp - Implementation of the Item class
 *************************************************************************************/
#include "Item.h"


/*************************************************************************************
 * Item - Constructs an Item with specified content
 *
 * Parameters:
 *   _content - Content of the item
 *************************************************************************************/
Item::Item(const std::string& _content) : content(_content){}

/*************************************************************************************
 * GetContent - Returns the content of the item
 *
 * Returns: Content of the item
 *************************************************************************************/
std::string Item::GetContent() const{return content;}

