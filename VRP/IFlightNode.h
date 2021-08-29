#pragma once
#include <string>
#include <vector>

using namespace std;
enum Direction { arrive, depart };
class IFlightNode
{
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
		int _terminalIndex):id(_id),flightCompany(_flightCompany), flightType(_flightType),flightClass(_flightClass), ferryVehicles(_ferryVehicles), apron(_apron), stand(_stand), standIndex(_standIndex), rdy(_rdy), direction(_direction), terminal(_terminal), terminalIndex(_terminalIndex)
	{
		
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

	int		id;							// 节点序号
	string	flightType;					// 航班机型
	string  flightCompany;				// 航班公司
	char	flightClass;				// 航班级别
	int		ferryVehicles;				// 航班所需摆渡车数量
	string  apron;						// 机坪
	string	stand;						// 远机位号
	int		standIndex;					// 远机位角标
	int		rdy;						// 准备时间
	int		serviceStartTime;			// 服务开始时间
	int		serviceEndTime;				// 服务结束时间（无用）
	enum Direction direction;			// 航班方向（进港航班/离港航班）
	string  terminal;					// 航站楼名称
	int		terminalIndex;				// 航站楼角标
};

