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

	double cal_solution_cost();														// ����ɱ�
	void update_value();															// ���³ɱ�

	/*�ж�����*/
	bool has_empty_vehicle();														// �Ƿ��пճ�
	void clear_empty_vehicle();														// ����ճ�
	bool isBetter(shared_ptr<Solution>& other);										// �Ƚ�����
	bool judge_not_exceed_max_length();												// �ж��Ƿ񳬹�������
	bool judge_sequence_feasible();													// �ж������Ƿ����

	void random_delete_without_update();											// ���ɾ���ڵ�
	void delete_without_update(int vehicle_pos, int node_pos);						// ɾ��ָ���ڵ�
	void addVehicle();																// function: ����ճ�
	void insert(int vehicle_number, int pos, int node);								// function: ��ָ��λ�ò���ڵ�

	/*��������*/
	// ��������queue�е�flight�ڵ�
	void skip_flight_node_not_in_queue(const unordered_map<int, NodeLocation>::iterator& flight_iter,
	  vector<vector<int>>& virtual_flight_consequence,
	  unordered_set<int>* node_in_queue,
	  unordered_map<int, NodeLocation>* flight_front);
	// ������ʱ�䴰����
	bool cal_timewindow_intersection(int vehicle_start, int vehicle_end, int flight_start, int flight_end, double* inter_start, double* inter_end);
	// ���ݽڵ�ĵ�ǰ����λ�ú͵�ǰ����λ�ø���ʱ��
	bool update_node_timewindow(const NodeLocation& vehicle_location, const NodeLocation& flight_location);

	string toString_with_update();																// ���ɽ���ַ���
	string to_string_without_update();												// ���ɽ���ַ�������������

	double											curVal = DBL_MAX;				// 1����ǰĿ�꺯��ֵ
	AllNodes*										all_nodes_;							// 2�����нڵ�
	int 											depot_;							// 3����վ	
	vector<IVehicle>								vehicles;		  				// 4������˳��
	vector<int>										removedList; 					// 5��ʣ��ڵ�
	double											vehicle_cost = A_S vcost;		// 6������ʹ�÷���
	double											vehicle_per_cost = A_S pcost;	// 7��������λ����ɱ�
};

