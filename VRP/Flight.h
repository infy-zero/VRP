#pragma once
#include <string>
#include "IFlightNode.h"
#include <vector>
using namespace std;

class Flight: public IFlightNode
{
private:
	int boardingTime[2];
public:
	Flight() {}
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

	int* getServiceTime()
	{
		return boardingTime;
	}
	void setServiceTime(int* _serviceTime)
	{
		boardingTime[0] = _serviceTime[0];
		boardingTime[1] = _serviceTime[1];
	}
	friend ostream& operator<<(ostream& outs, Flight& flight);
	string toString();
};

