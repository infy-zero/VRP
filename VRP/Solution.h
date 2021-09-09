#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "AllNodes.h"
#include "IVehicle.h"
#include "ALNS_Setting.h"
#include "ALNS/BasicClass/ISolutionNode.h"

using namespace std;

class Solution
{
public:
	Solution() = default;
	Solution(int depot, AllNodes* nodes);
	Solution(const Solution&) = default;
	double cal_solution_cost();														// 计算成本
	void addVehicle();																// function: 插入空车
	void insert(int vehicle_number, int pos, int node);								// function: 在指定位置插入节点
	bool isBetter(unique_ptr<Solution>& other);										// 比较优劣
	string toString();																// 生成解的字符串

	double											curVal = DBL_MAX;				// 1、当前目标函数值
	AllNodes*										nodes_;							// 2、所有节点
	int 											depot_;							// 3、场站	
	vector<IVehicle>								vehicles;		  				// 4、车辆顺序
	vector<int>										removedList; 					// 5、剩余节点
	double											vehicle_cost = A_S vcost;		// 6、车辆使用费用
	double											vehicle_per_cost = A_S pcost;	// 7、车辆单位距离成本
};

