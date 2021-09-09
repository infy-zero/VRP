#pragma once

#include <memory>
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
	void addVehicle();																// function: ����ճ�
	void insert(int vehicle_number, int pos, int node);								// function: ��ָ��λ�ò���ڵ�
	bool isBetter(unique_ptr<Solution>& other);										// �Ƚ�����
	string toString();																// ���ɽ���ַ���

	double											curVal = DBL_MAX;				// 1����ǰĿ�꺯��ֵ
	AllNodes*										nodes_;							// 2�����нڵ�
	int 											depot_;							// 3����վ	
	vector<IVehicle>								vehicles;		  				// 4������˳��
	vector<int>										removedList; 					// 5��ʣ��ڵ�
	double											vehicle_cost = A_S vcost;		// 6������ʹ�÷���
	double											vehicle_per_cost = A_S pcost;	// 7��������λ����ɱ�
};

