#include "FerryTaskSetting.h"
#include "Solution.h"
#include "MyException.h"

#include <algorithm>
#include <vector>


Solution::Solution(int depot, AllNodes* nodes):depot_(depot),all_nodes_(nodes) {
}

double Solution::cal_solution_cost() {
	double solution_cost = 0;
	for (int i = 0; i < vehicles.size(); i++) {
		solution_cost += vehicles.at(i).cal_cost();
	}
	return solution_cost;
}

void Solution::update_value() {
	curVal = cal_solution_cost();
}

bool Solution::has_empty_vehicle() {
	if (vehicles.size() == 0) {
		return false;
	}
	for (IVehicle& vehicle : vehicles) {
		if (vehicle.is_empty()) {
			return true;
		}
	}
	return false;
}

void Solution::clear_empty_vehicle() {
	if (has_empty_vehicle() == false) { // 如果没存在空车
		return;
	}
	for (int i = vehicles.size() - 1; i >= 0; i--) {
		if (vehicles.at(i).is_empty()) {
			vehicles.erase(vehicles.begin() + i);
		}
	}
}


// 比较当前解和other的优劣
bool Solution::isBetter(shared_ptr<Solution>& other) {
	if (other == nullptr) {			// 待比较解为空直接返回真
		return true;
	}
	if (curVal < other->curVal) {	// 当前解目标函数值更小
		return true;
	}
	return false;
}

bool Solution::judge_not_exceed_max_length() {
	for (IVehicle vehicle : vehicles) {
		if (vehicle.beyond_max_length()) {
			if (vehicle.size() <= 3) {
				throw MyException("The task's length is more than max length of a vehicle.");
			}
			cout << "当前解超过长度，max=" << vehicle.vehicle_max_length << "，cur=" << vehicle.cal_length() << endl;
			return false;
		}
	}
	return true;
}

