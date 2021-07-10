#pragma once
#include "Parallel.h"
#include "RollingCycle.h"
#include "MultiDepot.h"
#include "MultiTrip.h"
#include "MultiVehicle.h"
#include <string>

class Configuration
{
private:
	Parallel parallel;
	RollingCycle rollingCycle;
	MultiDepot multiDepot;
	MultiTrip multiTrip;
	MultiVehicle multiVehicle;
public:
	Configuration();
	Parallel& getParallel() 
	{
		return parallel;
	}
	RollingCycle& getRollingCycle()
	{
		return rollingCycle;
	}
	MultiDepot& getMultiDepot()
	{
		return multiDepot;
	}
	MultiTrip& getMultiTrip()
	{
		return multiTrip;
	}
	MultiVehicle& getMultiVehicle()
	{
		return multiVehicle;
	}
	string toString();
};

