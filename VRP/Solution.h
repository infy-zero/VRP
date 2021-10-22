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

	void update();															// ���³ɱ�
	// ɾ���ڵ����Ҫ����
	void update_after_delete();

	/*�ж�����*/
	// �Ƿ��пճ�
	bool has_empty_vehicle();														
	// ����ճ�
	void clear_empty_vehicle();														
	// �Ƚ�����
	const bool isBetter(shared_ptr<Solution>& other);
	const bool is_worse(shared_ptr<Solution>& other);												

	// ���ɾ���ڵ�
	void random_delete_without_update();											
	// ɾ��ָ���ڵ�
	void delete_without_update(int vehicle_pos, int node_pos);						
	// function: ����ճ�
	void add_empty_vehicle();																
	// function: ��ָ��λ�ò���ڵ�
	void insert(int vehicle_number, int pos, int node);								
	// �õ��ճ�����
	const int get_empty_vehicle_num();												
	// ȷ��ֻ��һ���ճ�
	void make_single_empty_vehicle();												

	/*��������*/
	// ��������queue�е�flight�ڵ�
	string toString_with_update();													// ���ɽ���ַ���
	string to_string_without_update();												// ���ɽ���ַ�������������
	const int get_total_node_num();													// ��ýڵ�����
	// ��������all_nodesһ�£���Ҫ�ڸı�vehicleʱʹ��
	void unify_vehicles_nodes();
	// �����г�����������
	void satisfy_condition();
	// �ֶ�������ж���
	void clear();

	// �жϲ���ڵ��Ƿ����
	bool check_operator_feasible(unique_ptr<InsertDescription>& operator_description);
	// ִ�в���ڵ����
	void excute_insert_operator(unique_ptr<InsertDescription>& operator_description);
	// ����ɾ���ڵ�Ľ�Լֵ
	void cal_delete_save_value(unique_ptr<DeleteDescription>& current_operator_description);
	// ִ��ɾ���ڵ����
	void excute_insert_operator(unique_ptr<DeleteDescription>& current_operator_description);

	double											cur_val_ = DBL_MAX;				// 1����ǰĿ�꺯��ֵ
	AllNodes										all_nodes_;						// 2�����нڵ�
	int 											depot_=INT_MIN;					// 3����վ	
	vector<IVehicle*>								vehicles;		  				// 4������˳��
	vector<int>										removedList; 					// 5��ʣ��ڵ�
	double											total_cost = DBL_MAX;			// 6����ǰ�����
	string											solution_string_;				// 7����ǰ���ַ���

};

