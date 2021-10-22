#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "AllNodes.h"
#include "ALNS_Setting.h"
#include "IInformation.h"
#include "ALNS/BasicClass/ISolutionNode.h"


using namespace std;

class IVehicle
{
public:
	IVehicle();
	IVehicle(int _depot, AllNodes* _nodes, bool is_initialize = true);
	IVehicle(const IVehicle& other) = default;

	// 确保当前车辆满足vrp类型要求（多场站、多车型等），在插入、更新前需要满足该条件
	void satisfy_condition();
	// 更新所有变量
	void update(enum BasicOperator basic_operator = BasicOperator::DELETE_NODE);
	// 获取指定位置的节点序号：vehicle_nodes.at(index);
	int get_node_index(int node_pos);
	// 指定位置插入节点
	void insert_node(int pos, int node, bool try_update = true);
	// 删除节点
	void delete_node(int pos, vector<int>& removed_list);
	// 删除场站
	void delete_depot(int pos);
	// 插入场站
	void insert_depot(int pos, bool try_update = true);

	double get_cost();
	bool beyond_max_length();
	
	// 获得node节点数量
	int get_node_size();
	// 获得所有节点数量
	int get_total_size();
	// 判断当前车是否被更新
	bool get_is_update();
	// 判断当前车辆是否为空车。节点数小于2，直接报错；节点中含有非场站节点，直接返回false；否则节点中只有场站节点，返回true
	bool is_empty();
	// 判断当前操作是否可行
	bool check_operator_feasible(unique_ptr<InsertDescription>& operator_description);
	// 计算删除节点节约值
	void cal_delete_save_value(unique_ptr<DeleteDescription>& current_operator_description);
	// 清空所有场站节点
	void clear();

	// 获取参数变量
	const double get_max_length();
	void set_all_nodes(AllNodes* all_nodes);
	int get_node_trip_pos(int node_pos);
private:
	void backforward_calculate(int cur_pos);
	void forward_calculate(int cur_pos);
	void backward_calculate_actual(int cur_pos);
private:
	vector<int> vehicle_nodes_;
	AllNodes* nodes_;
	int depot_;

	double total_cost_;						// 车辆总费用
	double total_length_;					// 车辆总路径
	double total_time_;						// 车辆总用时
	double vehicle_use_cost_;				// 车辆总成本
	double vehicle_per_cost_;				// 路径成本
	double vehicle_max_length_;				// 最大行驶里程
	double vehicle_max_time_;				// 最大行驶里程
	enum VehicleType vehicle_type_ = LARGE; // 车辆类型
	bool is_update_ = false;				// 是否更新

	// 多行程变量
	vector<int> trip_node_num_;				// 每个行程节点数
	vector<double> trip_length_;			// 每个行程行驶总长度
	vector<double> trip_time_;				// 每个行程行驶总时间

	int total_node_num_;					// 所有普通节点数量
	int total_depot_num_;					// 所有场站节点数量
};

