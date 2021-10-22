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
	ISolutionNode(FerryVehicleTask* _task, enum NodeType type);
	// 将所有节点状态重置——重置为不可运行状态
	void reset_infeasible();
	// 将所有节点状态重置——重置为可运行状态
	void reset_feasible();
	// 计算最早到达时间
	double cal_arrive_earliest_time();	
	// 得到最早到达时间
	double get_arrive_earliest_time();
	// 重置节点，让节点所有时间信息

	/*私有域*/
	FerryVehicleTask* task;				// 1、任务

	/*针对vehicle序列*/
	// 最早系列
	double arrive_earliest_time;					// 2、最早到达时间
	double service_earliest_start_time;				// 3、服务最早开始时间
	double leave_earliest_time;						// 4、最早离开时间
	
	// 最晚系列
	double arrive_latest_time;						// 5、最早到达时间
	double service_latest_start_time;				// 6、服务最晚到达时间
	double leave_latest_time;						// 7、最早离开时间

	// 实际最晚时间
	double actual_arrive_latest_time_;				// 8、实际最晚到达时间
	double actual_service_latest_time_;				// 9、实际最晚开始服务时间
	double actual_leave_latest_time_;				// 10、实际最晚离开时间

	enum NodeState state;							// 11、当前节点状态
	enum NodeType  type_;							// 12、当前节点类型

	// 多行程
	int trip_num;									// 13、多行程编号
};
