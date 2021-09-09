#include "IVehicle.h"

#include <string.h>
#include <string>
#include <unordered_map>

#include "MyException.h"

IVehicle::IVehicle(int _depot, AllNodes* _nodes):depot_(_depot),nodes(_nodes) {
	// ����������վ�ڵ�
	vehicle_nodes.push_back(depot_);
	vehicle_nodes.push_back(depot_);
	// ���س����ɱ�
	vehicle_cost = inf get_vehicle_cost(vehicle_type);
	vehicle_per_cost = inf get_vehicle_per_cost(vehicle_type);
}

double IVehicle::cal_cost() {
	if (vehicle_nodes.size() < 2) {
		throw MyException("The number of this vehicle is wrong.");
	}
	double total_cost = 0;
	for (int i = 1; i < vehicle_nodes.size(); i++) { // ��������
		int pre  = vehicle_nodes.at(i - 1);
		int next = vehicle_nodes.at(i);
		total_cost += inf cal_travel_time(pre, next) * vehicle_per_cost;
	}
	total_cost += vehicle_cost;
	return total_cost;
}

void IVehicle::insert(int pos, int node) {
	// �жϽڵ��Ƿ����
	if (!nodes->count(node)) {
		throw MyException("You can not insert non-exist node!");
	}
	// TODO: �ж�vehicle_nodes.size() + 1�Ƿ�Ϊ���һλ��
	if (pos <= 0 || pos >= vehicle_nodes.size()) {
		cout << "pos = " << pos << ", range = [1," << vehicle_nodes.size() << ")" << endl;
		throw MyException("This position is not allowed for insertion!" );
	}
	vehicle_nodes.insert(vehicle_nodes.begin() + pos, node);
}
int IVehicle::size() {
	return vehicle_nodes.size();
}