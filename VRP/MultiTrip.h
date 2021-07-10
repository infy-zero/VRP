#pragma once
#include <iostream>
#include <string>

using namespace std;
class MultiTrip
{
private:
	int tripLimit;
public:
	MultiTrip()
	{
		this->tripLimit = 1;
	}
	MultiTrip(int tripLimit)
	{
		this->tripLimit = tripLimit;
	}
	int getTripLimit()
	{
		return tripLimit;
	}
	void setTripLimit(int tripLimit)
	{
		this->tripLimit = tripLimit;
	}
	string toString()
	{
		return "\tTotal Trips\t\t= " + to_string(tripLimit) + "\n";
	}
};

