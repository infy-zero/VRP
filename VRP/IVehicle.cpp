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
}

double IVehicle::cal_cost() {
	if (vehicle_nodes.size() < 2) {
		throw MyException("The number of this vehicle is wrong.");
	}
	double total_cost = 0;
	for (int i = 1; i < vehicle_nodes.size(); i++) { // 遍历车辆
		int pre  = vehicle_nodes.at(i - 1);
		int next = vehicle_nodes.at(i);
		total_cost += inf cal_travel_time(pre, next) * vehicle_per_cost;
	}
	total_cost += vehicle_cost;
	return total_cost;
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
int IVehicle::size() {
	return vehicle_nodes.size();
}