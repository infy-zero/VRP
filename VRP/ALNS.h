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
	ICriterion										criterion;						// 1、迭代标准
	std::unordered_map<std::string, double>			solutionSpace;					// 2、解空间
	AllNodes*										nodes_;							// 3、所有节点
	int												depot_;							// 4、场站
	std::vector<std::vector<int>>					virtualFlight;					// 5、虚拟航班
	int												total_vehicle_num;				// 6、总的摆渡车任务数量
	Solution										bestSol;						// 7、最优解目标函数

	// 当前解
	Solution										curSol;							// 8、当前解
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
	
	// 恢复算子
	void greedyInsertion(Solution* solution = nullptr);

	void greedyInsertionOnce(Solution* solution);

	// 破坏算子
	void randomDestroy(Solution* solution = nullptr);

	// 计算目标函数
	double cal_objectives(Solution* solution = nullptr);

	// 将当前解送入解空间
	SolutionFrequency push_solution_space(ICriterion* criterion = nullptr, Solution* solution = nullptr);

	// 模拟退火
	bool simulated_annealing(double curT, double curVal);
	
	// 更新参数信息
	void updateParameters();

	// 更新得分情况
	void updateScores(SolutionFrequency sf);

	/*解的领域*/
	// 更新解的信息
	void updateSolution(Solution* solution = nullptr);

	// 更新当前解信息
	void update();

	// 判断当前解是否可行
	bool check_solution_feasible(Solution* solution = nullptr);
};
