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
	if (has_empty_vehicle() == false) { // ���û���ڿճ�
		return;
	}
	for (int i = vehicles.size() - 1; i >= 0; i--) {
		if (vehicles.at(i).is_empty()) {
			vehicles.erase(vehicles.begin() + i);
		}
	}
}

// ���ɾ��һ���ڵ�
void Solution::random_delete_without_update() {
	if (vehicles.empty()) {
		throw MyException("Empty vehicle can not be deleted.");
	}
	vector<int> prefix_sum(vehicles.size());
	// ����ǰ׺��
	for (int i = 0; i < vehicles.size(); i++) {
		if (i == 0) {
			prefix_sum.at(i) = vehicles.at(i).size() - 2;
		} else {
			prefix_sum.at(i) = prefix_sum.at(i - 1) + vehicles.at(i).size() - 2;
		}
	}
	// ѡ��ɾ���Ľڵ����
	int selected_node = static_cast<int> (prefix_sum.back() * Util::getRandom());
	int selected_vehicle_pos = -1;
	int selected_node_pos = -1;
	for (int i = 0; i < prefix_sum.size(); i++) { // Ѱ�ұ�ɾ���Ľڵ����ڳ����ź�λ��
		if (selected_node +1 > prefix_sum.at(i)) {
			continue;
		} else {
			selected_vehicle_pos = i;
			if (selected_vehicle_pos == 0) {		// ��һ������ֱ���Ǹ����
				selected_node_pos = selected_node + 1;
			} else {								// �����Ǹ���ż�ȥǰ׺��
				selected_node_pos = selected_node - prefix_sum.at(i - 1) + 1;
			}
			break;
		}
	}
	delete_without_update(selected_vehicle_pos, selected_node_pos);
}

// ɾ��ָ���ڵ�
void Solution::delete_without_update(int vehicle_pos, int node_pos) {
	IVehicle& vehicle = vehicles.at(vehicle_pos);
	int node_index = vehicle.delete_node(node_pos);
	removedList.push_back(node_index);
}

void Solution::addVehicle()
{
	// �����Ƿ��пճ����пճ����׳��쳣
	for (int i = 0; i < vehicles.size(); i++) {
		if (vehicles[i].size() <= 2) {

			throw MyException("You can not insert when an empty vehicle exists!");
		}
	}
	IVehicle tmp_vehicle(depot_, nodes_);
	vehicles.emplace_back(tmp_vehicle);
}

void Solution::insert(int vehicle_number, int pos, int node) {
	if (vehicles.empty()) { // �ǿ��ж�
		throw MyException("You can not insert into an empty vehicle!");
	}
	if (vehicle_number < 0 || vehicle_number >= vehicles.size()) { // �����ڸó���
		throw MyException("You can not insert into an non-exist vehicle!");
	}
	// ȡ����ǰ����
	IVehicle& vehicle = vehicles.at(vehicle_number);
	// ��posλ�ò���ڵ�node
	vehicle.insert(pos, node);
}

// �Ƚϵ�ǰ���other������
bool Solution::isBetter(shared_ptr<Solution>& other) {
	if (other == nullptr) {			// ���ȽϽ�Ϊ��ֱ�ӷ�����
		return true;
	}
	if (curVal < other->curVal) {	// ��ǰ��Ŀ�꺯��ֵ��С
		return true;
	}
	return false;
}


string Solution::toString() {
	// ɾ���ճ�
	for (int i = vehicles.size() - 1; i >= 0; i--) {
		if (vehicles.at(i).size() < 2) {
			throw new exception("�����ڵ�С��2");
		}
		else if (vehicles.at(i).size() == 2) {
			vehicles.erase(vehicles.begin()+i);
		}
	}
	// �Ƚ�������
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
	// ɾ���ճ�
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