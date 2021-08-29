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
struct Solution { // 解的结构
	double											curVal = DBL_MAX;				// 4、当前目标函数值
	vector<ISolutionNode>							nodes;							// 5、所有节点
	std::vector<std::vector<int>>					sol;		  					// 6、车辆顺序检验
	std::vector<int>								removedList; 					// 7、剩余节点
	int												depot;		  					// 8、场站序号

};
class ALNS
{
private:
	ICriterion										criterion;						// 1、迭代标准
	std::unordered_map<std::string, double>			solutionSpace;					// 2、解空间
	std::vector<std::vector<int>>					virtualFlight;					// 3、虚拟航班
	Solution										bestSol;						// 4、最优解目标函数

	// 当前解
	Solution										curSol;							// 5-8、当前解

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

	// 破坏算子
	void randomDestroy(Solution* solution = nullptr);

	// 计算目标函数
	double cal_objectives(Solution* solution = nullptr);

	// 将当前解送入解空间
	double push_solution_space(Solution* solution = nullptr);

	// 输出当前解
	std::string toString(Solution* solution = nullptr);
};
}// alns
