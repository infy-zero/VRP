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

	// ȷ����ǰ��������vrp����Ҫ�󣨶ೡվ���೵�͵ȣ����ڲ��롢����ǰ��Ҫ���������
	void satisfy_condition();
	// �������б���
	void update(enum BasicOperator basic_operator = BasicOperator::DELETE_NODE);
	// ��ȡָ��λ�õĽڵ���ţ�vehicle_nodes.at(index);
	int get_node_index(int node_pos);
	// ָ��λ�ò���ڵ�
	void insert_node(int pos, int node, bool try_update = true);
	// ɾ���ڵ�
	void delete_node(int pos, vector<int>& removed_list);
	// ɾ����վ
	void delete_depot(int pos);
	// ���볡վ
	void insert_depot(int pos, bool try_update = true);

	double get_cost();
	bool beyond_max_length();
	
	// ���node�ڵ�����
	int get_node_size();
	// ������нڵ�����
	int get_total_size();
	// �жϵ�ǰ���Ƿ񱻸���
	bool get_is_update();
	// �жϵ�ǰ�����Ƿ�Ϊ�ճ����ڵ���С��2��ֱ�ӱ����ڵ��к��зǳ�վ�ڵ㣬ֱ�ӷ���false������ڵ���ֻ�г�վ�ڵ㣬����true
	bool is_empty();
	// �жϵ�ǰ�����Ƿ����
	bool check_operator_feasible(unique_ptr<InsertDescription>& operator_description);
	// ����ɾ���ڵ��Լֵ
	void cal_delete_save_value(unique_ptr<DeleteDescription>& current_operator_description);
	// ������г�վ�ڵ�
	void clear();

	// ��ȡ��������
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

	double total_cost_;						// �����ܷ���
	double total_length_;					// ������·��
	double total_time_;						// ��������ʱ
	double vehicle_use_cost_;				// �����ܳɱ�
	double vehicle_per_cost_;				// ·���ɱ�
	double vehicle_max_length_;				// �����ʻ���
	double vehicle_max_time_;				// �����ʻ���
	enum VehicleType vehicle_type_ = LARGE; // ��������
	bool is_update_ = false;				// �Ƿ����

	// ���г̱���
	vector<int> trip_node_num_;				// ÿ���г̽ڵ���
	vector<double> trip_length_;			// ÿ���г���ʻ�ܳ���
	vector<double> trip_time_;				// ÿ���г���ʻ��ʱ��

	int total_node_num_;					// ������ͨ�ڵ�����
	int total_depot_num_;					// ���г�վ�ڵ�����
};

