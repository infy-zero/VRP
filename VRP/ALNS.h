#pragma once
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <unordered_map>
#include <unordered_set>

#include "AllNodes.h"
#include "ALNS/Util/Util.h"
#include "ALNS/Util/ICriterion.h"
#include "BasicClass/ISolutionNode.h"
#include "IInformation.h"
#include "Solution.h"

struct InsertPosition
{
	int inserted_pos = -1;
	int vehicle_num  = -1;
	int vheicle_pos  = -1;
};

class ALNS
{
private:
	ICriterion										criterion;						// 1��������׼
	std::unordered_map<std::string, double>			solutionSpace;					// 2����ռ�
	AllNodes*										nodes_;							// 3�����нڵ�
	int												depot_;							// 4����վ
	std::vector<std::vector<int>>					virtualFlight;					// 5�����⺽��
	int												total_vehicle_num;				// 6���ܵİڶɳ���������
	shared_ptr<Solution>							bestSol;						// 7�����Ž�Ŀ�꺯��

	// ��ǰ��
	shared_ptr<Solution>							curSol;							// 8����ǰ��
	// ����
	std::vector<double>								repairGrades; 					// 9���ָ����ӵ÷�
	std::vector<double>								repairAdd;	  					// 10���ָ����ӵ�ǰ�غϵ÷�
	std::vector<double>								destoryGrades;					// 11���ƻ����ӵ÷�
	std::vector<double>								destoryAdd;						// 12���ָ����ӵ�ǰ�غϵ÷�
	const double									ratio = ALNS_Setting::ratio;	// 13����Ȩ�ص�ռ��

public:
	ALNS();
	void start();
	void generateInitialSolution();
	
	/* �ָ����� */
	// �������
	void randomInsertion(shared_ptr<Solution>& solution);
	void randomInsertionOnce(shared_ptr<Solution>& solution);

	// ̰������
	void greedyInsertion(shared_ptr<Solution>& solution);
	void greedyInsertionOnce(shared_ptr<Solution>& solution);

	// �ƻ�����
	void randomDestroy(shared_ptr<Solution>& solution);

	// ����Ŀ�꺯��
	double cal_objectives(shared_ptr<Solution>& solution);

	// ����ǰ�������ռ�
	SolutionFrequency push_solution_space(ICriterion* criterion, shared_ptr<Solution>& solution);

	// ģ���˻�
	bool simulated_annealing(double curT, double curVal);
	
	// ���²�����Ϣ
	void updateParameters();

	// ���µ÷����
	void updateScores(SolutionFrequency sf);

	/*�������*/

	// ���µ�ǰ����Ϣ
	void update();

	// �жϵ�ǰ���Ƿ����
	bool check_solution_feasible(shared_ptr<Solution>& solution);

	/*��������*/
	void skip_flight_node_not_in_queue(const unordered_map<int, NodeLocation>::iterator& flight_iter,
		vector<vector<int>>& virtual_flight_consequence,
		unordered_set<int>* node_in_queue,
		unordered_map<int, NodeLocation>* flight_front);
};
