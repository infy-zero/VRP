#include "ALNS.h"

#include <algorithm>
#include <ctime>
#include <memory>
#include <queue>
#include <unordered_set>

#include "ALNS/Util/Util.h"
#include "ALNS_Setting.h"
#include "MyException.h"

/* 此处用来记录一些新思路
* 1、增强随机性：随机生成一个列表，在/不在列表中的节点不参与算子计算
* 2、降低耦合性：把所有算子和执行一次的算子彻底分离，不互相牵扯，为后续设计新算子预留改进空间
* 3、// 车辆节点对于到达时间不设限制，目前假设可以无限等待 TODO(Lvning): 增加停车位置，然后变为一个分段函数
	vehicle_service_latest_end_time = DBL_MAX;（current）
*/

ALNS::ALNS()
{
	cout << "启动 ALNS" << endl;
	// 确定摆渡车任务（车辆数）上限，超出此上限则认为订单或任务有问题
	total_vehicle_num = inf nodes.get_size(0b001);
	// 生成初始解，注入information
	nodes_ = &inf nodes;
	
	// 场站数据
	if (A_S range == EAST) {
		depot_ = -2;
	}
	else {
		depot_ = -1;
	}
	// 生成初始解
	if (curSol == nullptr) {
		curSol.reset(new Solution(depot_, *nodes_));
	}
	else {
		throw MyException("The current solution exists.");
	}
	// 将所有任务压入RemovedList TODO(Lvning): 修正角标
	for (int i = 0; i < inf nodes_indexs.at(0b001).size(); i++) {
		curSol->removedList.push_back(inf nodes_indexs.at(0b001).at(i));
	}

	// 初始化算子
	// 初始化插入算子
	initialize_insertion();
	initialize_destroy();
	cout << "ALNS就绪" << endl;
	
}

