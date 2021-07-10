#pragma once
#include <iostream>
#include <string>

using namespace std;
class MultiDepot
{
private:
	int depots = 1;
public:
	int getDepots()
	{
		return depots;
	}
	string toString()
	{
		return "\tTotal Depots\t\t= " + to_string(depots) + "\n";
	}
};

