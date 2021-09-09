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
bool Solution::isBetter(unique_ptr<Solution>& other) {
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