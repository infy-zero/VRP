#pragma once
#include <list>
#include <algorithm>
#include "ALNS/BasicClass/ISolutionNode.h"
using namespace std;
class IRemovedList
{
private:
	list<ISolutionNode> removedList;
public:
	void add(ISolutionNode node);
	ISolutionNode get(int location);
	ISolutionNode randomGet();
	ISolutionNode poll();
};