void ALNS::start()
{
	/*自适应大领域搜索在大领域搜索的基础上增加了更多的破坏和恢复算子，同时为这些算子赋予权重和增加了一个适应层，在适应层中需要控制迭代：更新比例r、segment number 更新周期，抽选算子准则，算子计分方法*/
	clock_t clock_time = clock();
	time_t now = time(0);	// 获取当前时间
	tm now_tm;
	localtime_s(&now_tm, &now);
	string now_time = to_string(now_tm.tm_mon)+'-'+ to_string(now_tm.tm_mday) +'_'+ to_string(now_tm.tm_hour) +'-'+ to_string(now_tm.tm_min) +'-'+ to_string(now_tm.tm_sec);   // 当前时间字符串
	string filename = A_S output_dir + "ALNS_result_" + now_time + "_MC" + to_string(nodes_->get_size(0b001)) + "_decay" + to_string(A_S decay) + "_decaytimes" + to_string(A_S decayTimes) + "_upt" + to_string(A_S segment) + ".txt";
	fopen_s(&file_, filename.c_str(), "w");
	output(file_, "    开始ALNS算法\n" );

	/*生成初始解——利用恢复算子+约束条件*/
	output(file_, "节点数 = " + to_string(curSol->removedList.size()));
	output(file_, "		生成初始解\n");
	select_insertion(curSol);
	output(file_, "初始解生成完毕\n");
	curSol->update();													// 计算当前目标函数值
	push_solution_space(nullptr, curSol);								// 加入解空间
	output(file_, clock_time, BEST, curSol);

	// 将初始解写入文件
	string origin_output_string = to_string(nodes_->get_size(0b001)) + "," + now_time + "," + to_string((clock() - clock_time) / CLOCKS_PER_SEC) + "," + to_string(1) + "," + to_string(bestSol->cur_val_) + "," + to_string(bestSol->vehicles.size()) + "\n";

	/*开始迭代——达到停止标准之前*/
	while (criterion.iter())
	{
		// 构建新解
		shared_ptr<Solution> newSol(new Solution(*curSol));
		// 破坏当前解
		int selected_destroy_index = select_destroy(newSol);
		// 恢复当前解
		int selected_repair_index = select_insertion(newSol);
		// 更新newSol信息
		newSol->update();
		// 判断是否更新新解
		SolutionFrequency sf = push_solution_space(&criterion, newSol);

		// 更新当前解的分数
		updateScores(selected_repair_index, selected_destroy_index, sf);
		// 更新 Adaptive layer 参数
		if (criterion.isUpdateParamter()) {
			updateParameters(criterion.max_segment);
		}
		if (criterion.curTimes % A_S PRINT_FRQ == 0) {
			output(file_, clock_time, sf, newSol);
		}
	}
	output(file_, "    迭代结束，共计迭代次数：" + to_string(criterion.getTotalTimes())+"\n");
	output(file_, clock_time, BEST, bestSol);
	fclose(file_);
	// 输出结果到csv(数量+计算时间+计算时间+迭代次数+最优函数值+车辆数)
	FILE* file_csv;
	fopen_s(&file_csv, (A_S output_dir + "output_MC" + to_string(nodes_->get_size(0b001)) + ".csv").c_str(), "a");
	now_time = to_string(now_tm.tm_mon) + '/' + to_string(now_tm.tm_mday) + ' ' + to_string(now_tm.tm_hour) + ':' + to_string(now_tm.tm_min) + ':' + to_string(now_tm.tm_sec);   // 当前时间字符串
	string output_string = to_string(nodes_->get_size(0b001)) + "," + now_time + "," + to_string((clock() - clock_time) / CLOCKS_PER_SEC) + "," + to_string(criterion.getTotalTimes()) + "," + to_string(bestSol->cur_val_) +"," + to_string(bestSol->vehicles.size()) + "\n";
	fprintf(file_csv, origin_output_string.c_str());
	fprintf(file_csv, output_string.c_str());
	fclose(file_csv);
}
// 恢复算子
// 初始化插入算子 TODO(Lvning):优化结构，增加可维护性
void ALNS::initialize_insertion() {
	// 目前只有两个算子——贪婪插入、单点贪婪、后悔值插入
	repair_grades = { A_S greedy_insertion_weight, A_S single_greedy_insertion_weight, A_S regret_insertion_weight };
	initialize_insertion_add_and_count();
}
void ALNS::initialize_insertion_add_and_count() {
	repair_add = { 0.0, 0.0, 0.0 };
	repair_count = { 0, 0, 0 };
}
// 轮盘赌选择算子
int ALNS::select_insertion(shared_ptr<Solution>& solution) {
	if (solution == nullptr) {											// solution为空，默认启用curSol
		throw MyException("You can check non-exist solution.");
	}
	if (solution->vehicles.size() == 0) {
		solution->add_empty_vehicle();
	}
	int select_index = Util::roulette(repair_grades);
	switch (select_index) {
	case 0:
		greedy_insertion(solution);
		break;
	case 1:
		single_greedy_insertion(solution);
		break;
	case 2:
		regret_insertion(solution); 
		break;
	default:
		throw MyException("Not a feasible index.");
	}
	return select_index;
}
// TODO(Lvning): 完善is_feasible后完善该算子
void ALNS::mix_insertion(shared_ptr<Solution>& solution) {

}
// 后悔值插入removedList中的所有节点，保证随时有空车（有一辆，最优和次优可能有一种在额外车辆中）
void ALNS::regret_insertion(shared_ptr<Solution>& solution) {
	output(file_, "后悔值插入\n");
	while (!solution->removedList.empty()) { // 直到removedList清空
		// 确保有且只有一辆空车
		solution->satisfy_condition();
		int node_left = solution->removedList.size();
		regret_insertion_once(solution);
		if (node_left == solution->removedList.size()) { // 节点没有发生变换，说明没能插入节点，报错
			solution->make_single_empty_vehicle();
		}
	}
}
// 随机插入一个节点
void ALNS::regret_insertion_once(shared_ptr<Solution>& solution) {
	if (solution->removedList.empty()) {								// 当removedList为空，禁止插入
		throw MyException("The removedList is empty, no need for insertion!");
	}
	unique_ptr<InsertDescription> global_best_operator_description;		// 全局最优操作
	double global_delta = -A_S dbl_max_value;

	// 从最后一个节点往前遍历，减少删除时间复杂度
	for (int i = 0; i < solution->removedList.size(); i++) {
		// 遍历所有位置，确定是否能插入
		// 每次保留三个值：delta、min、2nd、solution，引入计算差值的函数（始终为正）
		unique_ptr<InsertDescription> local_best_operator_description;    // 当前局部最优操作
		priority_queue<double, vector<double>, greater<double>> best_two_values;// 最小两个解的目标函数值(小顶堆)

		for (int j = 0; j < solution->vehicles.size(); j++) {				// 遍历所有车
			for (int k = 1; k < solution->vehicles.at(j)->get_total_size(); k++) {		// 遍历车内所有位置
				// TODO(Lvning): 将以下内容写为一个函数，最终解只保留算子信息，而非整个解
				unique_ptr<InsertDescription> current_operator_description(new InsertDescription(INSERT_NODE, j, k, solution->removedList.at(i), i));									  // 当前操作
				
				bool operator_is_feasible = solution->check_operator_feasible(current_operator_description);
				//cout << operator_is_feasible << "," << *current_operator_description;
				if (operator_is_feasible) {
					// 维护小顶堆
					best_two_values.push(current_operator_description->save_value_);
					while (best_two_values.size() > 2) {
						best_two_values.pop();
					}
					if (current_operator_description->is_better(local_best_operator_description)) {// 当前解可行且更好则进行替换
						local_best_operator_description = move(current_operator_description);
					}
					
				}
			}
		}
		// 循环结束，判断是否需要更新全局最优解。插入操作，节约值为负数
		double local_delta;
		if (best_two_values.size() == 1) {
			local_delta = best_two_values.top()+ A_S dbl_max_value;
		} else if (best_two_values.size() == 2) {
			double second_small_val = best_two_values.top();
			best_two_values.pop();
			double first_small_val = best_two_values.top();
			local_delta = abs(second_small_val - first_small_val);
			//cout << first_small_val << "," << second_small_val << endl;
		} else {
			continue;
		}
		if (local_delta >= global_delta) {
			global_best_operator_description = move(local_best_operator_description);
			global_delta = local_delta;
		}
	}
	// 加入新车
	if (global_best_operator_description != nullptr) {
		solution->excute_insert_operator(global_best_operator_description);
	}
}

