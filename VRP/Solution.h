#pragma once

#include <memory>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
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

	/*判断区域*/
	bool has_empty_vehicle();														// 是否有空车
	void clear_empty_vehicle();														// 清除空车
	bool isBetter(shared_ptr<Solution>& other);										// 比较优劣
	bool judge_not_exceed_max_length();												// 判断是否超过最大距离
	bool judge_sequence_feasible();													// 判断序列是否可行

	void random_delete_without_update();											// 随机删除节点
	void delete_without_update(int vehicle_pos, int node_pos);						// 删除指定节点
	void addVehicle();																// function: 插入空车
	void insert(int vehicle_number, int pos, int node);								// function: 在指定位置插入节点

	/*辅助函数*/
	// 跳过不在queue中的flight节点
	void skip_flight_node_not_in_queue(const unordered_map<int, NodeLocation>::iterator& flight_iter,
	  vector<vector<int>>& virtual_flight_consequence,
	  unordered_set<int>* node_in_queue,
	  unordered_map<int, NodeLocation>* flight_front);
	// 求两个时间窗交集
	bool cal_timewindow_intersection(int vehicle_start, int vehicle_end, int flight_start, int flight_end, double* inter_start, double* inter_end);
	// 根据节点的当前车辆位置和当前航班位置更新时间
	bool update_node_timewindow(const NodeLocation& vehicle_location, const NodeLocation& flight_location);

	string toString_with_update();																// 生成解的字符串
	string to_string_without_update();												// 生成解的字符串，但不更新

	double											curVal = DBL_MAX;				// 1、当前目标函数值
	AllNodes*										all_nodes_;							// 2、所有节点
	int 											depot_;							// 3、场站	
	vector<IVehicle>								vehicles;		  				// 4、车辆顺序
	vector<int>										removedList; 					// 5、剩余节点
	double											vehicle_cost = A_S vcost;		// 6、车辆使用费用
	double											vehicle_per_cost = A_S pcost;	// 7、车辆单位距离成本
};

