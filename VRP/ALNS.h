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
	int												total_vehicle_num;				// 5、总的摆渡车任务数量
	shared_ptr<Solution>							bestSol;						// 6、最优解目标函数

	// 当前解
	shared_ptr<Solution>							curSol;							// 7、当前解
	// 算子
	std::vector<double>								repair_grades; 					// 8、恢复算子得分
	std::vector<double>								repair_add;	  					// 9、恢复算子当前回合得分
	std::vector<int>								repair_count;					// 10、恢复算子使用次数
	std::vector<double>								destroy_grades;					// 11、破坏算子得分
	std::vector<double>								destroy_add;					// 12、破坏算子当前回合得分
	std::vector<int>								destroy_count;					// 13、破坏算子使用次数
	const double									ratio = ALNS_Setting::update_ratio;	// 14、新权重的占比

	FILE* file_;	//辅助输出文件

public:
	ALNS();
	void start();
	
	/* 恢复算子 */
	// 初始化插入算子
	void initialize_insertion();
	// 初始化插入算子增加分数和计数
	void initialize_insertion_add_and_count();
	// 通过轮盘赌选择当前使用算子
	int select_insertion(shared_ptr<Solution>& solution);
	// 混合插入算子
	void mix_insertion(shared_ptr<Solution>& solution);
	// 后悔值插入
	void regret_insertion(shared_ptr<Solution>& solution);
	void regret_insertion_once(shared_ptr<Solution>& solution);
	// 贪婪插入
	void greedy_insertion(shared_ptr<Solution>& solution);
	void greedy_insertion_once(shared_ptr<Solution>& solution);
	// 单点贪婪插入（每次只进行一个节点的贪婪插入，且这个节点随机选择）
	void single_greedy_insertion(shared_ptr<Solution>& solution);
	void single_greedy_insertion_once(shared_ptr<Solution>& solution);

	// 破坏算子
	// 初始化破坏算子
	void initialize_destroy();
	// 初始化破坏算子增加分数和计数
	void initialize_destroy_add_and_count();
	// 通过轮盘赌选择当前使用算子
	int select_destroy(shared_ptr<Solution>& solution);
	// 混合破坏算子
	void mix_destory(shared_ptr<Solution>& solution);
	// 随机破坏算子
	void random_destroy(shared_ptr<Solution>& solution);
	// 随机删除1个节点
	void random_destroy_once(shared_ptr<Solution>& solution);
	// 贪婪破解坏算子
	void greedy_destroy(shared_ptr<Solution>& solution);
	// 一次贪婪破解坏算子
	void greedy_destroy_once(shared_ptr<Solution>& solution);

	// 将当前解送入解空间
	SolutionFrequency push_solution_space(ICriterion* criterion, shared_ptr<Solution>& solution);

	// 模拟退火
	bool simulated_annealing(double curT, double curVal);
	
	// 更新参数信息
	void updateParameters(int times);

	// 更新得分情况
	void updateScores(int selected_repair_index, int selected_destroy_index, SolutionFrequency sf);

	/*辅助函数*/
	// 输出内容 0b01：cout；0b10：fcout
	void output(FILE* file, time_t time, SolutionFrequency sf, shared_ptr<Solution>& solution, int type = 0b11);
	void output(FILE* file, string content, int type = 0b11);
};