// 贪婪插入removedList中的所有节点
void ALNS::greedy_insertion(shared_ptr<Solution>& solution) {
	output(file_, "贪婪插入\n");
	while (!solution->removedList.empty()) { // 直到removedList清空
		// cout << "当前剩余节点数 = " << solution->removedList.size() << endl;
		int node_left = solution->removedList.size();
		solution->satisfy_condition();
		//cout << solution->to_string_without_update() << endl;
		greedy_insertion_once(solution);
		if (node_left == solution->removedList.size()) { // 节点没有发生变换，则新增车辆
			solution->add_empty_vehicle();
		}
	}
}
// 插入一个节点
void ALNS::greedy_insertion_once(shared_ptr<Solution>& solution) {
	if (solution->removedList.empty()) {								// 当removedList为空，禁止插入
		throw MyException("The removedList is empty, no need for insertion!");
	}
	unique_ptr<InsertDescription> global_best_operator_description;		// 全局最优操作
	bool tag = true;
	//cout << "insert once" << endl;
	// 从最后一个节点往前遍历，减少删除时间复杂度
	for (int i = solution->removedList.size()-1; i>=0; i--) {
		// 遍历所有位置，确定是否能插入
		for (int j = 0; j < solution->vehicles.size(); j++) {				// 遍历所有车
			for (int k = 1; k < solution->vehicles.at(j)->get_total_size(); k++) {		// 遍历车内所有位置
				unique_ptr<InsertDescription> current_operator_description(new InsertDescription(INSERT_NODE, j, k, solution->removedList.at(i), i));			// 当前操作
				
				bool operator_is_feasible = solution->check_operator_feasible(current_operator_description);
				
				if (operator_is_feasible && current_operator_description->is_better(global_best_operator_description)) {			// 当前解可行且更好则进行替换
					global_best_operator_description = move(current_operator_description);
				} else {
					//cout << "isfeasible="<< is_feasible << ", local_exitsts=" << (localSol!=nullptr) <<endl;
				}
	
			}
		}
	}
	// 插入失败时，加入新车，将最后一个节点放入新车
	if (global_best_operator_description != nullptr) {// 否则将当前车辆变更为局部最优解
		solution->excute_insert_operator(global_best_operator_description);
	}
}
// 单点贪婪插入（每次只进行一个节点的贪婪插入，且这个节点随机选择）
void ALNS::single_greedy_insertion(shared_ptr<Solution>& solution) {
	output(file_, "单点贪婪插入\n");
	while (!solution->removedList.empty()) { // 直到removedList清空
		solution->satisfy_condition();
		int node_left = solution->removedList.size();
		single_greedy_insertion_once(solution);
		if (node_left == solution->removedList.size()) { // 当有节点不能插入时，插入新车
			solution->make_single_empty_vehicle();
		}
	}
	solution->update();
}
void ALNS::single_greedy_insertion_once(shared_ptr<Solution>& solution) {
	if (solution->removedList.empty()) {								// 当removedList为空，禁止插入
		throw MyException("The removedList is empty, no need for insertion!");
	}
	unique_ptr<InsertDescription> global_best_operator_description;		// 全局最优操作
	int left_node_size = solution->removedList.size();
	
	// 从最后一个节点往前遍历，减少删除时间复杂度
	int selected_node_pos = left_node_size * Util::getRandom();
	// 遍历所有位置，确定是否能插入
	for (int j = 0; j < solution->vehicles.size(); j++) {				// 遍历所有车
		for (int k = 1; k < solution->vehicles.at(j)->get_total_size(); k++) {		// 遍历车内所有位置
			unique_ptr<InsertDescription> current_operator_description(new InsertDescription(INSERT_NODE, j, k, solution->removedList.at(selected_node_pos), selected_node_pos));			// 生成当前解的拷贝
			
			bool operator_is_feasible = solution->check_operator_feasible(current_operator_description);
			//cout << is_feasible << endl;
			if (operator_is_feasible && current_operator_description->is_better(global_best_operator_description)) {			// 当前解可行且更好则进行替换
				global_best_operator_description = move(current_operator_description);
			}
			else {
			}
		}
	}
	if (global_best_operator_description != nullptr) {
		solution->excute_insert_operator(global_best_operator_description);
	}
}

