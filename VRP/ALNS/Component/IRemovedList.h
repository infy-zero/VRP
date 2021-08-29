#pragma once
#include <list>
#include <algorithm>
#include "ALNS/BasicClass/ISolutionNode.h"
using namespace std;
class IRemovedList
{
private:
	
public:
	void add(ISolutionNode* node);
	ISolutionNode* get(int location);
	ISolutionNode* randomGet();
	ISolutionNode* poll();
	list<ISolutionNode*>* getRemovedList();
	int size();
};

