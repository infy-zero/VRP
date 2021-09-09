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
bool Solution::isBetter(unique_ptr<Solution>& other) {
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
	string result = "\n";
	for (int i = 0; i < vehicles.size(); i++) {
		result += to_string(depot_);
		result += ",";
		for (int j = 1; j < vehicles[i].size()-1; j++) {
			result += to_string(vehicles[i].vehicle_nodes.at(j)) + ',';
		}
		result += to_string(depot_);
		result += '\n';
	}
	return result;
}