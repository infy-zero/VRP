#pragma once
#include "ALNS/Component/ISolution.h"
// 所有约束的父类
class IConstraint
{
private:
	ISolution* solution;
	bool isRelaxed;
public:
	virtual bool getFeasible();
};

