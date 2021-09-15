#pragma once
#include <string>
#include <vector>

using namespace std;
enum Direction { DIRECTION_ARRIVE, DIRECTION_DEPART, unassign };
class IFlightNode
{
public:
	IFlightNode() = default;
	IFlightNode(int _id,
		string _flightCompany ,
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
		return direction==DIRECTION_ARRIVE?"arrive":"depart";
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

	int		id					= INT_MIN;   // 节点序号
	string	flightType			= NULL;		 // 航班机型
	string  flightCompany		= NULL;		 // 航班公司
	char	flightClass			= NULL;		 // 航班级别
	int		ferryVehicles		= INT_MIN;	 // 航班所需摆渡车数量
	string  apron				= NULL;		 // 机坪
	string	stand				= NULL;		 // 远机位号
	int		standIndex			= INT_MIN;	 // 远机位角标
	int		rdy					= INT_MIN;	 // 准备时间
	int		serviceStartTime	= INT_MIN;	 // 服务开始时间
	int		serviceEndTime		= INT_MIN;	 // 服务结束时间（无用）
	enum Direction direction    = unassign;	 // 航班方向（进港航班/离港航班）
	string  terminal			= NULL;		 // 航站楼名称
	int		terminalIndex		= INT_MIN;	 // 航站楼角标
};