/*破坏算子*/ 
void ALNS::initialize_destroy() {
	// 目前只有两个算子——随机删除、贪婪删除
	destroy_grades = { A_S random_removal_weight, A_S greedy_removal_weight };
	initialize_destroy_add_and_count();
}
void ALNS::initialize_destroy_add_and_count() {
	destroy_add = { 0.0, 0.0 };
	destroy_count = { 0, 0 };
}
// 轮盘赌选择算子
int ALNS::select_destroy(shared_ptr<Solution>& solution) {
	if (solution == nullptr) {
		throw MyException("You can check non-exist solution.");
	}
	int select_index = Util::roulette(destroy_grades);
	switch (select_index) {
	case 0:
		random_destroy(solution);
		break;
	case 1:
		greedy_destroy(solution);
		break;
	default:
		throw MyException("Not a feasible index.");
	}
	return select_index;
}
// 混合删除策略：将各种算子混合使用，加入自适应层  TODO(Lvning): 完善is_feasible后完善该算子
void ALNS::mix_destory(shared_ptr<Solution>& solution) {

}

// 随机删除 4-40% 个节点，由于节点之间两个方向的有序性， 
void ALNS::random_destroy(shared_ptr<Solution>& solution) {
	output(file_, "随机删除\n");
	if (solution->all_nodes_.get_size(0b001) == solution->removedList.size()) { // 没有节点可以删除
		throw MyException("You can not delete from an empty solution.");
	}
	int rest_node_num = solution->all_nodes_.get_size(0b001) - solution->removedList.size();
	int delete_node_num;
	if (rest_node_num > 10) {
		delete_node_num = static_cast<int> (Util::getRandom() * 0.4 * rest_node_num);
	} else {
		delete_node_num = static_cast<int> (Util::getRandom() * rest_node_num);
	}
	delete_node_num = max(delete_node_num, 1);	// 至少删除一个
	while (delete_node_num--) {
		random_destroy_once(solution);
	}
	// TODO(Lvning): 维护所有变量到统一的地方
	output(file_, "当前车辆内剩余节点数 = " + to_string(solution->all_nodes_.get_size(0b001) - solution->removedList.size()) + "，节点总数 = " + to_string(solution->all_nodes_.get_size(0b001)) + "\n");
	solution->update_after_delete();
}

