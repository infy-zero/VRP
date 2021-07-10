#pragma once
#include <vector>
#include "INode.h"
using namespace std;
#include "FerryVehicleTask.h"
class Node: INode
{
private:
	FerryVehicleTask& ferryVehicleTask;
	int vehicleNum;
	int tripNum;
	int segmentNum;
public:
	int getNodeNum()
	{
		return ferryVehicleTask.getID();
	}
	int getVehicleNum()
	{
		return vehicleNum;
	}
	int getTripNum()
	{
		return tripNum;
	}
	int getSegmentNum()
	{
		return segmentNum;
	}
	// �����ṩ�Ľڵ㣬��������
	void update(INode pre)
	{
		cout << "���½ڵ����Լ�λ��" << endl;
	}
	
};