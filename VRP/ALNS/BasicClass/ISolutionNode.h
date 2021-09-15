#pragma once

#include <memory>
#include <vector>

#include "FerryVehicleTask.h"
#include "ALNS_Setting.h"

// 节点状态：未知、已被检查node、已被检查depot、未被检查、在回收列表
enum NodeState { UNKNOWN, CHECKED_NODE, CHECKED_DEPOT, UNCHECKED_NODE, UNCHECKED_DEPOT, INREMOVELIST };
// 节点类型：场站、普通节点
enum NodeType { TYPE_DEPOT, TYPE_NODE, ERROR_TYPE };

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
	ISolutionNode(const shared_ptr<FerryVehicleTask>& _task, enum NodeType type);
	void reset_infeasible();
	void reset_feasible();
	double cal_arrive_earliest_time();				// 计算最早到达时间

	/*私有域*/
	shared_ptr<FerryVehicleTask> task;				// 0、任务

	/*针对vehicle序列*/
	double curTime;									// 1、本节点到达时间
	double arrive_earliest_time;					// 2、是否序号更新
	double service_earliest_start_time;				// 3、实际最早到达时间
	double service_latest_end_time;					// 4、实际最晚到达时间

	double minVehicleTime;							// 5、本节点最早到达时间（车辆）
	double maxVehicleTime;							// 6、本节点最晚到达时间（车辆）

	vector<pair<double, double>> vft;				// 7、和虚拟航班对应的摆渡车时间窗
	double minFlightTime;							// 8、本节点最早到达时间（虚拟航班）
	double maxFlightTime;							// 9、本节点最晚到达时间（虚拟航班）

	enum NodeState state;							// 10、当前节点状态
	enum NodeType  type_;							// 11、当前节点类型

};
