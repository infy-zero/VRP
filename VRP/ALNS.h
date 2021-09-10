#pragma once
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <unordered_map>

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
	Solution										bestSol;						// 7�����Ž�Ŀ�꺯��

	// ��ǰ��
	Solution										curSol;							// 8����ǰ��
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
	
	// �ָ�����
	void greedyInsertion(Solution* solution = nullptr);

	void greedyInsertionOnce(Solution* solution);

	// �ƻ�����
	void randomDestroy(Solution* solution = nullptr);

	// ����Ŀ�꺯��
	double cal_objectives(Solution* solution = nullptr);

	// ����ǰ�������ռ�
	SolutionFrequency push_solution_space(ICriterion* criterion = nullptr, Solution* solution = nullptr);

	// ģ���˻�
	bool simulated_annealing(double curT, double curVal);
	
	// ���²�����Ϣ
	void updateParameters();

	// ���µ÷����
	void updateScores(SolutionFrequency sf);

	/*�������*/
	// ���½����Ϣ
	void updateSolution(Solution* solution = nullptr);

	// ���µ�ǰ����Ϣ
	void update();

	// �жϵ�ǰ���Ƿ����
	bool check_solution_feasible(Solution* solution = nullptr);
};
