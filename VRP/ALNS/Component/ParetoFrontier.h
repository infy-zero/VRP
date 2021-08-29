#pragma once
#include <list>
#include <vector>
#include <string>
#include <assert.h>
#include "ISolution.h"
using namespace std;
class ParetoFrontier
{
private:
	list<pair<double,string>> paretoFrontier; // 每个帕累托解由vector和string构成，前者是目标函数值，后者是解的表达式
public:
	bool insert(ISolution& solution);
};

