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
	// 根据提供的节点，更新属性
	void update(INode pre)
	{
		cout << "更新节点属性及位置" << endl;
	}
	
};