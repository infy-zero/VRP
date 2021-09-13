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
	double cal_solution_cost();														// ����ɱ�
	void update_value();															// ���³ɱ�
	bool has_empty_vehicle();														// �Ƿ��пճ�
	void clear_empty_vehicle();														// ����ճ�
	void random_delete_without_update();											// ���ɾ���ڵ�
	void delete_without_update(int vehicle_pos, int node_pos);						// ɾ��ָ���ڵ�
	void addVehicle();																// function: ����ճ�
	void insert(int vehicle_number, int pos, int node);								// function: ��ָ��λ�ò���ڵ�
	bool isBetter(shared_ptr<Solution>& other);										// �Ƚ�����
	string toString();																// ���ɽ���ַ���
	string to_string_without_update();												// ���ɽ���ַ�������������

	double											curVal = DBL_MAX;				// 1����ǰĿ�꺯��ֵ
	AllNodes*										nodes_;							// 2�����нڵ�
	int 											depot_;							// 3����վ	
	vector<IVehicle>								vehicles;		  				// 4������˳��
	vector<int>										removedList; 					// 5��ʣ��ڵ�
	double											vehicle_cost = A_S vcost;		// 6������ʹ�÷���
	double											vehicle_per_cost = A_S pcost;	// 7��������λ����ɱ�
};