// 随机删除1个节点
void ALNS::random_destroy_once(shared_ptr<Solution>& solution) {
	if (solution->removedList.size() == solution->all_nodes_.get_size(0b001)) {
		throw MyException("Can't delete from an empty solution.");
	}
	// 随机删除一个节点 TODO(Lvning): 写成一个函数
	int total_node_num = solution->get_total_node_num();
	int selected_node_num = total_node_num * Util::getRandom()+1;
	int selected_num = selected_node_num;
	int vehicle_index = -1;
	int selected_node_pos = -1;
	for (vehicle_index = 0; vehicle_index < solution->vehicles.size(); vehicle_index++) {
		if (selected_node_num > solution->vehicles.at(vehicle_index)->get_node_size()) {
			selected_node_num -= solution->vehicles.at(vehicle_index)->get_node_size();
			continue;
		}
		for (int node_pos = 0; node_pos < solution->vehicles.at(vehicle_index)->get_total_size(); node_pos++) {
			int node_index = solution->vehicles.at(vehicle_index)->get_node_index(node_pos);
			if (solution->all_nodes_.get_node_type(node_index) == TYPE_NODE) {
				selected_node_num--;
			}
			if (selected_node_num == 0) {
				selected_node_pos = node_pos;
				break;
			}
		}
		if (selected_node_num == 0) {
			break;
		}
	}
	int node_index = solution->vehicles.at(vehicle_index)->get_node_index(selected_node_pos);
	if (solution->all_nodes_.get_node_type(node_index) != TYPE_NODE) {
		throw MyException("You can't delete node which type is not node.");
	}
	solution->vehicles.at(vehicle_index)->delete_node(selected_node_pos, solution->removedList);
}
// 贪婪破解坏算子
void ALNS::greedy_destroy(shared_ptr<Solution>& solution) {
	output(file_, "贪婪删除\n");
	if (solution->all_nodes_.get_size(0b001) == solution->removedList.size()) { // 没有节点可以删除
		throw MyException("You can not delete from an empty solution.");
	}
	int rest_node_num = solution->all_nodes_.get_size(0b001) - solution->removedList.size();
	int delete_node_num;
	if (rest_node_num > 10) {
		delete_node_num = static_cast<int> (Util::getRandom() * 0.4 * rest_node_num);
	}
	else {
		delete_node_num = static_cast<int> (Util::getRandom() * rest_node_num);
	}
	delete_node_num = max(delete_node_num, 1);	// 至少删除一个
	while (delete_node_num--) {
		greedy_destroy_once(solution);
	}
	// TODO(Lvning): 维护所有变量到统一的地方
	output(file_, "当前车辆内剩余节点数 = " + to_string(solution->all_nodes_.get_size(0b001) -solution->removedList.size()) + "，节点总数 = " + to_string(solution->all_nodes_.get_size(0b001)) + "\n");
	solution->update_after_delete();
}
// 一次贪婪破解坏算子（删除一个节点，获取目标函数值最低的那个），但不进行feasible检验
void ALNS::greedy_destroy_once(shared_ptr<Solution>& solution) {
	if (solution->removedList.size() == solution->all_nodes_.get_size(0b001)) {
		throw MyException("Can't delete from an empty solution.");
	}
	// 找到删除之后改进最大的节点
	unique_ptr<DeleteDescription> global_best_operator_description = nullptr;
	for (int i = 0; i < solution->vehicles.size(); i++) {	// 遍历所有车
		for (int j = 0; j < solution->vehicles.at(i)->get_total_size(); j++) { // 遍历所有节点
			int node_index = solution->vehicles.at(i)->get_node_index(j);
			if (solution->all_nodes_.get_node(node_index)->type_ == TYPE_NODE) {
				unique_ptr<DeleteDescription> current_operator_description(new DeleteDescription(DELETE_NODE, i, j, node_index));// 当前操作
				// 计算节约值

				solution->cal_delete_save_value(current_operator_description);

				if (current_operator_description->is_better(global_best_operator_description)) {
					global_best_operator_description = move(current_operator_description);
				}
			} else {
				continue;
			}
		}
	}
	if (global_best_operator_description != nullptr) {
		solution->excute_insert_operator(global_best_operator_description);
	} else {
		throw MyException("Should delete a node.");
	}
}

