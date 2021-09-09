#pragma once
#include <string>
#include "IFlightNode.h"
#include <vector>
using namespace std;

class Flight: public IFlightNode
{
public:
	int aheadFirstTime = INT_MIN; // 第一辆车提前到达时间 : 5
	int delayOtherTime = INT_MIN; // 除第一辆车推迟到达时间 : 2
	Flight(int id = INT_MIN,
		string flightcompany = NULL, 
		string flighttype = NULL,
		char flightclass = NULL,
		int ferryVehicles = INT_MIN,
		string apron = NULL,
		string stand = NULL,
		int standIndex = INT_MIN,
		int rdy = INT_MIN,
		enum Direction direction = unassign,
		string terminal = NULL,
		int terminalIndex = INT_MIN);

	friend ostream& operator<<(ostream& outs, Flight& flight);
	string toString();
};

