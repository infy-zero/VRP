#include "IVehicle.h"

#include <string.h>
#include <string>
#include <unordered_map>

#include "MyException.h"

IVehicle::IVehicle() {
	// 加载车辆成本
	vehicle_use_cost_ = A_S get_vehicle_cost(vehicle_type_);
	vehicle_per_cost_ = A_S get_vehicle_per_cost(vehicle_type_);
	vehicle_max_length_ = A_S get_vehicle_max_length(vehicle_type_);
	vehicle_max_time_ = A_S get_vehicle_max_time(vehicle_type_);
}
IVehicle::IVehicle(int depot, AllNodes* _nodes, bool is_initialize):depot_(depot),nodes_(_nodes){
	// 生成两个场站节点并插入
	if (is_initialize == true) {
		satisfy_condition();
	}
	// 加载车辆成本
	vehicle_use_cost_ = A_S get_vehicle_cost(vehicle_type_);
	vehicle_per_cost_ = A_S get_vehicle_per_cost(vehicle_type_);
	vehicle_max_length_ = A_S get_vehicle_max_length(vehicle_type_);
	vehicle_max_time_ = A_S get_vehicle_max_time(vehicle_type_);
}

void IVehicle::satisfy_condition() {
	if (get_total_size() == 0) { // 如果是空车
		if (A_S multi_trip == false || A_S multi_trip == true) { // 多轮次、非多轮次一致
			// 生成新的开始结束节点
			insert_depot(0, false);
			insert_depot(1, false);
			update();
			return;
		}
		
	} else {
		// 判断第一个和最后一个是场站
		NodeType first_type = nodes_->get_node_type(vehicle_nodes_.front());
		NodeType last_type = nodes_->get_node_type(vehicle_nodes_.back());
		if (first_type != TYPE_DEPOT || last_type != TYPE_DEPOT) {
			throw MyException("The first and last node should be depot.");
		}

		// 如果不是空车，多轮次要求每个场站成对存在（不存在单个，也不存在两个以上）
		if (A_S multi_trip == true) {
			// 双指针，左指针指向每一部分第一个场站位置，右指针=左值针+1
			for (int left = 0; left < get_total_size();) {
				// 当前节点不是场站则直接跳过
				if (nodes_->get_node_type(vehicle_nodes_.at(left)) != TYPE_DEPOT) {
					left++;
					continue;
				}
				int count = 0;	// 连续场站的数量 
				while (nodes_->get_node_type(vehicle_nodes_.at(left+count)) == TYPE_DEPOT) {
					count++;
					if (left + count >= vehicle_nodes_.size()) {
						break;
					}
				}
				if (count == 1) {
					insert_depot(left, false);
				}else if (count > 2) {
					for (int depot_pos = left + count - 1; depot_pos >= left + 2; depot_pos--) {
						delete_depot(depot_pos);
					}
				}
				left += 2;
			}
		}
	}
	is_update_ = false;
	update();
}

// 最大行驶距离和可达性
bool IVehicle::check_operator_feasible(unique_ptr<InsertDescription>& operator_description) {
	// 检验距离
	int pre_node_index = vehicle_nodes_.at(operator_description->node_pos_ - 1);
	int next_node_index = vehicle_nodes_.at(operator_description->node_pos_);
	int insert_node_index = operator_description->node_index_;
	ISolutionNode& pre_node = *nodes_->get_node(pre_node_index);
	ISolutionNode& next_node = *nodes_->get_node(next_node_index);
	ISolutionNode& inserted_node = *nodes_->get_node(insert_node_index);

	// 快速判断：1、上一节点最早出发时间超过插入节点最晚时间窗；(continue)
	if (pre_node.leave_earliest_time > inserted_node.task->predefined_service_latest_start_time_) {
		return false;
	}
	// 快速判断：2、插入节点最早出发时间超过下一节点最晚时间窗；（break）
	if (inserted_node.task->predefined_earliest_leave_time_ > next_node.service_latest_start_time) {
		return false;
	}

	// 行程总距离
	int trip_num = pre_node.trip_num;
	operator_description->save_value_ = inf cal_travel_distance(*nodes_, pre_node_index, next_node_index) - inf cal_travel_distance(*nodes_, pre_node_index, insert_node_index) - inf cal_travel_distance(*nodes_, insert_node_index, next_node_index);
	double tmp_length = trip_length_.at(trip_num) - operator_description->save_value_;
	if (tmp_length > vehicle_max_length_) {
		return false;
	}

	// 行程总时间 TODO(Lvning):比较复杂——目前处置方法：正推实际最晚到达时间差值
	// 可以确定的是，该行程之前和之后的时间均不会变得更长，因此计算该行程的时间，目前使用一种更紧的约束计算：即该轮次时间+时间差<=最大行程时间
	int pre_node_trip_pos = get_node_trip_pos(operator_description->node_pos_ - 1);
	double save_time = inf cal_travel_time(*nodes_, pre_node_index, next_node_index) - inf cal_travel_time(*nodes_, pre_node_index, insert_node_index) - inf cal_travel_time(*nodes_, insert_node_index, next_node_index);
	double tmp_trip_time = trip_time_[pre_node_trip_pos] - save_time;
	if (tmp_trip_time > vehicle_max_time_) {
		return false;
	}

	// 可达性（时间窗）
	double insert_node_arrive_earliest_time = pre_node.leave_earliest_time + inf cal_travel_time(*nodes_, pre_node_index, insert_node_index);
	if (insert_node_arrive_earliest_time > inserted_node.task->get_predefined_service_latest_start_time()) {
		return false;
	}
	double insert_node_service_earliest_time = max(inserted_node.task->get_predefined_earliest_service_start_time(), insert_node_arrive_earliest_time);
	double insert_node_leave_earliest_time = insert_node_service_earliest_time + inserted_node.task->get_total_service_time();
	double next_node_arrive_earliest_time = insert_node_leave_earliest_time + inf cal_travel_time(*nodes_, insert_node_index, next_node_index);
	if (next_node_arrive_earliest_time > next_node.service_latest_start_time) {
		return false;
	}
	return true;
}