// 将当前解送入解空间
SolutionFrequency ALNS::push_solution_space(ICriterion* criterion, shared_ptr<Solution>& solution) {
	// 计算目标函数值
	solution->update();
	// 计算字符串
	solution->solution_string_ = solution->toString_with_update();
	// 输入nullptr时，改用当前解
	if (solution == nullptr) {
		throw MyException("You can check non-exist solution.");
	}
	// 当没有判断标准时，即为第一次，直接接受
	double curT;
	if (criterion == nullptr) { // curT = inf -> p = 1
		solutionSpace.insert(make_pair(solution->solution_string_, solution->cur_val_));
		curSol = solution;
		bestSol = shared_ptr<Solution>(new Solution(*curSol));
		return SolutionFrequency::BEST;
	} else {
		curT = criterion->getCurT();
	}
	
	auto tmp = solutionSpace.find(solution->solution_string_);
	if (solution->isBetter(bestSol)) { // 判断是否为最优解，是最优解+1.5
		solutionSpace.insert(make_pair(solution->solution_string_, solution->cur_val_));
		curSol = solution;
		bestSol = shared_ptr<Solution>(new Solution(*curSol));
		return BEST;
	} else if (tmp == solutionSpace.end() && solution->isBetter(curSol)) { // 判断是否为未出现过的解，比当前解更好则+1.2
		solutionSpace.insert(make_pair(solution->solution_string_, solution->cur_val_));
		curSol = solution;
		return NEVER_BUT_BETTER;
	} else { // 模拟退火
		if (simulated_annealing(curT, solution->cur_val_)) { // 接受当前解
			solutionSpace.insert(make_pair(solution->solution_string_, solution->cur_val_));
			curSol = solution;
			return SA_ACCEPT;
		} else { // 拒绝当前解
			curSol = shared_ptr<Solution>(new Solution(*bestSol));
			return SA_REJECT;
		}
	}
	throw exception("You can not reach here!");
	return BEST;
}

