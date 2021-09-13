#include "IVehicle.h"

#include <string.h>
#include <string>
#include <unordered_map>

#include "MyException.h"

IVehicle::IVehicle(int _depot, AllNodes* _nodes):depot_(_depot),nodes(_nodes) {
	// 插入两个场站节点
	vehicle_nodes.push_back(depot_);
	vehicle_nodes.push_back(depot_);
	// 加载车辆成本
	vehicle_cost = inf get_vehicle_cost(vehicle_type);
	vehicle_per_cost = inf get_vehicle_per_cost(vehicle_type);
	vehicle_max_length = inf get_vehicle_max_length(vehicle_type);
}

int IVehicle::at(int index) {
	return vehicle_nodes.at(index);
}
// 计算总距离
double IVehicle::cal_length() {
	if (vehicle_nodes.size() < 2) {
		throw MyException("The number of this vehicle is wrong.");
	}
	double total_length = 0;
	for (int i = 1; i < vehicle_nodes.size(); i++) { // 遍历车辆
		int pre = vehicle_nodes.at(i - 1);
		int next = vehicle_nodes.at(i);
		total_length += inf cal_travel_time(pre, next);
	}
	return total_length;
}

// 计算总费用
double IVehicle::cal_cost() {
	// 总费用 = 总长度*单价+车辆使用费用
	double total_cost = cal_length() * vehicle_per_cost + vehicle_cost;
	return total_cost;
}
bool IVehicle::beyond_max_length() {
	return cal_length() > vehicle_max_length;
}

void IVehicle::insert(int pos, int node) {
	// 判断节点是否存在
	if (!nodes->count(node)) {
		throw MyException("You can not insert non-exist node!");
	}
	// TODO: 判断vehicle_nodes.size() + 1是否为最后一位。
	if (pos <= 0 || pos >= vehicle_nodes.size()) {
		cout << "pos = " << pos << ", range = [1," << vehicle_nodes.size() << ")" << endl;
		throw MyException("This position is not allowed for insertion!" );
	}
	vehicle_nodes.insert(vehicle_nodes.begin() + pos, node);
}

int IVehicle::delete_node(int pos) {
	int node_index = vehicle_nodes.at(pos);
	if (node_index < 0) {	// 不能删除场站节点
		throw MyException("You can not delete depot");
	}
	vehicle_nodes.erase(vehicle_nodes.begin() + pos);
	return node_index;
}

int IVehicle::size() {
	return vehicle_nodes.size();
}
bool IVehicle::is_empty() {
	if (size() < 2) {
		throw MyException("Illegal vehicle!");
	} else if (size() == 2) {
		return true;
	} else {
		return false;
	}
}