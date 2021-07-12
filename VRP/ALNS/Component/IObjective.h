#pragma once
#include "ALNS/Component/ISolution.h"
class IObjective
{
private:
	int level;
public:
	virtual double calObjective(ISolution* solution);
};