// TODO(Lvning): 当前只能够判断单行程所有节点的最早、最晚时间，如果要引入多行程，需要更改判断方法
bool Solution::judge_sequence_feasible() {
	//cout << to_string_without_update() << endl;
	
	// 将所有节点初始化
	for (auto node_iter : all_nodes_->nodes_) { // 所有节点信息作废
		node_iter.second.reset_infeasible();
	}
	for (auto depot_iter : all_nodes_->depots_) { // 所有场站信息初始化
		depot_iter.second.reset_feasible();
	}

	// 开始进行检验
	vector<int> vehicle_index(vehicles.size());
	auto& virtual_flight_consequence = inf consequence;
	vector<int> flight_index(virtual_flight_consequence.size());

	// TODO(Lvning):将角标做相应转换，虽然map和vector角标号一致（>0），但是为了方便起见，以map为基准
	// 两个unordered_map实现，内容为<节点角标，[组序号, 组内角标, 组长度]>
	// 以vehicles中的节点为主要参考目标，对于flights中的节点，只考虑和vehicles中重叠部分的顺序
	unordered_map<int, NodeLocation> vehicle_front;
	for (int i = 0; i < vehicles.size(); i++) {
		if (vehicles.at(i).is_empty()) {
			continue;
		}
		NodeLocation int_tmp{ i, 1, vehicles.at(i).size() - 1 };
		vehicle_front.insert(make_pair(vehicles.at(i).at(1), int_tmp));
	}

	// 航班前沿节点
	unordered_map<int, NodeLocation> flight_front;
	for (int i = 0; i < virtual_flight_consequence.size(); i++) {
		NodeLocation int_tmp{ i, 0, virtual_flight_consequence.at(i).size() - 1 };
		flight_front.insert(make_pair(virtual_flight_consequence.at(i).at(0), int_tmp));
	}

	// 统计当前安排任务的节点的数量和编号，并将节点状态置为UNCHECKED_NODE
	unordered_set<int> node_in_queue;
	for (IVehicle& veh : vehicles) { // 遍历车辆，加入节点序号
		for (int node_index : veh.vehicle_nodes) {
			if (all_nodes_->get(node_index)->type_ == TYPE_NODE) {
				node_in_queue.insert(node_index);
			}
		}
	}
	// 更新航班前沿的信息（防止开始节点卡死）---TODO(Lvning): 这里应该封装为函数！！
	for (auto flight_iter = flight_front.begin(); flight_iter != flight_front.end();) {
		if (node_in_queue.count(flight_iter->first)) { // 如果有则直接跳过
			flight_iter++;
			continue;
		}

		skip_flight_node_not_in_queue(flight_iter, virtual_flight_consequence, &node_in_queue, &flight_front);
		flight_iter = flight_front.erase(flight_front.find(flight_iter->first));
	}
	/*
	cout << "in_queue nodes = ";
	for (int index : node_in_queue)
	{
		cout << index << ",";
	}
	cout << endl;
	*/

	while (node_in_queue.size() > 0) {
		int node_in_queue_size = node_in_queue.size();
		//cout << to_string_without_update() << endl;
		/*
		cout << "vehicle front = " << endl;
		for (auto index : vehicle_front)
		{
			cout << "index=" << index.first << ",veh_pos=" << index.second.group_index << ",node_pos=" << index.second.cur_pos <<
				",max_pos=" << index.second.max_pos << endl;
		}
		cout << endl;

		cout << "flight front = " << endl;
		for (auto index : flight_front)
		{
			cout << "index=" << index.first << ",veh_pos=" << index.second.group_index << ",node_pos=" << index.second.cur_pos <<
				",max_pos=" << index.second.max_pos << endl;
		}
		cout << endl;
		*/

		for (auto vehicle_iter = vehicle_front.begin(); vehicle_iter != vehicle_front.end();) {
			int cur_index = vehicle_iter->first;
			unordered_map<int, NodeLocation>::iterator flight_iter = flight_front.find(cur_index);
			if (flight_iter != flight_front.end()) { // 找到了能够匹配的节点
			  // 更新节点时间，失败则直接返回
				if ( !update_node_timewindow(vehicle_iter->second, flight_iter->second) ) {
					return false;
				}


			  // 更新vehicle_front、flight_front、node_in_queue的数据
				
				// cout << "find = " << flight_iter->first << endl;
				node_in_queue.erase(vehicle_iter->first);

				// TODO(Lvning): 先声明变量，在一步一步计算
				// 插入新的车辆队列信息
				auto vehicle_new_tmp = vehicle_iter->second;
				if (vehicle_new_tmp.cur_pos < vehicle_new_tmp.max_pos - 1) {
					int vehicle_new_index = vehicles.at(vehicle_new_tmp.vehicle_index).at(vehicle_new_tmp.cur_pos + 1);
					vehicle_new_tmp.cur_pos++;
					vehicle_front.insert(make_pair(vehicle_new_index, vehicle_new_tmp));
				}
				vehicle_iter = vehicle_front.erase(vehicle_front.find(vehicle_iter->first));

				// 插入新的飞机队列信息
				skip_flight_node_not_in_queue(flight_iter, virtual_flight_consequence, &node_in_queue, &flight_front);
				flight_front.erase(flight_front.find(flight_iter->first));
			}
			else {
				vehicle_iter++;
			}
		}
		if (node_in_queue_size == node_in_queue.size()) { // 如果直到最后都没找到，则返回false
			if (has_empty_vehicle()) {
				//cout << "node_in_queue = " << node_in_queue.size() << endl;
			}
			//cout << solution->to_string_without_update() << endl;
			//cout << "Do not have feasible location. empty = " << solution->has_empty_vehicle() << endl;
			return false;
		}
	}
	return true;
}

// 随机删除一个节点
void Solution::random_delete_without_update() {
	if (vehicles.empty()) {
		throw MyException("Empty vehicle can not be deleted.");
	}
	vector<int> prefix_sum(vehicles.size());
	// 生成前缀和
	for (int i = 0; i < vehicles.size(); i++) {
		if (i == 0) {
			prefix_sum.at(i) = vehicles.at(i).size() - 2;
		} else {
			prefix_sum.at(i) = prefix_sum.at(i - 1) + vehicles.at(i).size() - 2;
		}
	}
	// 选择被删除的节点序号
	int selected_node = static_cast<int> (prefix_sum.back() * Util::getRandom());
	int selected_vehicle_pos = -1;
	int selected_node_pos = -1;
	for (int i = 0; i < prefix_sum.size(); i++) { // 寻找被删除的节点所在车辆号和位置
		if (selected_node +1 > prefix_sum.at(i)) {
			continue;
		} else {
			selected_vehicle_pos = i;
			if (selected_vehicle_pos == 0) {		// 第一辆车则直接是该序号
				selected_node_pos = selected_node + 1;
			} else {								// 否则是该序号减去前缀和
				selected_node_pos = selected_node - prefix_sum.at(i - 1) + 1;
			}
			break;
		}
	}
	delete_without_update(selected_vehicle_pos, selected_node_pos);
}