// 利用时差插入法计算
void IVehicle::update(enum BasicOperator basic_operator) {
	if (is_update_ == true) {
		throw MyException("You update same vehicle twice.");
	}

	// 向后计算最早到达时间、最早开始服务时间、最早离开时间
	for (int i = 1; i < vehicle_nodes_.size(); i++) {
		backforward_calculate(i);
	}

	// 向前计算最晚离开时间、最晚开始服务时间、最晚到达时间（插入节点用）
	for (int i = vehicle_nodes_.size() - 2; i >= 0; i--) {
		forward_calculate(i);
	}
	// 向后计算实际最晚到达时间（计算trip_time用）
	ISolutionNode& first_depot = *nodes_->get_node(vehicle_nodes_.at(0));
	first_depot.actual_leave_latest_time_ = first_depot.leave_latest_time;
	for (int i = 1; i < vehicle_nodes_.size(); i++) {
		backward_calculate_actual(i);
	}
	// 计算每个行程节点数量、距离、时间、费用
	trip_node_num_.clear();
	trip_length_.clear();
	trip_time_.clear();
	total_length_ = 0;
	// 从第一个开始，每遇到一个depot，开始计数，计数结果就是多行程节点数量（每个depot是当前行程的结束和下一行程的开始，第一个和最后一个除外）
	int last_depot_pos = 0;
	int single_trip_node_num = 0;
	double single_trip_length = 0;
	double single_trip_time = 0;
	int cur_trip_num = 0;
	nodes_->get_node(vehicle_nodes_.front())->trip_num = 0;
	for (int i = 1; i < vehicle_nodes_.size(); i++) {
		// 每个行程还有到场站的距离
		single_trip_length += inf cal_travel_distance(*nodes_, vehicle_nodes_.at(i - 1), vehicle_nodes_.at(i)) + nodes_->get_node(vehicle_nodes_.at(i))->task->service_distance_; // 总距离=行驶距离+每个节点的时间运行距离

		if (nodes_->get_node_type(vehicle_nodes_.at(i)) == TYPE_DEPOT) {
			trip_node_num_.push_back(single_trip_node_num);
			trip_length_.push_back(single_trip_length);
			// 每个行程的时间使用最晚到达时间(此处都是场站，使用的都是最晚出发时间)
			double trip_end_time;
			double trip_start_time = nodes_->get_node(vehicle_nodes_.at(last_depot_pos))->leave_latest_time;
			if (i != vehicle_nodes_.size() - 1) {
				trip_end_time = nodes_->get_node(vehicle_nodes_.at(last_depot_pos))->leave_latest_time;
			} else {
				trip_end_time = nodes_->get_node(vehicle_nodes_.at(vehicle_nodes_.size() - 2))->leave_latest_time + inf cal_travel_distance(*nodes_, vehicle_nodes_.at(vehicle_nodes_.size() - 2), vehicle_nodes_.at(vehicle_nodes_.size() - 1)) + nodes_->get_node(vehicle_nodes_.at(vehicle_nodes_.size() - 1))->task->total_service_time_;
			}
			trip_time_.push_back(trip_end_time - trip_start_time);
			total_length_ += single_trip_length;
			single_trip_node_num = 0;
			single_trip_length = 0;
			last_depot_pos = i;
			nodes_->get_node(vehicle_nodes_.at(i))->trip_num = ++cur_trip_num;
			continue;
		}
		single_trip_node_num++;
		nodes_->get_node(vehicle_nodes_.at(i))->trip_num = cur_trip_num;
	}
	// 总时间使用最晚到达时间（实际最晚离开时间的差值）
	total_time_ = nodes_->get_node(vehicle_nodes_.at(vehicle_nodes_.size()-1))->actual_leave_latest_time_ - nodes_->get_node(vehicle_nodes_.front())->actual_leave_latest_time_;
	total_cost_ = vehicle_per_cost_ * total_length_ + vehicle_use_cost_;
	is_update_ = true;
}
int IVehicle::get_node_index(int index) {
	return vehicle_nodes_.at(index);
}