// 模拟退火准则
bool ALNS::simulated_annealing(double curT, double curVal) {
	double possibility = exp( - (curVal - bestSol->cur_val_) / curT );	// 系统接受新状态的概率
	double random = Util::getRandom();							// 随机数（0，1）
	if (random < possibility) {
		return true;
	} else {
		return false;
	}
}

// 更新参数信息
void ALNS::updateParameters(int times) {
	if (repair_grades.size() != repair_add.size()) {
		throw MyException("The size of grades and add are not equal.");
	}
	int total_repair_times = 0;
	for (int i = 0; i < repair_grades.size(); i++) {
		if (repair_count.at(i) == 0) {
			continue;
		}
		repair_grades.at(i) = repair_grades.at(i) * (1.0 - A_S update_ratio) + A_S update_ratio * repair_add.at(i) / static_cast<double>(repair_count.at(i));
		total_repair_times += repair_count.at(i);
	}
	int total_destroy_times = 0;
	for (int i = 0; i < destroy_grades.size(); i++) {
		if (destroy_count.at(i) == 0) {
			continue;
		}
		destroy_grades.at(i) = destroy_grades.at(i) * (1.0 - A_S update_ratio) + A_S update_ratio * destroy_add.at(i) / static_cast<double>(destroy_count.at(i));
		total_destroy_times += destroy_count.at(i);
	}
	if (total_repair_times != total_destroy_times || total_destroy_times != times) {			// 检验次数是否相等
		throw MyException("Total times are not equal.");
	}
	initialize_insertion_add_and_count();
	initialize_destroy_add_and_count();
}

// 更新得分情况
void ALNS::updateScores(int selected_repair_index, int selected_destroy_index, SolutionFrequency sf) {
	// TODO: 更新当前追加得分
	repair_add.at(selected_repair_index) += A_S get_grade_addition(sf);
	repair_count.at(selected_repair_index)++;
	destroy_add.at(selected_destroy_index) += A_S get_grade_addition(sf);
	destroy_count.at(selected_destroy_index)++;
}

void ALNS::output(FILE* file, time_t time, SolutionFrequency sf, shared_ptr<Solution>& solution, int type) {
	cout << "当前迭代次数" << criterion.getTotalTimes() << endl;
	fprintf(file, "当前迭代次数 = %d\n", criterion.getTotalTimes());
	output(file, "当前解状态 = " + A_S SolutionFrequency_to_string(sf) + "\n");

	output(file, "当前迭代目标函数值 = " + to_string(solution->cur_val_) + "，当前车辆数 = " + to_string(solution->vehicles.size()) + "，当前解 = " + solution->solution_string_ + "\n");

	output(file, "当前全局目标函数值 = " + to_string(curSol->cur_val_) + "，当前车辆数 = " + to_string(curSol->vehicles.size()) + "，当前解 = " + curSol->solution_string_ + "\n");
	
	if (bestSol!=nullptr) {
		output(file, "当前全局最优值 = " + to_string(bestSol->cur_val_) + "，当前车辆数 = " + to_string(bestSol->vehicles.size()) + "，最优解 = " + bestSol->solution_string_ + "\n");
	}
	output(file, "适应层参数如下：\n");
	output(file, "恢复算子：贪婪插入、单点贪婪、后悔值插入\n");
	for (double repair_grade : repair_grades) {
		output(file, to_string(repair_grade) + ",");
	}
	output(file, "\n");
	
	output(file, "破坏算子：随机破坏、贪婪破坏\n");
	for (double destroy_grade : destroy_grades) {
		output(file, to_string(destroy_grade) + ",");
	}
	output(file, "\n");

	cout << "用时共计 = " << ((clock() - time) / CLOCKS_PER_SEC) << "秒" << endl;
	fprintf(file, "用时共计 =  %d秒, 平均速度 = %d\n", ((clock() - time) / CLOCKS_PER_SEC));
}
void ALNS::output(FILE* file, string content, int type) {
	cout << content;
	fprintf(file, content.c_str());
}

