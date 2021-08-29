#include "ALNS/BasicClass/ValueRange.h"

double ValueRange::getMinVehicleTime()
{
	return minVehicleTime;
}
void ValueRange::setMinVehicleTime(double _minTime)
{
	minVehicleTime = _minTime;
}
double ValueRange::getCurTime()
{
	return curTime;
}
void ValueRange::setCurTime(double _curTime)
{
	curTime = _curTime;
}
double ValueRange::getMaxVehicleTime()
{
	return maxVehicleTime;
}
void ValueRange::setMaxVehicleTime(double _maxTime)
{
	maxVehicleTime = _maxTime;
}
double ValueRange::getMinFlightTime()
{
	return minFlightTime;
}
void ValueRange::setMinFlightTime(double _minFlightTime)
{
	minFlightTime = _minFlightTime;
}

double ValueRange::getMaxFlightTime()
{
	return maxFlightTime;
}
void ValueRange::setMaxFlightTime(double _maxFlightTime)
{
	maxFlightTime = _maxFlightTime;
}