// 计算总费用
double IVehicle::get_cost() {
	if (is_update_ == false) {
		throw MyException("Have not been updated.");
	}
	return total_cost_;
}
bool IVehicle::beyond_max_length() {
	if (is_update_ == false) {
		throw MyException("Have not been updated.");
	}
	return total_length_ > vehicle_max_length_;
}

void IVehicle::insert_node(int pos, int node, bool try_update) {
	if ((try_update == true) && (is_update_ == false)) {
		throw MyException("You have not update.");
	}
	// 判断节点是否存在
	if (!nodes_->count(node)) {
		throw MyException("You can not insert non-exist node!");
	}
	// TODO: 判断vehicle_nodes.size() + 1是否为最后一位。
	if (pos <= 0 || pos >= vehicle_nodes_.size()) {
		cout << "pos = " << pos << ", range = [1," << vehicle_nodes_.size() << ")" << endl;
		throw MyException("This position is not allowed for insertion!" );
	}
	vehicle_nodes_.insert(vehicle_nodes_.begin() + pos, node);
	total_node_num_++;
	is_update_ = false;
	if (try_update) {
		satisfy_condition();
	}
}

void IVehicle::delete_node(int pos, vector<int>& removed_list) {
	int node_index = vehicle_nodes_.at(pos);
	if (node_index < 0) {	// 不能删除场站节点
		throw MyException("Can not delete depot.");
	}
	vehicle_nodes_.erase(vehicle_nodes_.begin() + pos);
	removed_list.push_back(node_index);
	total_node_num_--;
	is_update_ = false;
}
void IVehicle::delete_depot(int pos) {
	int depot_index = vehicle_nodes_.at(pos);
	if (depot_index >= 0) {	// 不能删除场站节点
		throw MyException("You can not delete node");
	}
	nodes_->remove_depot(depot_index);
	vehicle_nodes_.erase(vehicle_nodes_.begin() + pos);
	total_depot_num_--;
	is_update_ = false;
}
void IVehicle::insert_depot(int pos, bool try_update) {
	if ((try_update == true) && (is_update_ == false)) {
		throw MyException("You have not update.");
	}
	ISolutionNode* depot = new ISolutionNode(*nodes_->get_node(depot_,0b100));
	int depot_index = nodes_->push_depot(depot);
	vehicle_nodes_.insert(vehicle_nodes_.begin()+pos, depot_index);
	total_depot_num_++;
	is_update_ = false;
	if (try_update == true) {
		satisfy_condition();
	}
}
// 现在只有两个场站节点，所以直接-2,TODO(Lvning): 更新计算方法
int IVehicle::get_node_size() {
	return total_node_num_;
}
int IVehicle::get_total_size() {
	return vehicle_nodes_.size();
}
bool IVehicle::get_is_update() {
	return is_update_;
}
// 节点数小于2，直接报错；节点中含有非场站节点，直接返回false；否则节点中只有场站节点，返回true,TODO(Lvning): 加入node和depot计数
bool IVehicle::is_empty() {
	if (get_total_size() < 2) {
		throw MyException("Illegal vehicle!");
	}
	if (total_node_num_ != 0) {
		return false;
	}
	return true;
}

// 最大行驶距离和可达性
void IVehicle::cal_delete_save_value(unique_ptr<DeleteDescription>& current_operator_description) {
	int pre_node_index = vehicle_nodes_.at(current_operator_description->node_pos_ - 1);
	int cur_node_index = vehicle_nodes_.at(current_operator_description->node_pos_);
	int next_node_index = vehicle_nodes_.at(current_operator_description->node_pos_ + 1);
	current_operator_description->save_value_ = inf cal_travel_distance(*nodes_, pre_node_index, cur_node_index) + inf cal_travel_distance(*nodes_, cur_node_index, next_node_index) - inf cal_travel_distance(*nodes_, pre_node_index, next_node_index);
}
void IVehicle::clear() {
	for (int i = vehicle_nodes_.size() - 1; i >= 0; i--) {
		delete_depot(i);
	}
}

