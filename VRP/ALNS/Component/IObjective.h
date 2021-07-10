#pragma once
#include "ALNS/Component/ISolution.h"
class IObjective
{
private:
	ISolution* solution;
	int level;
public:
	virtual double getObjective();
};

