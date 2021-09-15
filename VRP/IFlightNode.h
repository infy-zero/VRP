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

	int		id					= INT_MIN;   // �ڵ����
	string	flightType			= NULL;		 // �������
	string  flightCompany		= NULL;		 // ���๫˾
	char	flightClass			= NULL;		 // ���༶��
	int		ferryVehicles		= INT_MIN;	 // ��������ڶɳ�����
	string  apron				= NULL;		 // ��ƺ
	string	stand				= NULL;		 // Զ��λ��
	int		standIndex			= INT_MIN;	 // Զ��λ�Ǳ�
	int		rdy					= INT_MIN;	 // ׼��ʱ��
	int		serviceStartTime	= INT_MIN;	 // ����ʼʱ��
	int		serviceEndTime		= INT_MIN;	 // �������ʱ�䣨���ã�
	enum Direction direction    = unassign;	 // ���෽�򣨽��ۺ���/��ۺ��ࣩ
	string  terminal			= NULL;		 // ��վ¥����
	int		terminalIndex		= INT_MIN;	 // ��վ¥�Ǳ�
};

