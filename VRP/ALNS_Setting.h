#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "MyException.h"
#include "Util/Util.h"

enum SolutionFrequency { BEST, NEVER_BUT_BETTER, SA_ACCEPT, SA_REJECT };
enum OperatorType { DESTROY, REPAIR };
enum ScopeRange { WEST, EAST, MIX };
enum RepairOperatorNum { GREEDYREPAIR };
enum DestroyOperatorNum { GREEDYDESTROY };
enum BasicOperator { INSERT_NODE, DELETE_NODE };

class MyLocation // 用来统计插入节点的位置 nodeNum、vehNum、pos、saveValue
{
public:
	MyLocation& operator = (const MyLocation& other)
	{
		nodeNum = other.nodeNum;
		vehNum = other.vehNum;
		pos = other.pos;
		saveValue = other.saveValue;
		return *this;
	};
	friend std::ostream& operator<<(std::ostream& o, const MyLocation& ml)
	{
		o << ml.nodeNum << "," << ml.vehNum << "," << ml.pos << "," << ml.saveValue << std::endl;
		return o;
	};
	MyLocation(int _nodeNum = -1, int _vehNum = -1, int _pos = -1, double _saveValue = -DBL_MAX) :nodeNum(_nodeNum), vehNum(_vehNum), pos(_pos), saveValue(_saveValue)
	{
		// cout << "执行赋值" << this << endl;
	};
	MyLocation(const MyLocation& other) :nodeNum(other.nodeNum), vehNum(other.vehNum), pos(other.pos), saveValue(other.saveValue)
	{
		// cout << "执行引用拷贝" << endl;
	};
	int nodeNum;
	int vehNum;
	int pos;
	double saveValue;
};
class RepairOperatorInformation
{
public:
	double weight;
	double grade;
	enum RepairOperatorNum type;
	RepairOperatorInformation(double _weight, double _grade, enum RepairOperatorNum _type) :weight(_weight), grade(_grade), type(_type) {}
	static RepairOperatorInformation build(double _weight, double _grade, enum RepairOperatorNum _type)
	{
		RepairOperatorInformation result(_weight, _grade, _type);
		return result;
	}
};
class DestroyOperatorInformation
{
public:
	double weight;
	double grade;
	enum DestroyOperatorNum type;
	DestroyOperatorInformation(double _weight, double _grade, enum DestroyOperatorNum _type) :weight(_weight), grade(_grade), type(_type) {}
	static DestroyOperatorInformation build(double _weight, double _grade, enum DestroyOperatorNum _type)
	{
		DestroyOperatorInformation result(_weight, _grade, _type);
		return result;
	}
};
class InsertDescription {
public:
	InsertDescription(enum BasicOperator basic_operator, int vehicle_pos, int node_pos, int node_index, int removed_list_pos):basic_operator_(basic_operator),vehicle_pos_(vehicle_pos), node_pos_(node_pos), node_index_(node_index), removed_list_pos_(removed_list_pos){
	}
	friend ostream& operator<<(ostream& out, const InsertDescription& od) {
		out << "operator=" << od.basic_operator_ << ", node_index" << od.node_index_ << ", veh_pos=" << od.vehicle_pos_ << ", node_pos=" << od.node_pos_ << ", removed_list_pos=" << od.removed_list_pos_ << ", save_value=" << od.save_value_ << endl;
		return out;
	}
	bool is_better(const unique_ptr<InsertDescription>& other) {
		if (other == nullptr) {
			return true;
		} else {
			if (save_value_ > other->save_value_) {
				return true;
			} else{
				return false;
			}
		}
	}

