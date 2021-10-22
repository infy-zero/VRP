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

class MyLocation // ����ͳ�Ʋ���ڵ��λ�� nodeNum��vehNum��pos��saveValue
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
		// cout << "ִ�и�ֵ" << this << endl;
	};
	MyLocation(const MyLocation& other) :nodeNum(other.nodeNum), vehNum(other.vehNum), pos(other.pos), saveValue(other.saveValue)
	{
		// cout << "ִ�����ÿ���" << endl;
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
	double save_value_ = -DBL_MAX; // ��Լֵ������ʱ<0���Ƴ�ʱ>0���ɵļ�ȥ�µģ�Խ��Խ��
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
	double save_value_ = -DBL_MAX; // ��Լֵ������ʱ<0���Ƴ�ʱ>0���ɵļ�ȥ�µģ�Խ��Խ��
};



#define A_S ALNS_Setting::

using namespace std;

static class ALNS_Setting
{
public:
	// ���Ŀ¼
	static string output_dir;
	static string input_dir;
	static constexpr int PRINT_FRQ = 50;	// ��ӡƵ��

	// �����Ľṹ
	static constexpr bool multi_trip = 1;
	static constexpr bool multi_vehicle_type = false;
	static constexpr bool forward_insert = true;

	// Ŀ�꺯������
	static constexpr double vcost = 1000; // �����̶��ɱ� , ��information��
	static constexpr double pcost = 0.005; // ������ʻ��λ����ɱ� , ��information��
	static constexpr int max_count = 100; // ������ڵ�����
	static constexpr int max_iteration = 1000; // ����������

	static constexpr double dbl_max_value = 10000000.0;				// Ŀ�꺯��ֵ���½�
	static constexpr double time_max_value = 1000000.0;				// ʱ�����½磨[-10h, 10h]��
	static constexpr double time_minimum_delta = 0.00000001;				// ��Сʱ�侫��

	// ���ӳ�ʼȨ��
	static constexpr double greedy_insertion_weight = 1.0;
	static constexpr double single_greedy_insertion_weight = 1.0;
	static constexpr double regret_insertion_weight = 1.0;

	static constexpr double random_removal_weight = 1.0;
	static constexpr double greedy_removal_weight = 1.0;

	// ��Ӧ�����
	static constexpr double r = 0.2; // ��������
	static constexpr int segment = 3; // ����Ƶ��
	static constexpr double initWeight = 1.0; // ��ʼȨ��
	static constexpr double best = 10;
	static constexpr double never = 5;
	static constexpr double accept = 1;
	static constexpr double reject = 0.5;
	static constexpr double update_ratio = 0.2;
	
	// ��������
	static unordered_map<char, int> flight_type_to_count;

	// ��������
	static constexpr double startT = 100000;		// ��ʼ�¶�	
	static constexpr double endT = 10;				// �����¶�
	static constexpr double decay = 0.95;			// ��������
	static constexpr int decayTimes = 10;			// ÿ�ν��µ�������+��������
	// static constexpr int additionalTimes = 5;

	// Լ������
	static constexpr double maxLength = 200000;     // ������
	static constexpr double maxTime = 3600 * 4;		// ÿ���г����ʱ��
	static constexpr double speed = 30 / 3.6;       // ƽ����ʻ�ٶ�
	static std::string to_string(enum OperatorType ot);

	// ��������
	static constexpr enum ScopeRange range = EAST; // ��Χ-����/����/���
	static constexpr int startIndex = 0;	// ��ʼ����
	static constexpr int counts = 31;	// ��ֹ����

	// ��ó����ɱ�
	static double get_vehicle_cost(enum VehicleType vehicle_type);
	
	// ��ó�����ʻ�ɱ�
	static double get_vehicle_per_cost(enum VehicleType vehicle_type);

	// �����ʻ����
	static double get_vehicle_max_length(enum VehicleType vehicle_type);

	// �����ʻʱ��
	static double get_vehicle_max_time(enum VehicleType vehicle_type);

	// ��õ÷�
	static double get_grade_addition(enum SolutionFrequency sf);
	
	// ��õ÷�����
	static string SolutionFrequency_to_string(enum SolutionFrequency sf);
};