// 删除指定节点
void Solution::delete_without_update(int vehicle_pos, int node_pos) {
	IVehicle& vehicle = vehicles.at(vehicle_pos);
	int node_index = vehicle.delete_node(node_pos);
	removedList.push_back(node_index);
}

void Solution::addVehicle()
{
	// 检验是否有空车，有空车则抛出异常
	for (int i = 0; i < vehicles.size(); i++) {
		if (vehicles[i].size() <= 2) {

			throw MyException("You can not insert when an empty vehicle exists!");
		}
	}
	IVehicle tmp_vehicle(depot_, all_nodes_);
	vehicles.emplace_back(tmp_vehicle);
}

void Solution::insert(int vehicle_number, int pos, int node) {
	if (vehicles.empty()) { // 非空判断
		throw MyException("You can not insert into an empty vehicle!");
	}
	if (vehicle_number < 0 || vehicle_number >= vehicles.size()) { // 不存在该车辆
		throw MyException("You can not insert into an non-exist vehicle!");
	}
	// 取出当前车辆
	IVehicle& vehicle = vehicles.at(vehicle_number);
	// 在pos位置插入节点node
	vehicle.insert(pos, node);
}

// 跳过不在node_in_queue的节点，并计算相应的时间
void Solution::skip_flight_node_not_in_queue(const unordered_map<int, NodeLocation>::iterator& flight_iter,
											 vector<vector<int>>& virtual_flight_consequence,
											 unordered_set<int>* node_in_queue,
											 unordered_map<int, NodeLocation>* flight_front) {

	const NodeLocation& flight_old_tmp = flight_iter->second;
	vector<int>& flight_order = virtual_flight_consequence.at(flight_old_tmp.vehicle_index);
	if (flight_old_tmp.max_pos != flight_order.size() - 1) { // 检验两个飞机序列是否相同
		throw MyException("These two sequence of flight are not same.");
	}
	int flight_new_pos = flight_old_tmp.cur_pos + 1;
	while (flight_new_pos <= flight_old_tmp.max_pos &&
		node_in_queue->find(flight_order.at(flight_new_pos)) == node_in_queue->end()) { // 跳过不在当前解中的任务
		// 更新时间窗
		int node_index = flight_order.at(flight_new_pos);
		ISolutionNode& node = *(all_nodes_->get(node_index));
		//ISolutionNode&;
		// 不可变节点，不进行处理
		if (node.task->type == FVTType::UNVARIABLE_FLIGHT) {
			node.service_earliest_start_time = node.task->predefined_earliest_arrival_time;
			node.arrive_earliest_time = node.service_earliest_start_time - node.task->predefined_service_max_before_;
			node.service_latest_end_time = node.task->predefined_service_latest_start_time;
		}
		if (node.task->type == FVTType::VARIABLE_FLIGHT) { // 可变节点 —— 一定不是第一、二个
			int pre_flight_node_index = flight_order.at(flight_new_pos - 1);
			ISolutionNode& pre_flight_node = *(all_nodes_->get(pre_flight_node_index));
			// 当前航班节点时间窗 —— 上一飞机节点加延时
			double flight_earliest_service_start_time = pre_flight_node.task->predefined_earliest_service_start_time + fts FTS_max_delta_time;
			double filght_earliest_arrive_time = pre_flight_node.task->predefined_earliest_arrival_time + fts FTS_max_delta_time;
			double flight_latest_service_start_time = pre_flight_node.task->predefined_service_latest_start_time + fts FTS_max_delta_time;

			// 当前车辆节点时间窗 —— 上一车辆节点加旅行时间 + 上车时间 + 下车时间
			double vehicle_earliest_service_start_time = pre_flight_node.task->predefined_earliest_service_start_time + fts FTS_max_delta_time;
			double vehicle_earliest_arrive_time = pre_flight_node.task->predefined_earliest_arrival_time + fts FTS_max_delta_time;
			double vehicle_latest_service_start_time = pre_flight_node.task->predefined_service_latest_start_time + fts FTS_max_delta_time;

			// 当前航班的时间窗上下限
			double kEarliest_service_start_time = node.task->predefined_earliest_service_start_time;
			double kEarliest_arrive_time = node.task->predefined_earliest_arrival_time;
			double kLatest_service_start_time = node.task->predefined_service_latest_start_time;

			// 更新当前节点时间窗
			if ( !cal_timewindow_intersection(flight_earliest_service_start_time, flight_latest_service_start_time, vehicle_earliest_service_start_time, vehicle_latest_service_start_time, &(node.service_earliest_start_time), &(node.service_latest_end_time))) {
				cout << "[" << flight_earliest_service_start_time << "," << flight_latest_service_start_time << "]" << endl;
				cout << "[" << kEarliest_service_start_time << "," << kLatest_service_start_time << "]" << endl;
				throw MyException("The intersection of flight and task can not be less than flight.");
			}

			// 判断是否满足最大时间窗要求
			if (flight_earliest_service_start_time < kEarliest_service_start_time || flight_latest_service_start_time > kLatest_service_start_time) {
				cout << "[" << flight_earliest_service_start_time << "," << flight_latest_service_start_time << "]" << endl;
				cout << "[" << kEarliest_service_start_time << "," << kLatest_service_start_time << "]" << endl;
				throw MyException("The intersection of flight and task can not be less than flight.");
			}
		}	

		flight_new_pos++;
	}
	if (flight_new_pos <= flight_old_tmp.max_pos) { // 插入新的飞机队列信息
		int flight_new_index = flight_order.at(flight_new_pos);
		NodeLocation flight_new_tmp = { flight_old_tmp.vehicle_index, flight_new_pos, flight_old_tmp.max_pos };
		flight_front->insert(make_pair(flight_new_index, flight_new_tmp));
	}
	// flight_front->erase(flight_front->find(flight_iter->first));
}