	enum BasicOperator basic_operator_;
	int vehicle_pos_	= -1;
	int node_pos_		= -1;
	int node_index_		= INT_MIN;
	int removed_list_pos_ = -1;
	double save_value_ = -DBL_MAX; // 节约值：插入时<0，移除时>0。旧的减去新的，越大越好
};
class DeleteDescription {
public:
	DeleteDescription(enum BasicOperator basic_operator, int vehicle_pos, int node_pos, int node_index) :basic_operator_(basic_operator), vehicle_pos_(vehicle_pos), node_pos_(node_pos), node_index_(node_index) {
	}
	friend ostream& operator<<(ostream& out, const DeleteDescription& od) {
		out << "operator=" << od.basic_operator_ << ", node_index" << od.node_index_ << ", veh_pos=" << od.vehicle_pos_ << ", node_pos=" << od.node_pos_ << ", save_value=" << od.save_value_ << endl;
		return out;
	}
	bool is_better(const unique_ptr<DeleteDescription>& other) {
		if (other == nullptr) {
			return true;
		}
		else {
			if (save_value_ > other->save_value_) {
				return true;
			}
			else {
				return false;
			}
		}
	}

	enum BasicOperator basic_operator_ = DELETE_NODE;
	int vehicle_pos_ = -1;
	int node_pos_ = -1;
	int node_index_ = INT_MIN;
	double save_value_ = -DBL_MAX; // 节约值：插入时<0，移除时>0。旧的减去新的，越大越好
};



#define A_S ALNS_Setting::

using namespace std;

static class ALNS_Setting
{
public:
	// 输出目录
	static string output_dir;
	static string input_dir;
	static constexpr int PRINT_FRQ = 50;	// 打印频率

	// 输出解的结构
	static constexpr bool multi_trip = 1;
	static constexpr bool multi_vehicle_type = false;
	static constexpr bool forward_insert = true;

	// 目标函数参数
	static constexpr double vcost = 1000; // 车辆固定成本 , 在information中
	static constexpr double pcost = 0.005; // 车辆行驶单位距离成本 , 在information中
	static constexpr int max_count = 100; // 最大服务节点数数
	static constexpr int max_iteration = 1000; // 最大迭代次数

	static constexpr double dbl_max_value = 10000000.0;				// 目标函数值上下界
	static constexpr double time_max_value = 1000000.0;				// 时间上下界（[-10h, 10h]）
	static constexpr double time_minimum_delta = 0.00000001;				// 最小时间精度

	// 算子初始权重
	static constexpr double greedy_insertion_weight = 1.0;
	static constexpr double single_greedy_insertion_weight = 1.0;
	static constexpr double regret_insertion_weight = 1.0;

	static constexpr double random_removal_weight = 1.0;
	static constexpr double greedy_removal_weight = 1.0;

	// 适应层参数
	static constexpr double r = 0.2; // 更新速率
	static constexpr int segment = 3; // 更新频率
	static constexpr double initWeight = 1.0; // 初始权重
	static constexpr double best = 10;
	static constexpr double never = 5;
	static constexpr double accept = 1;
	static constexpr double reject = 0.5;
	static constexpr double update_ratio = 0.2;
	
	// 参数设置
	static unordered_map<char, int> flight_type_to_count;

	// 迭代参数
	static constexpr double startT = 100000;		// 初始温度	
	static constexpr double endT = 10;				// 结束温度
	static constexpr double decay = 0.95;			// 降温速率
	static constexpr int decayTimes = 10;			// 每次降温迭代次数+更新速率
	// static constexpr int additionalTimes = 5;

	// 约束参数
	static constexpr double maxLength = 200000;     // 最大里程
	static constexpr double maxTime = 3600 * 4;		// 每个行程最大时间
	static constexpr double speed = 30 / 3.6;       // 平均行驶速度
	static std::string to_string(enum OperatorType ot);

	// 问题描述
	static constexpr enum ScopeRange range = EAST; // 范围-东区/西区/混合
	static constexpr int startIndex = 0;	// 起始坐标
	static constexpr int counts = 31;	// 终止坐标

	// 获得车辆成本
	static double get_vehicle_cost(enum VehicleType vehicle_type);
	
	// 获得车辆行驶成本
	static double get_vehicle_per_cost(enum VehicleType vehicle_type);

	// 最大行驶距离
	static double get_vehicle_max_length(enum VehicleType vehicle_type);

	// 最大行驶时间
	static double get_vehicle_max_time(enum VehicleType vehicle_type);

	// 获得得分
	static double get_grade_addition(enum SolutionFrequency sf);
	
	// 获得得分中文
	static string SolutionFrequency_to_string(enum SolutionFrequency sf);
};
