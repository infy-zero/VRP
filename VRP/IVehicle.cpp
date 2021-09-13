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
	vehicle_max_length = inf get_vehicle_max_length(vehicle_type);
}

int IVehicle::at(int index) {
	return vehicle_nodes.at(index);
}
// �����ܾ���
double IVehicle::cal_length() {
	if (vehicle_nodes.size() < 2) {
		throw MyException("The number of this vehicle is wrong.");
	}
	double total_length = 0;
	for (int i = 1; i < vehicle_nodes.size(); i++) { // ��������
		int pre = vehicle_nodes.at(i - 1);
		int next = vehicle_nodes.at(i);
		total_length += inf cal_travel_time(pre, next);
	}
	return total_length;
}

// �����ܷ���
double IVehicle::cal_cost() {
	// �ܷ��� = �ܳ���*����+����ʹ�÷���
	double total_cost = cal_length() * vehicle_per_cost + vehicle_cost;
	return total_cost;
}
bool IVehicle::beyond_max_length() {
	return cal_length() > vehicle_max_length;
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

int IVehicle::delete_node(int pos) {
	int node_index = vehicle_nodes.at(pos);
	if (node_index < 0) {	// ����ɾ����վ�ڵ�
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