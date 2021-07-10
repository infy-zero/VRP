#pragma once
#include <iostream>
#include <string>

using namespace std;
class RollingCycle
{
private:
	int rollingCycle = 1;
public:
	int getRollingCycle()
	{
		return rollingCycle;
	}
	string toString()
	{
		return "\tTotal RollingCycles\t= " + to_string(rollingCycle) + "\n";
	}
};

