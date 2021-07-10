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
	FerryTaskSetting fts;
	int t1 = fts.t1;
	int t2 = fts.t2;
	int t3 = fts.t3;
	int t4 = fts.t4;
	int t50 = fts.t50;
	int t51 = fts.t51;
	int t60 = fts.t60;
	int t61 = fts.t61;
	setServiceStartTime(getDirection() == arrive ? getRdy() - t1 : getRdy() - t3);
	setServiceEndTime(getDirection() == arrive ? getRdy() - t2 : getRdy() - t4);
	boardingTime[0] = getDirection() == arrive ? t50 : t60;
	boardingTime[1] = getDirection() == arrive ? t51 : t61;
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
		flight.getServiceTime()[0] << ", " <<
		flight.getServiceTime()[1] << "], " <<
		flight.getServiceStartTime() << ", " <<
		flight.getServiceEndTime() << std::endl;
	return outs;
}
string Flight::toString()
{
	string comma = ", ";
	string tmp =
		to_string(getID())+", "+
		getFlightCompany() + ", " +
		getFlightType() + ", " +
		getFlightClass() + ", " +
		to_string(getFerryVehicles()) + ", " +
		getApron() + ", " +
		getTerminal() + ", " +
		to_string(getTerminalIndex()) + ", " +
		getStand() + ", " +
		to_string(getStandIndex()) + ", " +
		to_string(getRdy()) + ", " +
		getDirectionString() + ", [" +
		to_string(getServiceTime()[0]) + ", " +
		to_string(getServiceTime()[1]) + "], " +
		to_string(getServiceStartTime()) + ", " +
		to_string(getServiceEndTime()) + ", "  ;
	return tmp;
}