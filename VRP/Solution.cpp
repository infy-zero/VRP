#include "Solution.h"
#include "MyException.h"

#include <algorithm>
#include <vector>


Solution::Solution(int depot, AllNodes* nodes):depot_(depot),nodes_(nodes) {
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
	IVehicle tmp_vehicle(depot_, nodes_);
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


string Solution::toString() {
	// 删除空车
	for (int i = vehicles.size() - 1; i >= 0; i--) {
		if (vehicles.at(i).size() < 2) {
			throw new exception("车辆节点小于2");
		}
		else if (vehicles.at(i).size() == 2) {
			vehicles.erase(vehicles.begin()+i);
		}
	}
	// 先进行排序
	// sort(sol.begin(), sol.end());
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