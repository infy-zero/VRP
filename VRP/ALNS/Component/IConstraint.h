#pragma once
#include "ALNS/Component/ISolution.h"
// ����Լ���ĸ���
class IConstraint
{
private:
	ISolution* solution;
	bool isRelaxed;
public:
	virtual bool getFeasible();
};

