#pragma once
#include <vector>
#include <unordered_map>

#include "AllNodes.h"
#include "ALNS/BasicClass/ISolutionNode.h"
#include "ALNS_Setting.h"
#include "FerryVehicleTask.h"
#include "Flight.h"
#include "MyException.h"
#include "Util/Util.h"

#define inf IInformation::

static class IInformation
{
public:
	static AllNodes nodes;						// 1、所有订单+场站节点：BGS[0] + CA[1]

	static vector<vector<double>> matrix;		// 3、距离矩阵

	static unordered_map<char, int> flight2FerryVehcle;  // 4、类型-数量哈希表

	static vector<string> index2flightName;		// 5、角标-名称数组

	static unordered_map<string, int> flightName2index;  // 6、名称-角标哈希表

	static vector<Flight*> flightTasks;			// 7、飞机

	static vector<vector<int>> consequence;		// 8、约束

	static vector<pair<int, int>> positions;	// 9、每个节点对应的虚拟航班任务

	/*计算从from到to的旅行时间*/
	static double cal_travel_time(int pre, int next, double fromTime = 0)
	{
		int from = nodes.get(pre)->task->id;
		int to   = nodes.get(next)->task->id;
		return matrix[from][to] / A_S speed;
	}

	/*计算从from到to的旅行距离*/
	static double cal_travel_distance(int pre, int next)
	{
		int from = nodes.get(pre)->task->id;
		int to = nodes.get(next)->task->id;
		return matrix[from][to];
	}

	// 获得车辆成本
	static double get_vehicle_cost(enum VehicleType vehicle_type) {
		switch (vehicle_type) {
		case LARGE:
			return 1000;
		default:
			throw MyException("No vehicle type found!");
			return INT_MAX;
		}
	}
	// 获得车辆行驶成本
	static double get_vehicle_per_cost(enum VehicleType vehicle_type) {
		if (vehicle_type == LARGE) {
			return 1;
		}
		throw MyException("No vehicle type found!");
		return -DBL_MAX;
	}

	// 最大行驶距离
	static double get_vehicle_max_length(enum VehicleType vehicle_type) {
		switch (vehicle_type) {
		case LARGE:
			return 200000; // 最大行驶里程200km
		default:
			throw MyException("No vehicle type found!");
			return INT_MAX;
		}
	}
};
