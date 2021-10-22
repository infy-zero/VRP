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

#define CONST_DEPOT_CODE 0b100
#define DEPOT_CODE 0b010
#define NODE_CODE 0b001

enum CalDistanceType {CDT_Task, CDT_Index};

static class IInformation
{
public:
	static AllNodes nodes;						// 1、所有订单+场站节点：BGS[0] + CA[1]

	static vector<vector<int>> nodes_indexs;

	static vector<vector<double>> matrix;		// 3、距离矩阵

	static unordered_map<char, int> flight2FerryVehcle;  // 4、类型-数量哈希表

	static vector<string> kIndexToPositionName;		// 5、角标-名称数组

	static unordered_map<string, int> kPositionNameToIndex;  // 6、名称-角标哈希表

	static vector<Flight*> flight_flights;			// 7、飞机

	static vector<pair<int, int>> positions;	// 8、每个节点对应的虚拟航班任务

	/*计算从from到to的旅行时间，单位：秒*/ // TODO(Lvning): 旅行时间应该是上一节点结束节点到下一节点开始节点
	static double cal_travel_time(AllNodes& nodes, int from_node_index, int to_node_index, int type = 0b011, double fromTime = 0);

	/*计算从from到to的旅行距离*/
	static double cal_travel_distance(AllNodes& nodes, int from_node_index, int to_node_index, int type = 0b011, double fromTime = 0);

};
