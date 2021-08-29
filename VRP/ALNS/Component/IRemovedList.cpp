#include "IRemovedList.h"
#include "stdlib.h"
void IRemovedList::add(ISolutionNode* node)
{
	removedList.push_back(node);
}
ISolutionNode* IRemovedList::get(int location)
{
	auto iterator = removedList.begin();
	for (int i = 0; i < location; i++)
		iterator++;
	return *iterator;
}
ISolutionNode* IRemovedList::randomGet()
{
	auto iterator = removedList.begin();
	int location = rand() % removedList.size();
	for (int i = 0; i < location; i++)
		iterator++;
	return *iterator;
}
ISolutionNode* IRemovedList::poll()
{
	ISolutionNode* tmp = removedList.front();
	removedList.pop_front();
	return tmp;
}
list<ISolutionNode*>* IRemovedList::getRemovedList()
{
	return &removedList;
}
int IRemovedList::size()
{
	return removedList.size();
}