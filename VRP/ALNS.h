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
	ICriterion										criterion;						// 1、迭代标准
	std::unordered_map<std::string, double>			solutionSpace;					// 2、解空间
	AllNodes*										nodes_;							// 3、所有节点
	int												depot_;							// 4、场站
	std::vector<std::vector<int>>					virtualFlight;					// 5、虚拟航班
	int												total_vehicle_num;				// 6、总的摆渡车任务数量
	shared_ptr<Solution>							bestSol;						// 7、最优解目标函数

	// 当前解
	shared_ptr<Solution>							curSol;							// 8、当前解
	// 算子
	std::vector<double>								repairGrades; 					// 9、恢复算子得分
	std::vector<double>								repairAdd;	  					// 10、恢复算子当前回合得分
	std::vector<double>								destoryGrades;					// 11、破坏算子得分
	std::vector<double>								destoryAdd;						// 12、恢复算子当前回合得分
	const double									ratio = ALNS_Setting::ratio;	// 13、新权重的占比

public:
	ALNS();
	void start();
	void generateInitialSolution();
	
	/* 恢复算子 */
	// 随机插入
	void randomInsertion(shared_ptr<Solution>& solution);
	void randomInsertionOnce(shared_ptr<Solution>& solution);

	// 贪婪插入
	void greedyInsertion(shared_ptr<Solution>& solution);
	void greedyInsertionOnce(shared_ptr<Solution>& solution);

	// 破坏算子
	void randomDestroy(shared_ptr<Solution>& solution);

	// 计算目标函数
	double cal_objectives(shared_ptr<Solution>& solution);

	// 将当前解送入解空间
	SolutionFrequency push_solution_space(ICriterion* criterion, shared_ptr<Solution>& solution);

	// 模拟退火
	bool simulated_annealing(double curT, double curVal);
	
	// 更新参数信息
	void updateParameters();

	// 更新得分情况
	void updateScores(SolutionFrequency sf);

	/*解的领域*/

	// 更新当前解信息
	void update();

	// 判断当前解是否可行
	bool check_solution_feasible(shared_ptr<Solution>& solution);

	/*辅助函数*/
	void skip_flight_node_not_in_queue(const unordered_map<int, NodeLocation>::iterator& flight_iter,
		vector<vector<int>>& virtual_flight_consequence,
		unordered_set<int>* node_in_queue,
		unordered_map<int, NodeLocation>* flight_front);
};
