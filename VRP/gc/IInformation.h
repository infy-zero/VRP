#pragma once
#include <vector>
#include <unordered_map>
#include "ALNS/BasicClass/ISolutionNode.h"
#include "FerryVehicleTask.h"
#include "ALNS/setting/ALNS_Setting.h"
#include "Flight.h"

#define inf IInformation::

using namespace std;
static class IInformation
{
public:
	static vector<FerryVehicleTask*> nodes;		// 1、所有任务节点

	static vector<FerryVehicleTask*> depots;	// 2、所有场站节点：BGS[0] + CA[1]
	
	static vector<vector<double>> matrix;		// 3、距离矩阵
	
	static unordered_map<char, int> flight2FerryVehcle;	 // 4、类型-数量哈希表
	
	static vector<string> index2flightName;		// 5、角标-名称数组
	
	static unordered_map<string, int> flightName2index;  // 6、名称-角标哈希表
	
	static vector<Flight*> flightTasks;			// 7、飞机
	
	static vector<vector<int>> consequence;		// 8、约束

	static vector<pair<int, int>> positions;   // 9、每个节点对应的虚拟航班任务

	/*计算从from到to的旅行时间*/
	static double calTravelTime(int from, int to, double fromTime = 0)
	{
		return matrix[from][to]/A_S speed;
	}
};