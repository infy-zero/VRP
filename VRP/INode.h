#pragma once
#include <vector>

using namespace std;
enum NodeType {DEPOT,VEHICLE,TRIP,SEGMENT,NODE};
class INode
{
private:
	enum NodeType nodeType;
	int num;
	INode* pre = nullptr;
	INode* next = nullptr;
	double arrivalTime;
	double distanceToNext;
	double distanceToPre;
	double EAT;// earliest arrival time
	double LAT;// latest arrival time
public:
	enum NodeType getNodeType()
	{
		return nodeType;
	}
	virtual int getNodeNum()
	{
		return num;
	}
	INode* getPre() 
	{ 
		return pre; 
	}

	void setPre(INode* _pre) 
	{ 
		pre = _pre; 
	}

	INode * getNext() 
	{ 
		return next; 
	}

	void setNext(INode* _next) 
	{ 
		next = _next; 
	}

	double getArrivalTime()
	{
		return arrivalTime;
	}
	void setArrivalTime(double _arrivalTime) 
	{
		arrivalTime = _arrivalTime;
	}
	double getDistanceToNext() 
	{ 
		return distanceToNext;
	}
	void setDistanceToNext(double _distanceToNext)
	{
		distanceToNext = _distanceToNext;
	}
	double getDistanceToPre()
	{
		return distanceToPre;
	}
	void setDistanceToPre(double _distanceToPre)
	{
		distanceToPre = _distanceToPre;
	}
	double getEAT()
	{
		return EAT;
	}
	void setEAT(double _EAT)
	{
		EAT = _EAT;
	}
	double getLAT()
	{
		return LAT;
	}
	void setLAT(double _LAT)
	{
		LAT = _LAT;
	}
};