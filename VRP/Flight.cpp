#include "Flight.h"
#include <iostream>
#include <string>
#include "FerryTaskSetting.h"
using namespace std;

Flight::Flight(int _id,
	string _flightCompany,
	string _flightType,
	char _flightClass,
	int _ferryVehicles,
	string _apron,
	string _stand,
	int _standIndex,
	int _rdy,
	enum Direction _direction,
	string _terminal,
	int _terminalIndex):IFlightNode(_id, _flightCompany, _flightType,
		_flightClass, _ferryVehicles, _apron,
		_stand, _standIndex, _rdy, _direction,_terminal, _terminalIndex)
{
	// parameter setting
	if (direction != unassign) { // 普通节点
		setServiceStartTime(getDirection() == DIRECTION_ARRIVE ? getRdy() - fts FTS_t1 : getRdy() - fts FTS_t3);
		setServiceEndTime(getDirection() == DIRECTION_ARRIVE ? getRdy() - fts FTS_t2 : getRdy() - fts FTS_t4);
		aheadFirstTime = getDirection() == DIRECTION_ARRIVE ? fts FTS_t50 : fts FTS_t60;
		delayOtherTime = getDirection() == DIRECTION_ARRIVE ? fts FTS_max_delta_time : fts FTS_max_delta_time;
	} else { // 场站
		serviceStartTime = INT_MIN;
		serviceEndTime = INT_MAX;
		aheadFirstTime = INT_MIN;
		delayOtherTime = INT_MIN;
	}
	
}

ostream& operator<<(ostream& outs, Flight& flight)
{
	outs <<
		flight.getID() << ", " <<
		flight.getFlightCompany() << ", " <<
		flight.getFlightType() << ", " <<
		flight.getFlightClass() << ", " <<
		flight.getFerryVehicles() << ", " <<
		flight.getApron() << ", " <<
		flight.getStand() << ", " <<
		flight.getRdy() << ", " <<
		flight.getDirection() << ", [" <<
		flight.aheadFirstTime << ", " <<
		flight.delayOtherTime << "], " <<
		flight.getServiceStartTime() << ", " <<
		flight.getServiceEndTime() << std::endl;
	return outs;
}
string Flight::toString()
{
	string comma = ", ";
	string tmp;
	if (direction == DIRECTION_DEPART)
	{
		tmp =
			to_string(getID()) + ", " +
			getFlightCompany() + ", " +
			getFlightType() + ", " +
			getFlightClass() + ", " +
			to_string(getFerryVehicles()) + ", " +
			getApron() + ", " +
			getTerminal() + ", " +
			to_string(getTerminalIndex()) + ", " +
			getStand() + ", " +
			to_string(getStandIndex());
	}
	else {
		tmp =
			to_string(getID()) + ", " +
			getFlightCompany() + ", " +
			getFlightType() + ", " +
			getFlightClass() + ", " +
			to_string(getFerryVehicles()) + ", " +
			getApron() + ", " +
			getStand() + ", " +
			to_string(getStandIndex()) + ", " +
			getTerminal() + ", " +
			to_string(getTerminalIndex());
	}
	return tmp;
}