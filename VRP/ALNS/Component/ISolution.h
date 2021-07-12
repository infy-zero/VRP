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

	vector<double> objectives;
public:
	ISolution();
	virtual string toString();
	virtual double compare(ISolution* other);
	void addRemovedList(ALNSNode* node);
	void setObjectives(vector<double> _objectives);
	vector<double> getObjectives();
};

