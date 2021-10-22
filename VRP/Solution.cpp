#include "FerryTaskSetting.h"
#include "Solution.h"
#include "MyException.h"

#include <algorithm>
#include <vector>

Solution::Solution(int depot, AllNodes nodes):depot_(depot),all_nodes_(nodes) {
}
Solution::Solution(const Solution& other): cur_val_(other.cur_val_),all_nodes_(other.all_nodes_), depot_(other.depot_), removedList(other.removedList), total_cost(other.total_cost), solution_string_(other.solution_string_) {
	
	vehicles.clear();
	for (IVehicle* vehicle : other.vehicles) {
		vehicles.push_back(new IVehicle(*vehicle));
	}
	unify_vehicles_nodes();
}
Solution::~Solution() {
	for (IVehicle* vehicle : vehicles) {
		delete vehicle;
	}
	vehicles.clear();
}

void Solution::update() {
	cur_val_ = 0;
	for (IVehicle* vehicle : vehicles) {
		cur_val_ += vehicle->get_cost();
	}
}
void Solution::update_after_delete() {
	for (IVehicle* vehicle : vehicles) {
		if (vehicle->get_is_update() == false) {
			vehicle->update();
		}
	}
	clear_empty_vehicle();
	update();
}
bool Solution::has_empty_vehicle() {
	if (vehicles.size() == 0) {
		return false;
	}
	for (IVehicle* vehicle : vehicles) {
		if (vehicle->is_empty()) {
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
		if (vehicles.at(i)->is_empty()) {
			vehicles.at(i)->clear();
			vehicles.erase(vehicles.begin() + i);
		}
	}
}


// 比较当前解和other的优劣
const bool Solution::isBetter(shared_ptr<Solution>& other) {
	if (other == nullptr) {			// 待比较解为空直接返回真
		return true;
	}
	if (cur_val_ < other->cur_val_) {	// 当前解目标函数值更小
		return true;
	}
	return false;
}
const bool Solution::is_worse(shared_ptr<Solution>& other) {
	if (other == nullptr) {			// 待比较解为空直接返回真
		return true;
	}
	if (cur_val_ > other->cur_val_) {	// 当前解目标函数值更小
		return true;
	}
	return false;
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
			prefix_sum.at(i) = vehicles.at(i)->get_total_size() - 2;
		} else {
			prefix_sum.at(i) = prefix_sum.at(i - 1) + vehicles.at(i)->get_total_size() - 2;
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
	IVehicle* vehicle = vehicles.at(vehicle_pos);
	vehicle->delete_node(node_pos, removedList);
}
// 加入一辆空车
void Solution::add_empty_vehicle()
{
	// 检验是否有空车，有空车则抛出异常
	for (int i = 0; i < vehicles.size(); i++) {
		if (vehicles[i]->is_empty()==true) {

			throw MyException("You can not insert when an empty vehicle exists!");
		}
	}
	IVehicle* tmp_vehicle = new IVehicle(depot_, &all_nodes_, false);
	vehicles.emplace_back(tmp_vehicle);
	vehicles.back()->satisfy_condition();
}

void Solution::insert(int vehicle_number, int pos, int node) {
	if (vehicles.empty()) { // 非空判断
		throw MyException("You can not insert into an empty vehicle!");
	}
	if (vehicle_number < 0 || vehicle_number >= vehicles.size()) { // 不存在该车辆
		throw MyException("You can not insert into an non-exist vehicle!");
	}
	// 取出当前车辆
	IVehicle* vehicle = vehicles.at(vehicle_number);
	// 在pos位置插入节点node
	vehicle->insert_node(pos, node);
}
const int Solution::get_empty_vehicle_num() {
	int empty_vehicle_num = 0;
	if (vehicles.size() == 0) {
		return 0;
	}
	for (IVehicle* vehicle : vehicles) {
		if (vehicle->is_empty()) {
			empty_vehicle_num++;
		}
	}
	return empty_vehicle_num;
}
void Solution::make_single_empty_vehicle() {
	int empty_vehicle_num = get_empty_vehicle_num();
	if (empty_vehicle_num == 0) {
		add_empty_vehicle();
		return;
	} else {
		throw MyException("Already has empty vehicle.");
	}
}

// 生成可行解字符串，push_solution_space专用
string Solution::toString_with_update() {
	// 删除空车
	for (int i = vehicles.size() - 1; i >= 0; i--) {
		if (vehicles.at(i)->get_total_size() < 2) {
			throw new exception("车辆节点小于2");
		}
		else if (vehicles[i]->is_empty() == true) {
			vehicles[i]->clear();
			vehicles.erase(vehicles.begin()+i);
		}
	}
	for (IVehicle* vehicle : vehicles) {
		vehicle->satisfy_condition();
	}
	// 先进行排序（按照每辆车第二个节点的序号） TODO(Lvning): 完善排序规则
	sort(vehicles.begin(), vehicles.end(), [](IVehicle* v1, IVehicle* v2)->bool
		{
			int a1, a2;
			if (A_S multi_trip == true) {
				a1 = v1->get_node_index(3);
				a2 = v2->get_node_index(3);
			} else {
				a1 = v1->get_node_index(2);
				a2 = v2->get_node_index(2);
			}
			
			if (a1 < a2) {
				return true;
			}
			else {
				return false;
			}
		});
	// 将结果先输出到stringstream，然后再导出
	stringstream stream;
	for (int i = 0; i < vehicles.size(); i++) {
		stream << to_string(depot_);
		stream << ",";
		for (int j = 1; j < vehicles[i]->get_total_size()-1; j++) {
			if (vehicles[i]->get_node_index(j) < 0) {
				stream << to_string(depot_) + ',';
			} else {
				stream << to_string(vehicles[i]->get_node_index(j)) + ',';
			}
		}
		stream << to_string(depot_);
		stream << '|';
	}
	string result;
	stream >> result;
	stream.str("");
	stream.clear();
	solution_string_ = result;
	return result;
}

string Solution::to_string_without_update() {
	// 删除空车
	stringstream stream;
	for (int i = 0; i < vehicles.size(); i++) {
		for (int j = 0; j < vehicles[i]->get_total_size()-1; j++) {
			stream << to_string(vehicles[i]->get_node_index(j)) + ',';
		}
		stream << to_string(vehicles[i]->get_node_index(vehicles[i]->get_total_size()-1));
		stream << '|';
	}
	string result;
	stream >> result;
	stream.str("");
	stream.clear();
	return result;
}
// TODO(Lvning): 更新计算方法
const int Solution::get_total_node_num() {
	int total_node_num = 0;
	for (IVehicle* vehicle : vehicles) {
		total_node_num += vehicle->get_node_size();
	}
	return total_node_num;
}
void Solution::unify_vehicles_nodes() {
	for (IVehicle* vehicle : vehicles) {
		vehicle->set_all_nodes(&all_nodes_);
	}
}
void Solution::satisfy_condition() {
	for (IVehicle* vehicle : vehicles) {
		vehicle->satisfy_condition();
	}
}
void Solution::clear() {
	for (IVehicle* vehicle : vehicles) {
		vehicle->clear();
	}
}
// 当前解一定是可行解，只需要判断被插入的车辆是否可行即可
bool Solution::check_operator_feasible(unique_ptr<InsertDescription>& operator_description) {
	// 判断当前解是否可行
	if (this == nullptr || vehicles.empty()) {
		throw MyException("You can check non-exist solution.");
	}
	IVehicle* vehicle = vehicles.at(operator_description->vehicle_pos_);
	/*限制条件*/
	// 最大行驶距离限制+可达性
	if (!vehicle->check_operator_feasible(operator_description)) {
		return false;
	}
	return true;
}
// 执行算子操作
void Solution::excute_insert_operator(unique_ptr<InsertDescription>& operator_description) {
	int node_index = operator_description->node_index_;
	int removed_list_pos = operator_description->removed_list_pos_;
	if (removedList.at(removed_list_pos) != node_index) {
		throw MyException("Wrong node selected!");
	}
	// 从剩余列表删除
	removedList.erase(removedList.begin() + removed_list_pos);
	// 加入到当前解
	IVehicle* vehicle = vehicles.at(operator_description->vehicle_pos_);
	vehicle->insert_node(operator_description->node_pos_, node_index);
}
// 计算删除节点的节约值
void Solution::cal_delete_save_value(unique_ptr<DeleteDescription>& current_operator_description) {
	IVehicle* vehicle = vehicles.at(current_operator_description->vehicle_pos_);
	vehicle->cal_delete_save_value(current_operator_description);
}
void Solution::excute_insert_operator(unique_ptr<DeleteDescription>& current_operator_description) {
	IVehicle* vehicle = vehicles.at(current_operator_description->vehicle_pos_);
	vehicle->delete_node(current_operator_description->node_pos_, removedList);
}