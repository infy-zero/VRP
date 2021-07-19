#include "IVehicle.h"
bool IVehicle::isUpdate()
{
	return update;
}
list<ISolutionNode*>::iterator& IVehicle::getFirstBestLoc()
{
	return firstBestLoc;
}
list<ISolutionNode*>::iterator& IVehicle::getSecondBestLoc()
{
	return secondBestLoc;
}
void IVehicle::clearBestLoc()
{
	firstBestLoc  = vehicles.end();
	secondBestLoc = vehicles.end();
}
double IVehicle::getLength()
{
	return 0;
}