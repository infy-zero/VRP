#pragma once
#include <list>
#include <algorithm>
#include "ALNS/BasicClass/ALNSNode.h"
using namespace std;
class IRemovedList
{
private:
	list<ALNSNode*> removedList;
public:
	void add(ALNSNode* node);
	ALNSNode* get(int location);
	ALNSNode* randomGet();
	ALNSNode* poll();
};

