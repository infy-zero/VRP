#pragma once
#include <iostream>
#include <string>

using namespace std;
class MultiVehicle
{
private:
	int vehicles = 1;
public:
	int getVehicles()
	{
		return vehicles;
	}
	string toString()
	{
		return "\tTotal VehicleTypes\t= " + to_string(vehicles) + "\n";
	}
};

