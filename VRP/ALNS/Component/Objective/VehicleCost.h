#pragma once
#include "ALNS/Component/IObjective.h"
#include "ALNS/Component/ISolution.h"
#include "ALNS/setting/ALNS_Setting.h"
class VehicleCost:public IObjective
{
private:
	double costPerVehicle = ALNS_Setting::vcost;
public:
	VehicleCost();
	double calObjective(ISolution& solution);
};