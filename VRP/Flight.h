#pragma once
#include <string>
#include "IFlightNode.h"
#include <vector>
using namespace std;

class Flight: public IFlightNode
{
public:
	int aheadFirstTime; // 第一辆车提前到达时间 : 5
	int delayOtherTime; // 除第一辆车推迟到达时间 : 2
	Flight(int id, 
		string flightcompany, 
		string flighttype, 
		char flightclass, 
		int ferryVehicles, 
		string apron, 
		string stand, 
		int standIndex,
		int rdy, 
		enum Direction direction,
		string terminal,
		int terminalIndex);

	friend ostream& operator<<(ostream& outs, Flight& flight);
	string toString();
};

