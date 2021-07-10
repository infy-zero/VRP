#pragma once
#include <string>
#include <vector>

using namespace std;
enum Direction { arrive, depart };
class IFlightNode
{
private:
	int		id;
	string	flightType;
	string  flightCompany;
	char	flightClass;
	int		ferryVehicles;
	string  apron;
	string	stand;
	int		standIndex;
	int		rdy;
	int		serviceStartTime;
	int		serviceEndTime;
	enum Direction direction;
	string  terminal;
	int		terminalIndex;
public:
	IFlightNode() {}
	IFlightNode(int _id,
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
		int _terminalIndex)
	{
		id = _id;
		flightCompany = _flightCompany;
		flightType = _flightType;
		flightClass = _flightClass;
		ferryVehicles = _ferryVehicles;
		apron = _apron;
		stand = _stand;
		standIndex = _standIndex;
		rdy = _rdy;
		direction = _direction;
		terminal = _terminal;
		terminalIndex = _terminalIndex;
	}
	const int getID()
	{
		return id;
	}
	void setID(int _id)
	{
		id = _id;
	}
	const string	getFlightType()
	{
		return flightType;
	}
	const string	getFlightCompany()
	{
		return flightCompany;
	}
	const char getFlightClass()
	{
		return flightClass;
	}
	const int getFerryVehicles()
	{
		return ferryVehicles;
	}
	const string	getApron()
	{
		return apron;
	}
	const string	getStand()
	{
		return stand;
	}
	const int	getStandIndex()
	{
		return standIndex;
	}
	const int	getRdy()
	{
		return rdy;
	}
	const int getServiceStartTime()
	{
		return serviceStartTime;
	}
	void setServiceStartTime(int _serviceStartTime)
	{
		serviceStartTime = _serviceStartTime;
	}
	const int getServiceEndTime()
	{
		return serviceEndTime;
	}
	void setServiceEndTime(int _serviceEndTime)
	{
		serviceEndTime = _serviceEndTime;
	}
	const string getDirectionString()
	{
		return direction==arrive?"arrive":"depart";
	}
	const enum Direction getDirection()
	{
		return direction;
	}
	const string& getTerminal()
	{
		return terminal;
	}
	void setTerminal(string _terminal)
	{
		terminal = _terminal;
	}
	int getTerminalIndex()
	{
		return terminalIndex;
	}
};

