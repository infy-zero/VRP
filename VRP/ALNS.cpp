#include "ALNS.h"

#include <algorithm>
#include <ctime>
#include <memory>

#include "ALNS/Util/Util.h"
#include "MyException.h"

ALNS::ALNS():virtualFlight(inf consequence)
{
	cout << "启动 ALNS" << endl;
	// 确定摆渡车任务（车辆数）上限，超出此上限则认为订单或任务有问题
	total_vehicle_num = inf nodes.nodes_.size();
	// 生成初始解，注入information
	nodes_ = &inf nodes;
	// 将所有任务压入RemovedList
	for (int i = 0; i < nodes_->nodes_.size(); i++) {
		int id = nodes_->nodes_.at(i).task->id;// 摆渡车任务
		curSol.removedList.push_back(id);
	}
	// 场站数据
	if (A_S range == EAST) {
		depot_ = -2;
	}
	else {
		depot_ = -1;
	}
	// 将场站号、所有节点注入solution
	curSol.depot_ = depot_;
	curSol.nodes_ = nodes_;
	cout << "ALNS就绪" << endl;
}

void ALNS::start()
{
	/*自适应大领域搜索在大领域搜索的基础上增加了更多的破坏和恢复算子，同时为这些算子赋予权重和增加了一个适应层，在适应层中需要控制迭代：更新比例r、segment number 更新周期，抽选算子准则，算子计分方法*/
	cout << "    开始ALNS算法" << std::endl;
	clock_t time = clock();
	/*生成初始解——利用恢复算子+约束条件*/
	cout << "		生成初始解" << std::endl;
	greedyInsertion();
	cout << "		初始解生成完毕" << std::endl;
	cal_objectives();									// 计算当前目标函数值
	push_solution_space();								// 加入解空间

	/*开始迭代——达到停止标准之前*/
	cout << criterion.toString() << std::endl;
	while (criterion.iter())
	{
		// 构建新解
		Solution newSol = curSol;
		// 破坏当前解
		randomDestroy(&newSol);
		// 恢复当前解
		greedyInsertion(&newSol);
		// 更新newSol信息
		updateSolution(&newSol);
		// 判断是否更新新解
		SolutionFrequency sf = push_solution_space(&criterion, &newSol);
		string solutionString = newSol.toString();
		if (sf == BEST) // 最优解直接插入SP，更新当前解，全局最优解
		{
			solutionSpace.insert(make_pair(solutionString, newSol.curVal));
			curSol  = newSol;
			bestSol = newSol;
		} else if (sf == NEVER_BUT_BETTER) { // 比当前解更好，则插入SP
			solutionSpace.insert(make_pair(solutionString, newSol.curVal));
			curSol = newSol;
		} else if (sf == SA_ACCEPT) {		// 没有当前解好，但是根据模拟退火准则接受
			solutionSpace.insert(make_pair(solutionString, newSol.curVal));
			curSol = newSol;
		}
		else {								// 拒绝当前解
			
		}
		// 更新当前解的分数
		updateScores(sf);
		// 更新当前状态
		update();
		// 更新 Adaptive layer 参数
		if (criterion.isUpdateParamter()) {
			updateParameters();
		}
	}
	std::cout << "    迭代结束，共计迭代次数：" << criterion.getTotalTimes() << std::endl;
	std::cout << "当前目标函数值 = " << curSol.curVal << "，当前车辆数 = " << curSol.vehicles.size() << "，当前解 = " << curSol.toString();
	std::cout << "最优值 = " << bestSol.curVal << "，最优解 = " << bestSol.toString();
	cout << "用时共计 = " << ((clock() - time) / CLOCKS_PER_SEC) << "秒" << endl;
}
// 恢复算子
// 插入removedList中的所有节点
void ALNS::greedyInsertion(Solution* solution) {
	if (solution == nullptr) {											// solution为空，默认启用curSol
		solution = &curSol;
	}
	std::cout << "贪婪插入" << std::endl;
	while (!solution->removedList.empty()) { // 直到removedList清空
		int node_left = solution->removedList.size();
		greedyInsertionOnce(solution);
		if (node_left == solution->removedList.size()) { // 节点没有发生变换，则新增车辆
			solution->addVehicle();
		}
	}
}
// 插入一个节点
void ALNS::greedyInsertionOnce(Solution* solution) {
	if (solution->removedList.empty()) {								// 当removedList为空，禁止插入
		throw MyException("The removedList is empty, no need for insertion!");
	}
	unique_ptr<Solution> local_best_solution = nullptr;					// 当前最优解
	bool tag = true;
	// 从最后一个节点往前遍历，减少删除时间复杂度
	for (int i = 0; i< solution->removedList.size(); i++) {
		// 遍历所有位置，确定是否能插入
		for (int j = 0; j < solution->vehicles.size(); j++) {				// 遍历所有车
			for (int k = 1; k < solution->vehicles.at(j).size(); k++) {		// 遍历车内所有位置
				Solution localSol = *solution;							// 生成当前解的拷贝
				localSol.insert(j, k, solution->removedList.at(i));		// 生成新的解
				updateSolution(&localSol);								// 更新新的解目标函数等信息
				if (check_solution_feasible(&localSol) && localSol.isBetter(local_best_solution)) {			// 当前解可行且更好则进行替换
					local_best_solution.reset(new Solution(localSol));	
					local_best_solution->removedList.erase(local_best_solution->removedList.begin() + i);
				}
			}
		}
	}
	// 插入失败时，加入新车，将最后一个节点放入新车
	if (local_best_solution == nullptr || !local_best_solution) {
		return;
	} else { // 否则将当前车辆变更为局部最优解
		*solution = *(local_best_solution.get());
	}
}

