#pragma once

#include <memory>
#include <sstream>
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
	void update_value();															// 更新成本
	bool has_empty_vehicle();														// 是否有空车
	void clear_empty_vehicle();														// 清除空车
	void random_delete_without_update();											// 随机删除节点
	void delete_without_update(int vehicle_pos, int node_pos);						// 删除指定节点
	void addVehicle();																// function: 插入空车
	void insert(int vehicle_number, int pos, int node);								// function: 在指定位置插入节点
	bool isBetter(shared_ptr<Solution>& other);										// 比较优劣
	string toString();																// 生成解的字符串
	string to_string_without_update();												// 生成解的字符串，但不更新

	double											curVal = DBL_MAX;				// 1、当前目标函数值
	AllNodes*										nodes_;							// 2、所有节点
	int 											depot_;							// 3、场站	
	vector<IVehicle>								vehicles;		  				// 4、车辆顺序
	vector<int>										removedList; 					// 5、剩余节点
	double											vehicle_cost = A_S vcost;		// 6、车辆使用费用
	double											vehicle_per_cost = A_S pcost;	// 7、车辆单位距离成本
};

