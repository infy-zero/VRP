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
	int												total_vehicle_num;				// 5���ܵİڶɳ���������
	shared_ptr<Solution>							bestSol;						// 6�����Ž�Ŀ�꺯��

	// ��ǰ��
	shared_ptr<Solution>							curSol;							// 7����ǰ��
	// ����
	std::vector<double>								repair_grades; 					// 8���ָ����ӵ÷�
	std::vector<double>								repair_add;	  					// 9���ָ����ӵ�ǰ�غϵ÷�
	std::vector<int>								repair_count;					// 10���ָ�����ʹ�ô���
	std::vector<double>								destroy_grades;					// 11���ƻ����ӵ÷�
	std::vector<double>								destroy_add;					// 12���ƻ����ӵ�ǰ�غϵ÷�
	std::vector<int>								destroy_count;					// 13���ƻ�����ʹ�ô���
	const double									ratio = ALNS_Setting::update_ratio;	// 14����Ȩ�ص�ռ��

	FILE* file_;	//��������ļ�

public:
	ALNS();
	void start();
	
	/* �ָ����� */
	// ��ʼ����������
	void initialize_insertion();
	// ��ʼ�������������ӷ����ͼ���
	void initialize_insertion_add_and_count();
	// ͨ�����̶�ѡ��ǰʹ������
	int select_insertion(shared_ptr<Solution>& solution);
	// ��ϲ�������
	void mix_insertion(shared_ptr<Solution>& solution);
	// ���ֵ����
	void regret_insertion(shared_ptr<Solution>& solution);
	void regret_insertion_once(shared_ptr<Solution>& solution);
	// ̰������
	void greedy_insertion(shared_ptr<Solution>& solution);
	void greedy_insertion_once(shared_ptr<Solution>& solution);
	// ����̰�����루ÿ��ֻ����һ���ڵ��̰�����룬������ڵ����ѡ��
	void single_greedy_insertion(shared_ptr<Solution>& solution);
	void single_greedy_insertion_once(shared_ptr<Solution>& solution);

	// �ƻ�����
	// ��ʼ���ƻ�����
	void initialize_destroy();
	// ��ʼ���ƻ��������ӷ����ͼ���
	void initialize_destroy_add_and_count();
	// ͨ�����̶�ѡ��ǰʹ������
	int select_destroy(shared_ptr<Solution>& solution);
	// ����ƻ�����
	void mix_destory(shared_ptr<Solution>& solution);
	// ����ƻ�����
	void random_destroy(shared_ptr<Solution>& solution);
	// ���ɾ��1���ڵ�
	void random_destroy_once(shared_ptr<Solution>& solution);
	// ̰���ƽ⻵����
	void greedy_destroy(shared_ptr<Solution>& solution);
	// һ��̰���ƽ⻵����
	void greedy_destroy_once(shared_ptr<Solution>& solution);

	// ����ǰ�������ռ�
	SolutionFrequency push_solution_space(ICriterion* criterion, shared_ptr<Solution>& solution);

	// ģ���˻�
	bool simulated_annealing(double curT, double curVal);
	
	// ���²�����Ϣ
	void updateParameters(int times);

	// ���µ÷����
	void updateScores(int selected_repair_index, int selected_destroy_index, SolutionFrequency sf);

	/*��������*/
	// ������� 0b01��cout��0b10��fcout
	void output(FILE* file, time_t time, SolutionFrequency sf, shared_ptr<Solution>& solution, int type = 0b11);
	void output(FILE* file, string content, int type = 0b11);
};