// 破坏算子
void  ALNS::randomDestroy(Solution* solution) {
	if (solution == nullptr) {
		solution = &curSol;
	}
	std::cout << "随机删除" << std::endl;
}

// 计算目标函数
double ALNS::cal_objectives(Solution* solution) {
	if (solution == nullptr) {
		solution = &curSol;
	}
	std::cout << "计算目标函数" << std::endl;
	return solution->curVal;
}
// 将当前解送入解空间
SolutionFrequency ALNS::push_solution_space(ICriterion* criterion, Solution* solution) {
	// 输入nullptr时，改用当前解
	if (solution == nullptr) {
		solution = &curSol;
	}
	// 当没有判断标准时，即为第一次，直接接受
	double curT;
	if (criterion == nullptr) { // curT = inf -> p = 1
		curT = DBL_MAX;
	} else {
		curT = criterion->getCurT();
	}
	// 计算目标函数值
	cal_objectives(solution);
	// 计算字符串
	string solution_str = solution->toString();
	auto tmp = solutionSpace.find(solution_str);
	if (solution->curVal < bestSol.curVal) { // 判断是否为最优解，是最优解+1.5
		return BEST;
	} else if (tmp != solutionSpace.end() && solution->curVal > curSol.curVal) { // 判断是否为未出现过的解，比当前解更好则+1.2
		return NEVER_BUT_BETTER;
	} else { // 模拟退火
		if (simulated_annealing(curT, solution->curVal)) { // 接受当前解
			return SA_ACCEPT;
		} else { // 拒绝当前解
			return SA_REJECT;
		}
	}
	throw exception("You can not reach here!");
	return BEST;
}

// 模拟退火准则
bool ALNS::simulated_annealing(double curT, double curVal) {
	double possibility = exp( - (curVal - bestSol.curVal) / curT);	// 系统接受新状态的概率
	double random = Util::getRandom();							// 随机数（0，1）
	if (random < possibility) {
		return true;
	} else {
		return false;
	}
}

// 更新参数信息
void ALNS::updateParameters() {

}

// 更新得分情况
void ALNS::updateScores(SolutionFrequency sf) {
	// TODO: 更新当前追加得分
}

// 更新解的信息
void ALNS::updateSolution(Solution* solution) {
	// TODO: 更新费用
	double total_cost = 0;								// 初始费用为0
	for (int i = 0; i < solution->vehicles.size(); i++) { // 计算目标函数
		total_cost += solution->cal_solution_cost();
	}
	solution->curVal = total_cost;

}

// 更新当前解信息，主要用事后/事前检测
void ALNS::update() {
	// TODO: 更新当前的状态-仅用于测试阶段调试，后期会移除

}

// 判断当前解是否可行
bool ALNS::check_solution_feasible(Solution* solution) {
	// TODO: 判断当前解是否可行
	if (solution == nullptr) {
		solution = &curSol;
	}
	for (int i=0;i<solution->vehicles.size();i++)
		if (solution->vehicles[i].size() > 10)
			return false;
	
	return true;
}
