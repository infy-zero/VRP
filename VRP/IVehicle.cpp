#include "IVehicle.h"

#include <string.h>
#include <string>
#include <unordered_map>

#include "MyException.h"

IVehicle::IVehicle() {
	// ���س����ɱ�
	vehicle_use_cost_ = A_S get_vehicle_cost(vehicle_type_);
	vehicle_per_cost_ = A_S get_vehicle_per_cost(vehicle_type_);
	vehicle_max_length_ = A_S get_vehicle_max_length(vehicle_type_);
	vehicle_max_time_ = A_S get_vehicle_max_time(vehicle_type_);
}
IVehicle::IVehicle(int depot, AllNodes* _nodes, bool is_initialize):depot_(depot),nodes_(_nodes){
	// ����������վ�ڵ㲢����
	if (is_initialize == true) {
		satisfy_condition();
	}
	// ���س����ɱ�
	vehicle_use_cost_ = A_S get_vehicle_cost(vehicle_type_);
	vehicle_per_cost_ = A_S get_vehicle_per_cost(vehicle_type_);
	vehicle_max_length_ = A_S get_vehicle_max_length(vehicle_type_);
	vehicle_max_time_ = A_S get_vehicle_max_time(vehicle_type_);
}

void IVehicle::satisfy_condition() {
	if (get_total_size() == 0) { // ����ǿճ�
		if (A_S multi_trip == false || A_S multi_trip == true) { // ���ִΡ��Ƕ��ִ�һ��
			// �����µĿ�ʼ�����ڵ�
			insert_depot(0, false);
			insert_depot(1, false);
			update();
			return;
		}
		
	} else {
		// �жϵ�һ�������һ���ǳ�վ
		NodeType first_type = nodes_->get_node_type(vehicle_nodes_.front());
		NodeType last_type = nodes_->get_node_type(vehicle_nodes_.back());
		if (first_type != TYPE_DEPOT || last_type != TYPE_DEPOT) {
			throw MyException("The first and last node should be depot.");
		}

		// ������ǿճ������ִ�Ҫ��ÿ����վ�ɶԴ��ڣ������ڵ�����Ҳ�������������ϣ�
		if (A_S multi_trip == true) {
			// ˫ָ�룬��ָ��ָ��ÿһ���ֵ�һ����վλ�ã���ָ��=��ֵ��+1
			for (int left = 0; left < get_total_size();) {
				// ��ǰ�ڵ㲻�ǳ�վ��ֱ������
				if (nodes_->get_node_type(vehicle_nodes_.at(left)) != TYPE_DEPOT) {
					left++;
					continue;
				}
				int count = 0;	// ������վ������ 
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

// �����ʻ����Ϳɴ���
bool IVehicle::check_operator_feasible(unique_ptr<InsertDescription>& operator_description) {
	// �������
	int pre_node_index = vehicle_nodes_.at(operator_description->node_pos_ - 1);
	int next_node_index = vehicle_nodes_.at(operator_description->node_pos_);
	int insert_node_index = operator_description->node_index_;
	ISolutionNode& pre_node = *nodes_->get_node(pre_node_index);
	ISolutionNode& next_node = *nodes_->get_node(next_node_index);
	ISolutionNode& inserted_node = *nodes_->get_node(insert_node_index);

	// �����жϣ�1����һ�ڵ��������ʱ�䳬������ڵ�����ʱ�䴰��(continue)
	if (pre_node.leave_earliest_time > inserted_node.task->predefined_service_latest_start_time_) {
		return false;
	}
	// �����жϣ�2������ڵ��������ʱ�䳬����һ�ڵ�����ʱ�䴰����break��
	if (inserted_node.task->predefined_earliest_leave_time_ > next_node.service_latest_start_time) {
		return false;
	}

	// �г��ܾ���
	int trip_num = pre_node.trip_num;
	operator_description->save_value_ = inf cal_travel_distance(*nodes_, pre_node_index, next_node_index) - inf cal_travel_distance(*nodes_, pre_node_index, insert_node_index) - inf cal_travel_distance(*nodes_, insert_node_index, next_node_index);
	double tmp_length = trip_length_.at(trip_num) - operator_description->save_value_;
	if (tmp_length > vehicle_max_length_) {
		return false;
	}

	// �г���ʱ�� TODO(Lvning):�Ƚϸ��ӡ���Ŀǰ���÷���������ʵ��������ʱ���ֵ
	// ����ȷ�����ǣ����г�֮ǰ��֮���ʱ��������ø�������˼�����г̵�ʱ�䣬Ŀǰʹ��һ�ָ�����Լ�����㣺�����ִ�ʱ��+ʱ���<=����г�ʱ��
	int pre_node_trip_pos = get_node_trip_pos(operator_description->node_pos_ - 1);
	double save_time = inf cal_travel_time(*nodes_, pre_node_index, next_node_index) - inf cal_travel_time(*nodes_, pre_node_index, insert_node_index) - inf cal_travel_time(*nodes_, insert_node_index, next_node_index);
	double tmp_trip_time = trip_time_[pre_node_trip_pos] - save_time;
	if (tmp_trip_time > vehicle_max_time_) {
		return false;
	}

	// �ɴ��ԣ�ʱ�䴰��
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

// ����ʱ����뷨����
void IVehicle::update(enum BasicOperator basic_operator) {
	if (is_update_ == true) {
		throw MyException("You update same vehicle twice.");
	}

	// ���������絽��ʱ�䡢���翪ʼ����ʱ�䡢�����뿪ʱ��
	for (int i = 1; i < vehicle_nodes_.size(); i++) {
		backforward_calculate(i);
	}

	// ��ǰ���������뿪ʱ�䡢����ʼ����ʱ�䡢������ʱ�䣨����ڵ��ã�
	for (int i = vehicle_nodes_.size() - 2; i >= 0; i--) {
		forward_calculate(i);
	}
	// ������ʵ��������ʱ�䣨����trip_time�ã�
	ISolutionNode& first_depot = *nodes_->get_node(vehicle_nodes_.at(0));
	first_depot.actual_leave_latest_time_ = first_depot.leave_latest_time;
	for (int i = 1; i < vehicle_nodes_.size(); i++) {
		backward_calculate_actual(i);
	}
	// ����ÿ���г̽ڵ����������롢ʱ�䡢����
	trip_node_num_.clear();
	trip_length_.clear();
	trip_time_.clear();
	total_length_ = 0;
	// �ӵ�һ����ʼ��ÿ����һ��depot����ʼ����������������Ƕ��г̽ڵ�������ÿ��depot�ǵ�ǰ�г̵Ľ�������һ�г̵Ŀ�ʼ����һ�������һ�����⣩
	int last_depot_pos = 0;
	int single_trip_node_num = 0;
	double single_trip_length = 0;
	double single_trip_time = 0;
	int cur_trip_num = 0;
	nodes_->get_node(vehicle_nodes_.front())->trip_num = 0;
	for (int i = 1; i < vehicle_nodes_.size(); i++) {
		// ÿ���г̻��е���վ�ľ���
		single_trip_length += inf cal_travel_distance(*nodes_, vehicle_nodes_.at(i - 1), vehicle_nodes_.at(i)) + nodes_->get_node(vehicle_nodes_.at(i))->task->service_distance_; // �ܾ���=��ʻ����+ÿ���ڵ��ʱ�����о���

		if (nodes_->get_node_type(vehicle_nodes_.at(i)) == TYPE_DEPOT) {
			trip_node_num_.push_back(single_trip_node_num);
			trip_length_.push_back(single_trip_length);
			// ÿ���г̵�ʱ��ʹ��������ʱ��(�˴����ǳ�վ��ʹ�õĶ����������ʱ��)
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
	// ��ʱ��ʹ��������ʱ�䣨ʵ�������뿪ʱ��Ĳ�ֵ��
	total_time_ = nodes_->get_node(vehicle_nodes_.at(vehicle_nodes_.size()-1))->actual_leave_latest_time_ - nodes_->get_node(vehicle_nodes_.front())->actual_leave_latest_time_;
	total_cost_ = vehicle_per_cost_ * total_length_ + vehicle_use_cost_;
	is_update_ = true;
}
int IVehicle::get_node_index(int index) {
	return vehicle_nodes_.at(index);
}

// �����ܷ���
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
	// �жϽڵ��Ƿ����
	if (!nodes_->count(node)) {
		throw MyException("You can not insert non-exist node!");
	}
	// TODO: �ж�vehicle_nodes.size() + 1�Ƿ�Ϊ���һλ��
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
	if (node_index < 0) {	// ����ɾ����վ�ڵ�
		throw MyException("Can not delete depot.");
	}
	vehicle_nodes_.erase(vehicle_nodes_.begin() + pos);
	removed_list.push_back(node_index);
	total_node_num_--;
	is_update_ = false;
}
void IVehicle::delete_depot(int pos) {
	int depot_index = vehicle_nodes_.at(pos);
	if (depot_index >= 0) {	// ����ɾ����վ�ڵ�
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
// ����ֻ��������վ�ڵ㣬����ֱ��-2,TODO(Lvning): ���¼��㷽��
int IVehicle::get_node_size() {
	return total_node_num_;
}
int IVehicle::get_total_size() {
	return vehicle_nodes_.size();
}
bool IVehicle::get_is_update() {
	return is_update_;
}
// �ڵ���С��2��ֱ�ӱ����ڵ��к��зǳ�վ�ڵ㣬ֱ�ӷ���false������ڵ���ֻ�г�վ�ڵ㣬����true,TODO(Lvning): ����node��depot����
bool IVehicle::is_empty() {
	if (get_total_size() < 2) {
		throw MyException("Illegal vehicle!");
	}
	if (total_node_num_ != 0) {
		return false;
	}
	return true;
}

// �����ʻ����Ϳɴ���
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

// ʱ����뷨������������絽��ʱ�䡢���翪ʼ����ʱ�䡢�����뿪ʱ��
void IVehicle::backforward_calculate(int cur_pos) {
	if (cur_pos <= 0 || cur_pos >= vehicle_nodes_.size()) {
		throw MyException("Wrong position.");
	}
	ISolutionNode& pre_node = *nodes_->get_node(vehicle_nodes_.at(cur_pos - 1));
	ISolutionNode& cur_node = *nodes_->get_node(vehicle_nodes_.at(cur_pos));
	int pre_node_index = vehicle_nodes_.at(cur_pos - 1);
	int cur_node_index = vehicle_nodes_.at(cur_pos);
	// ����ʱ�� = ��һ�ڵ��뿪ʱ�� + ����ʱ��
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
	// ����ʱ�� = ��ʼ����ʱ�� = ��һ�ڵ��뿪ʱ�� + ����ʱ��
	cur_node.actual_arrive_latest_time_ = pre_node.actual_leave_latest_time_ + inf cal_travel_time(*nodes_, pre_node_index, cur_node_index);
	cur_node.actual_service_latest_time_ = max(cur_node.actual_arrive_latest_time_, cur_node.task->get_predefined_earliest_service_start_time());
	// ���ǵ�����ʱ��Ļ������
	if (cur_node.actual_service_latest_time_ > cur_node.service_latest_start_time + A_S time_minimum_delta) {
		throw MyException("The task can not be insert here.");
	}
	cur_node.actual_leave_latest_time_ = cur_node.actual_arrive_latest_time_ + cur_node.task->get_total_service_time();
	
}

// ʱ����뷨���������������ʱ�䡢����ʼ����ʱ�䡢�����뿪ʱ��
void IVehicle::forward_calculate(int cur_pos) {
	if (cur_pos < 0 || cur_pos >= vehicle_nodes_.size()-1) {
		throw MyException("Wrong position.");
	}
	ISolutionNode& next_node = *nodes_->get_node(vehicle_nodes_.at(cur_pos + 1));
	ISolutionNode& cur_node = *nodes_->get_node(vehicle_nodes_.at(cur_pos));
	int next_node_index = vehicle_nodes_.at(cur_pos + 1);
	int cur_node_index = vehicle_nodes_.at(cur_pos);
	// �����뿪ʱ��
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