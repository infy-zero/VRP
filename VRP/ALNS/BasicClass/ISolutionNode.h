#pragma once

#include <memory>
#include <vector>

#include "FerryVehicleTask.h"
#include "ALNS_Setting.h"

enum NodeState { CHECKED, UNCHECKED, INREMOVELIST };   // 节点状态：在当前解、在回收列表

using namespace std;
/* 1、第一辆摆渡车提前5 - 8分钟到达，0分钟开始提供服务，0+1分钟结束服务；
	2、第二辆摆渡车[-2,1]分钟内到达,1分钟开始提供服务；
	3、第三辆摆渡车及以后在前一辆车离开时LT_n-1开始提供服务，在[LT_n-1 - 2, LT_n-1]时间内到达；
	4、头两辆车时间窗不可变，第三辆开始时间窗可变（不能超过相应的curTime）；
	5、时间窗分为两种：车辆序列时间窗、虚拟航班时间窗；
	6、车辆序列中的节点[minVT,maxVT]，根据虚拟航班时间窗和序列共同决定；
	7、虚拟航班中的节点[minFT,maxFT]，根据虚拟航班序列所有节点curTime共同决定；*/
class ISolutionNode
{
public:
	ISolutionNode() = default;
	ISolutionNode(const ISolutionNode& other) = default;
	ISolutionNode(const shared_ptr<FerryVehicleTask>& _task);

	shared_ptr<FerryVehicleTask> task;// 0、任务
	bool isUpdate = true;		// 1、是否序号更新

	double curTime;				// 2、本节点到达时间
	double minTime;				// 3、实际最早到达时间
	double maxTime;				// 4、实际最晚到达时间

	double minVehicleTime;		// 5、本节点最早到达时间（车辆）
	double maxVehicleTime;		// 6、本节点最晚到达时间（车辆）

	vector<pair<double, double>> vft; // 7、和虚拟航班对应的摆渡车时间窗
	double minFlightTime;		// 8、本节点最早到达时间（虚拟航班）
	double maxFlightTime;		// 9、本节点最晚到达时间（虚拟航班）

	enum NodeState state;		// 10、当前节点状态


};
