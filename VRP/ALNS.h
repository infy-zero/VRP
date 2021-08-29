#pragma once
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <unordered_map>
#include "IInformation.h"
#include "BasicClass/ISolutionNode.h"
#include "ALNS/Util/ICriterion.h"

namespace alns {
struct Solution { // ��Ľṹ
	double											curVal = DBL_MAX;				// 4����ǰĿ�꺯��ֵ
	vector<ISolutionNode>							nodes;							// 5�����нڵ�
	std::vector<std::vector<int>>					sol;		  					// 6������˳�����
	std::vector<int>								removedList; 					// 7��ʣ��ڵ�
	int												depot;		  					// 8����վ���

};
class ALNS
{
private:
	ICriterion										criterion;						// 1��������׼
	std::unordered_map<std::string, double>			solutionSpace;					// 2����ռ�
	std::vector<std::vector<int>>					virtualFlight;					// 3�����⺽��
	Solution										bestSol;						// 4�����Ž�Ŀ�꺯��

	// ��ǰ��
	Solution										curSol;							// 5-8����ǰ��

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

	// �ƻ�����
	void randomDestroy(Solution* solution = nullptr);

	// ����Ŀ�꺯��
	double cal_objectives(Solution* solution = nullptr);

	// ����ǰ�������ռ�
	double push_solution_space(Solution* solution = nullptr);

	// �����ǰ��
	std::string toString(Solution* solution = nullptr);
};
}// alns
