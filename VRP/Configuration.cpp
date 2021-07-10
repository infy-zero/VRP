#include "Configuration.h"
#include <string>
Configuration::Configuration()
{
	parallel		= *new Parallel();
	rollingCycle	= *new RollingCycle();
	multiDepot		= *new MultiDepot();
	multiTrip		= *new MultiTrip();
	multiVehicle	= *new MultiVehicle();
}
string Configuration::toString()
{
	return parallel.toString() +
		rollingCycle.toString() +
		multiDepot.toString() +
		multiTrip.toString() +
		multiVehicle.toString();
}