// 时差插入法，正向计算最早到达时间、最早开始服务时间、最早离开时间
void IVehicle::backforward_calculate(int cur_pos) {
	if (cur_pos <= 0 || cur_pos >= vehicle_nodes_.size()) {
		throw MyException("Wrong position.");
	}
	ISolutionNode& pre_node = *nodes_->get_node(vehicle_nodes_.at(cur_pos - 1));
	ISolutionNode& cur_node = *nodes_->get_node(vehicle_nodes_.at(cur_pos));
	int pre_node_index = vehicle_nodes_.at(cur_pos - 1);
	int cur_node_index = vehicle_nodes_.at(cur_pos);
	// 到达时间 = 上一节点离开时间 + 旅行时间
	cur_node.arrive_earliest_time = pre_node.leave_earliest_time + inf cal_travel_time(*nodes_, pre_node_index, cur_node_index);
	cur_node.service_earliest_start_time = max(cur_node.arrive_earliest_time, cur_node.task->get_predefined_earliest_service_start_time());
	cur_node.leave_earliest_time = cur_node.service_earliest_start_time + cur_node.task->get_total_service_time();
	if (cur_node.arrive_earliest_time > cur_node.task->get_predefined_service_latest_start_time()) {
		throw MyException("The task can not be insert here.");
	}
}

void IVehicle::backward_calculate_actual(int cur_pos) {
	if (cur_pos <= 0 || cur_pos >= vehicle_nodes_.size()) {
		throw MyException("Wrong position.");
	}
	ISolutionNode& pre_node = *nodes_->get_node(vehicle_nodes_.at(cur_pos - 1));
	ISolutionNode& cur_node = *nodes_->get_node(vehicle_nodes_.at(cur_pos));
	int pre_node_index = vehicle_nodes_.at(cur_pos - 1);
	int cur_node_index = vehicle_nodes_.at(cur_pos);
	// 到达时间 = 开始服务时间 = 上一节点离开时间 + 旅行时间
	cur_node.actual_arrive_latest_time_ = pre_node.actual_leave_latest_time_ + inf cal_travel_time(*nodes_, pre_node_index, cur_node_index);
	cur_node.actual_service_latest_time_ = max(cur_node.actual_arrive_latest_time_, cur_node.task->get_predefined_earliest_service_start_time());
	// 考虑到计算时候的机器误差
	if (cur_node.actual_service_latest_time_ > cur_node.service_latest_start_time + A_S time_minimum_delta) {
		throw MyException("The task can not be insert here.");
	}
	cur_node.actual_leave_latest_time_ = cur_node.actual_arrive_latest_time_ + cur_node.task->get_total_service_time();
	
}

// 时差插入法，反向计算最晚到达时间、最晚开始服务时间、最晚离开时间
void IVehicle::forward_calculate(int cur_pos) {
	if (cur_pos < 0 || cur_pos >= vehicle_nodes_.size()-1) {
		throw MyException("Wrong position.");
	}
	ISolutionNode& next_node = *nodes_->get_node(vehicle_nodes_.at(cur_pos + 1));
	ISolutionNode& cur_node = *nodes_->get_node(vehicle_nodes_.at(cur_pos));
	int next_node_index = vehicle_nodes_.at(cur_pos + 1);
	int cur_node_index = vehicle_nodes_.at(cur_pos);
	// 最晚离开时间
	cur_node.leave_latest_time = next_node.arrive_latest_time - inf cal_travel_time(*nodes_, cur_node_index, next_node_index);
	cur_node.service_latest_start_time = min(cur_node.task->predefined_service_latest_start_time_, cur_node.leave_latest_time - cur_node.task->get_total_service_time());
	cur_node.arrive_latest_time = cur_node.service_latest_start_time;
	if (cur_node.arrive_latest_time > cur_node.task->get_predefined_service_latest_start_time()) {
		throw MyException("The task can not be insert here.");
	}
}

const double IVehicle::get_max_length() {
	return vehicle_max_length_;
}
void IVehicle::set_all_nodes(AllNodes* all_nodes) {
	nodes_ = all_nodes;
}	
int IVehicle::get_node_trip_pos(int node_pos) {
	if (node_pos == 0) {
		return 0;
	}
	for (int i = 0; i < trip_node_num_.size(); i++) {
		if (node_pos > trip_node_num_[i] + 2) {
			node_pos -= trip_node_num_[i] + 2;
		}
		else {
			return i;
		}
	}
	throw MyException("Can't find the trip index.");
	return -1;
}