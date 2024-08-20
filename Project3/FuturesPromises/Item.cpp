#include "Item.h"


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

