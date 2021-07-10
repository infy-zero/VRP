#pragma once
#include <vector>
#include <iostream>
#include "IRemovedList.h"
#include "ALNS/BasicClass/ALNSNode.h"
using namespace std;
// ���н�ĸ���
class ISolution
{
private:
	IRemovedList removedList;
public:
	ISolution();
	virtual string toString();
	virtual bool compare(ISolution* other);
	void addRemovedList(ALNSNode* node);
};

