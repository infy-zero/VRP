#pragma once
#include <vector>
#include "Vehicle.h"

using namespace std;
class VehicleList
{
private:
	vector<Vehicle> vehicles;
public:
	vector<Vehicle> getVehicles()
	{
		return vehicles;
	}
	vector<vector<vector<int>>> getVehiclesNum()
	{
		vector<vector<vector<int>>> vehiclesNums;
		for (auto vehicle : vehicles)
		{
			auto tmp = vehicle.getTripsNum();
			vehiclesNums.push_back(tmp);
		}
		return vehiclesNums;
	}
};

