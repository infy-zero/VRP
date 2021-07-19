#pragma once
#include <list>

#include "ISolutionNode.h"
using namespace std;
class IVehicle
{
private:
	bool update = true;	// 判断在进行删插造作后是否更新。
	list<ISolutionNode*> vehicles;	// 节点列表
	/* 存储最优位置 */
	list<ISolutionNode*>::iterator firstBestLoc; // 最好的位置
	list<ISolutionNode*>::iterator secondBestLoc; // 第二好的位置

public:
	bool isUpdate();
	list<ISolutionNode*>::iterator& getFirstBestLoc();
	list<ISolutionNode*>::iterator& getSecondBestLoc();
	void clearBestLoc();
	double getLength();

};