// 该方法只能用于单行程vrp
bool Solution::update_node_timewindow(const NodeLocation& vehicle_location, const NodeLocation& flight_location) {
	int node_index = vehicles.at(vehicle_location.vehicle_index).at(vehicle_location.cur_pos); // 获取坐标
	if (all_nodes_->get_node_type(node_index) != NodeType::TYPE_NODE) {
		throw MyException("This fuction can only deal with node type.");
	}
	if (node_index != inf consequence.at(flight_location.vehicle_index).at(flight_location.cur_pos)) {
		throw MyException("The information offered can not match, need to check.");
	}

	// TODO(Lvning): get_travel_time()中，增加服务时间
	ISolutionNode& node     = all_nodes_->nodes_.at(node_index);	// 获取当前节点
	int pre_node_index		= vehicles.at(vehicle_location.vehicle_index).at(vehicle_location.cur_pos-1);// 获取前一车辆节点序号
	ISolutionNode& pre_node = all_nodes_->nodes_.at(node_index);    // 获取前一车辆节点
	// 当当前节点为时间窗不可变节点（针对前两个flight任务和depot）时，只需要先计算vehicle方向的时间窗，然后和flight方向时间窗取交集即可
	if (node.task->type == FVTType::UNVARIABLE_FLIGHT) {		
		// 前一车辆节点时间
		double pre_earliest_service_start_time = pre_node.service_earliest_start_time;
		double pre_earliest_arrive_time		   = pre_node.arrive_earliest_time;
		double pre_latest_service_start_time   = pre_node.service_latest_end_time;

		// 根据前一车辆节点时间计算所得的当前节点时间
		double travel_time = inf cal_travel_time(node_index, pre_node_index);
		double node_earliest_service_start_time = pre_earliest_service_start_time + travel_time;
		double node_earliest_arrive_time        = pre_node.arrive_earliest_time + travel_time;
		double node_latest_service_start_time   = pre_node.service_latest_end_time + travel_time;

		// 当前航班节点时间窗
		double flight_earliest_service_start_time = node.task->predefined_earliest_service_start_time;
		double filght_earliest_arrive_time		  = node.task->predefined_earliest_arrival_time;
		double flight_latest_service_start_time = node.task->predefined_service_latest_start_time;

		if (!cal_timewindow_intersection(node_earliest_service_start_time, node_latest_service_start_time, flight_earliest_service_start_time, flight_latest_service_start_time, &(node.service_earliest_start_time), &(node.service_latest_end_time))) {
			return false;
		} else{
			return true;
		}

	}
	// 当当前节点为时间窗可变节点时，只需要先计算vehicle方向的时间窗，然后计算flight方向时间窗，最后和flight方向时间窗取交集即可
	// vehicle方向肯定有前置节点，flight则需要分别判断，以及根据当前节点是否可变时间窗进行分析
	if (node.task->type == FVTType::VARIABLE_FLIGHT) {
		int pre_flight_node_index = vehicles.at(flight_location.vehicle_index).at(flight_location.cur_pos - 1);// 获取前一飞机节点序号
		ISolutionNode& pre_flight_node = all_nodes_->nodes_.at(pre_flight_node_index);    // 获取前一车辆节点
		// 前一车辆节点时间
		double pre_earliest_service_start_time = pre_node.service_earliest_start_time;
		double pre_earliest_arrive_time = pre_node.arrive_earliest_time;
		double pre_latest_service_start_time = pre_node.service_latest_end_time;

		// 根据前一车辆节点时间计算所得的当前节点时间
		double travel_time = inf cal_travel_time(node_index, pre_node_index);
		double node_earliest_service_start_time = pre_earliest_service_start_time + travel_time;
		double node_earliest_arrive_time = pre_node.arrive_earliest_time + travel_time;
		double node_latest_service_start_time = pre_node.service_latest_end_time + travel_time;

		// 当前航班节点时间窗——上一飞机节点加延时
		double flight_earliest_service_start_time = pre_flight_node.task->predefined_earliest_service_start_time + fts FTS_max_delta_time;
		double filght_earliest_arrive_time = pre_flight_node.task->predefined_earliest_arrival_time + fts FTS_max_delta_time;
		double flight_latest_service_start_time = pre_flight_node.task->predefined_service_latest_start_time + fts FTS_max_delta_time;

		if (!cal_timewindow_intersection(node_earliest_service_start_time, node_latest_service_start_time, flight_earliest_service_start_time, flight_latest_service_start_time, &node.service_earliest_start_time, &node.service_latest_end_time)) {
			return false;
		}
		else {
			return true;
		}

	}
	return true;
}

