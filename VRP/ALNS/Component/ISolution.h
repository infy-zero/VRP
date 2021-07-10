#pragma once
#include <vector>
#include <iostream>
#include "IRemovedList.h"
#include "ALNS/BasicClass/ALNSNode.h"
using namespace std;
// 所有解的父类
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

