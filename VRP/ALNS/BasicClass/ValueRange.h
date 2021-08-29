#pragma once
class ValueRange
{
private:
	double curTime;
	double minVehicleTime;
	double maxVehicleTime;
	double minFlightTime;
	double maxFlightTime;
public:
	double getCurTime();
	void setCurTime(double _curVehicleTime); 

	double getMinVehicleTime();
	void setMinVehicleTime(double _minVehicleTime);

	double getMaxVehicleTime();
	void setMaxVehicleTime(double _maxVehicleTime);

	double getMinFlightTime();
	void setMinFlightTime(double _minFlightTime);

	double getMaxFlightTime();
	void setMaxFlightTime(double _maxFlightTime);

};