bool Solution::cal_timewindow_intersection(int vehicle_start, int vehicle_end, int flight_start, int flight_end, double* inter_start, double* inter_end) {
	if (vehicle_end < flight_start) {
		return false;
	}
	if (flight_end < vehicle_start) {
		return false;
	}
	*inter_start = max(vehicle_start, flight_start);
	*inter_end = min(vehicle_end, flight_end);
	return true;
}

// 生成可行解字符串，push_solution_space专用
string Solution::toString_with_update() {
	// 删除空车
	for (int i = vehicles.size() - 1; i >= 0; i--) {
		if (vehicles.at(i).size() < 2) {
			throw new exception("车辆节点小于2");
		}
		else if (vehicles.at(i).size() == 2) {
			vehicles.erase(vehicles.begin()+i);
		}
	}
	// 先进行排序（按照每辆车第二个节点的序号） TODO(Lvning): 完善排序规则
	sort(vehicles.begin(), vehicles.end(), [](IVehicle& v1, IVehicle& v2)->int { return v1.at(1) - v2.at(1); });
	// 将结果先输出到stringstream，然后再导出
	stringstream stream;
	for (int i = 0; i < vehicles.size(); i++) {
		stream << to_string(depot_);
		stream << ",";
		for (int j = 1; j < vehicles[i].size()-1; j++) {
			stream << to_string(vehicles[i].vehicle_nodes.at(j)) + ',';
		}
		stream << to_string(depot_);
		stream << '|';
	}
	string result;
	stream >> result;
	stream.str("");
	stream.clear();
	return result;
}

string Solution::to_string_without_update() {
	// 删除空车
	stringstream stream;
	for (int i = 0; i < vehicles.size(); i++) {
		stream << to_string(depot_);
		stream << ",";
		for (int j = 1; j < vehicles[i].size() - 1; j++) {
			stream << to_string(vehicles[i].vehicle_nodes.at(j)) + ',';
		}
		stream << to_string(depot_);
		stream << '|';
	}
	string result;
	stream >> result;
	stream.str("");
	stream.clear();
	return result;
}