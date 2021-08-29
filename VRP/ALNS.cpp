#include "ALNS.h"
#include <algorithm>

namespace alns {

ALNS::ALNS():virtualFlight(inf consequence)
{
	cout << "启动 ALNS" << endl;
	curSol.curVal = DBL_MAX;
	// 生成初始解，注入information
	if (ALNS_Setting::range == WEST)
		curSol.depot = inf depots.at(1)->getID();
	else if (ALNS_Setting::range == EAST)
		curSol.depot = inf depots.at(0)->getID();
	// 将所有任务压入RemovedList
	for (int i = 0; i < inf nodes.size(); i++) {
		FerryVehicleTask* fvt = inf nodes.at(i); // 摆渡车任务
		ISolutionNode sn(fvt);// 扔到堆里，和解共生。
		curSol.nodes.push_back(std::move(sn));
		curSol.removedList.push_back(i);
	}
	// 将场站节点压入nodes末尾
	for (int i = 0; i < inf depots.size(); i++) {
		FerryVehicleTask* fvt = inf depots.at(i); // 摆渡车任务
		ISolutionNode sn(fvt);// 扔到堆里，和解共生。
		curSol.nodes.push_back(std::move(sn));
	}
	cout << "ALNS就绪" << endl;
}

void ALNS::start()
{
	/*自适应大领域搜索在大领域搜索的基础上增加了更多的破坏和恢复算子，同时为这些算子赋予权重和增加了一个适应层，在适应层中需要控制迭代：更新比例r、segment number 更新周期，抽选算子准则，算子计分方法*/
	std::cout << "    开始ALNS算法" << std::endl;
	/*生成初始解——利用恢复算子+约束条件*/
	std::cout << "		生成初始解" << std::endl;
	greedyInsertion();
	std::cout << "		初始解生成完毕" << std::endl;
	cal_objectives();									// 计算当前目标函数值
	push_solution_space();								// 加入解空间

	/*开始迭代——达到停止标准之前*/
	std::cout << criterion.toString() << std::endl;
	enum SolutionFrequency sf = ALREADY;
	while (criterion.iter())
	{
		/*构建新解*/
		Solution newSol = curSol;
		/*破坏当前解*/
		randomDestroy(&newSol);
		/*恢复当前解*/
		greedyInsertion(&newSol);
		// 更新新解
		double grade = push_solution_space(&newSol);
		/*利用模拟退火准则判断是否接受当前解*/
		bool isAccept = accept(criterion.getCurT(), sol, newSol);
		if (isAccept)
		{
			string solutionString = newSol->toString();
			if (newSol->cal_objectives() < bestVal)
			{
				solutionSpace.insert(solutionString);
				sf = BEST;
			}
			else if (solutionSpace.count(solutionString))// 没找到则插入
			{
				solutionSpace.insert(solutionString);
				sf = NEVER;
			}
			else
				sf = ALREADY;
			delete sol;
			sol = newSol;
		}
		else
			delete newSol;
		// 将当前解放入解空间
		sol->updateScores(sf, ond, onr);
		/*更新 Adaptive layer 参数*/
		if (criterion.isUpdateParamter())
			sol->updateParameter(ALNS_Setting::segment);
	}
	std::cout << "    迭代结束，共计迭代次数：" << criterion.getTotalTimes() << std::endl;
}
// 恢复算子
void ALNS::greedyInsertion(Solution* solution) {
	if (solution == nullptr) {
		solution = &curSol;
	}
	std::cout << "贪婪插入" << std::endl;
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
double ALNS::push_solution_space(Solution* solution) {
	// 输入nullptr时，改用当前解
	if (solution == nullptr) {
		solution = &curSol;
	}
	// 计算目标函数值
	cal_objectives(solution);
	// 计算字符串
	string solution_str = toString(solution);
	
	if (solution->curVal < bestSol.curVal) { // 判断是否为最优解
		solutionSpace.insert(make_pair(solution_str, solution->curVal));
		return 1.2;
	} else if ()
	auto tmp = solutionSpace.find(solution_str);
	if (tmp == solutionSpace.end()) {

	}
	return 0;
}

// 输出当前解
std::string ALNS::toString(Solution* solution) {
	if (solution == nullptr) {
		solution = &curSol;
	}
	// 删除空车
	for (auto iter = solution->sol.end(); iter != solution->sol.begin(); iter--) {
		if (iter->size() < 2) {
			throw new exception("车辆节点小于2");
		} else if (iter->size() == 2) {
			iter = solution->sol.erase(iter);
		}
	}
	// 先进行排序
	sort(solution->sol.begin(), solution->sol.end());
	std::string result;
	for (int i = 0; i < solution->sol.size(); i++) {
		for (int j = 0; j < solution->sol[i].size(); j++) {
			result += std::to_string(solution->sol[i][j]) + ',';
		}
		result += '\n';
	}
	return result;
}
} // alns
