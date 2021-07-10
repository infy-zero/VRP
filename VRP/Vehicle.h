#pragma once
#include <vector>
#include "Trip.h"

using namespace std;

class Vehicle
{
private:
	vector<Trip> trips;
public:
	vector<Trip> getTrips()
	{
		return trips;
	}
	vector<vector<int>> getTripsNum()
	{
		vector<vector<int>> tripsNums;
		for (auto trip : trips)
		{
			auto tmp = trip.getSegmentsNum();
			tripsNums.push_back(tmp);
		}
		return tripsNums;
	}
};

