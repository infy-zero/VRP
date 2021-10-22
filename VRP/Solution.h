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
	Solution(int depot, AllNodes nodes);
	Solution(const Solution& other);
	~Solution();

	void update();															// 更新成本
	// 删除节点后需要更新
	void update_after_delete();

	/*判断区域*/
	// 是否有空车
	bool has_empty_vehicle();														
	// 清除空车
	void clear_empty_vehicle();														
	// 比较优劣
	const bool isBetter(shared_ptr<Solution>& other);
	const bool is_worse(shared_ptr<Solution>& other);												

	// 随机删除节点
	void random_delete_without_update();											
	// 删除指定节点
	void delete_without_update(int vehicle_pos, int node_pos);						
	// function: 插入空车
	void add_empty_vehicle();																
	// function: 在指定位置插入节点
	void insert(int vehicle_number, int pos, int node);								
	// 得到空车数量
	const int get_empty_vehicle_num();												
	// 确保只有一辆空车
	void make_single_empty_vehicle();												

	/*辅助函数*/
	// 跳过不在queue中的flight节点
	string toString_with_update();													// 生成解的字符串
	string to_string_without_update();												// 生成解的字符串，但不更新
	const int get_total_node_num();													// 获得节点总数
	// 保持所有all_nodes一致，需要在改变vehicle时使用
	void unify_vehicles_nodes();
	// 让所有车辆满足条件
	void satisfy_condition();
	// 手动清除所有东西
	void clear();

	// 判断插入节点是否可行
	bool check_operator_feasible(unique_ptr<InsertDescription>& operator_description);
	// 执行插入节点操作
	void excute_insert_operator(unique_ptr<InsertDescription>& operator_description);
	// 计算删除节点的节约值
	void cal_delete_save_value(unique_ptr<DeleteDescription>& current_operator_description);
	// 执行删除节点操作
	void excute_insert_operator(unique_ptr<DeleteDescription>& current_operator_description);

	double											cur_val_ = DBL_MAX;				// 1、当前目标函数值
	AllNodes										all_nodes_;						// 2、所有节点
	int 											depot_=INT_MIN;					// 3、场站	
	vector<IVehicle*>								vehicles;		  				// 4、车辆顺序
	vector<int>										removedList; 					// 5、剩余节点
	double											total_cost = DBL_MAX;			// 6、当前解费用
	string											solution_string_;				// 7、当前解字符串

};

