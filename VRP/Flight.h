#pragma once
#include <string>
#include "IFlightNode.h"
#include <vector>
using namespace std;

class Flight: public IFlightNode
{
public:
	int aheadFirstTime; // ��һ������ǰ����ʱ�� : 5
	int delayOtherTime; // ����һ�����Ƴٵ���ʱ�� : 2
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

