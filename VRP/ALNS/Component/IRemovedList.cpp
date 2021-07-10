#include "IRemovedList.h"
#include "stdlib.h"
void IRemovedList::add(ALNSNode* node)
{
	removedList.push_back(node);
}
ALNSNode* IRemovedList::get(int location)
{
	auto iterator = removedList.begin();
	for (int i = 0; i < location; i++)
		iterator++;
	return *iterator;
}
ALNSNode* IRemovedList::randomGet()
{
	auto iterator = removedList.begin();
	int location = rand() % removedList.size();
	for (int i = 0; i < location; i++)
		iterator++;
	return *iterator;
}
ALNSNode* IRemovedList::poll()
{
	ALNSNode* tmp = removedList.front();
	removedList.pop_front();
	return tmp;
}