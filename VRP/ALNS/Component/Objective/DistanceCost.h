#pragma once
#include "ALNS/Component/IObjective.h"
#include "ALNS/Component/ISolution.h"
#include "ALNS/setting/ALNS_Setting.h"
class DistanceCost:public IObjective
{
private:
	double pcost = ALNS_Setting::pcost;
public:
	DistanceCost();
	double calObjective(ISolution& solution);
};