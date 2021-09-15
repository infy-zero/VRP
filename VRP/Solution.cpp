#include "FerryTaskSetting.h"
#include "Solution.h"
#include "MyException.h"

#include <algorithm>
#include <vector>


Solution::Solution(int depot, AllNodes* nodes):depot_(depot),all_nodes_(nodes) {
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

bool Solution::judge_not_exceed_max_length() {
	for (IVehicle vehicle : vehicles) {
		if (vehicle.beyond_max_length()) {
			if (vehicle.size() <= 3) {
				throw MyException("The task's length is more than max length of a vehicle.");
			}
			cout << "��ǰ�ⳬ�����ȣ�max=" << vehicle.vehicle_max_length << "��cur=" << vehicle.cal_length() << endl;
			return false;
		}
	}
	return true;
}

// TODO(Lvning): ��ǰֻ�ܹ��жϵ��г����нڵ�����硢����ʱ�䣬���Ҫ������г̣���Ҫ�����жϷ���
bool Solution::judge_sequence_feasible() {
	//cout << to_string_without_update() << endl;
	
	// �����нڵ��ʼ��
	for (auto node_iter : all_nodes_->nodes_) { // ���нڵ���Ϣ����
		node_iter.second.reset_infeasible();
	}
	for (auto depot_iter : all_nodes_->depots_) { // ���г�վ��Ϣ��ʼ��
		depot_iter.second.reset_feasible();
	}

	// ��ʼ���м���
	vector<int> vehicle_index(vehicles.size());
	auto& virtual_flight_consequence = inf consequence;
	vector<int> flight_index(virtual_flight_consequence.size());

	// TODO(Lvning):���Ǳ�����Ӧת������Ȼmap��vector�Ǳ��һ�£�>0��������Ϊ�˷����������mapΪ��׼
	// ����unordered_mapʵ�֣�����Ϊ<�ڵ�Ǳ꣬[�����, ���ڽǱ�, �鳤��]>
	// ��vehicles�еĽڵ�Ϊ��Ҫ�ο�Ŀ�꣬����flights�еĽڵ㣬ֻ���Ǻ�vehicles���ص����ֵ�˳��
	unordered_map<int, NodeLocation> vehicle_front;
	for (int i = 0; i < vehicles.size(); i++) {
		if (vehicles.at(i).is_empty()) {
			continue;
		}
		NodeLocation int_tmp{ i, 1, vehicles.at(i).size() - 1 };
		vehicle_front.insert(make_pair(vehicles.at(i).at(1), int_tmp));
	}

	// ����ǰ�ؽڵ�
	unordered_map<int, NodeLocation> flight_front;
	for (int i = 0; i < virtual_flight_consequence.size(); i++) {
		NodeLocation int_tmp{ i, 0, virtual_flight_consequence.at(i).size() - 1 };
		flight_front.insert(make_pair(virtual_flight_consequence.at(i).at(0), int_tmp));
	}

	// ͳ�Ƶ�ǰ��������Ľڵ�������ͱ�ţ������ڵ�״̬��ΪUNCHECKED_NODE
	unordered_set<int> node_in_queue;
	for (IVehicle& veh : vehicles) { // ��������������ڵ����
		for (int node_index : veh.vehicle_nodes) {
			if (all_nodes_->get(node_index)->type_ == TYPE_NODE) {
				node_in_queue.insert(node_index);
			}
		}
	}
	// ���º���ǰ�ص���Ϣ����ֹ��ʼ�ڵ㿨����---TODO(Lvning): ����Ӧ�÷�װΪ��������
	for (auto flight_iter = flight_front.begin(); flight_iter != flight_front.end();) {
		if (node_in_queue.count(flight_iter->first)) { // �������ֱ������
			flight_iter++;
			continue;
		}

		skip_flight_node_not_in_queue(flight_iter, virtual_flight_consequence, &node_in_queue, &flight_front);
		flight_iter = flight_front.erase(flight_front.find(flight_iter->first));
	}
	/*
	cout << "in_queue nodes = ";
	for (int index : node_in_queue)
	{
		cout << index << ",";
	}
	cout << endl;
	*/

	while (node_in_queue.size() > 0) {
		int node_in_queue_size = node_in_queue.size();
		//cout << to_string_without_update() << endl;
		/*
		cout << "vehicle front = " << endl;
		for (auto index : vehicle_front)
		{
			cout << "index=" << index.first << ",veh_pos=" << index.second.group_index << ",node_pos=" << index.second.cur_pos <<
				",max_pos=" << index.second.max_pos << endl;
		}
		cout << endl;

		cout << "flight front = " << endl;
		for (auto index : flight_front)
		{
			cout << "index=" << index.first << ",veh_pos=" << index.second.group_index << ",node_pos=" << index.second.cur_pos <<
				",max_pos=" << index.second.max_pos << endl;
		}
		cout << endl;
		*/

		for (auto vehicle_iter = vehicle_front.begin(); vehicle_iter != vehicle_front.end();) {
			int cur_index = vehicle_iter->first;
			unordered_map<int, NodeLocation>::iterator flight_iter = flight_front.find(cur_index);
			if (flight_iter != flight_front.end()) { // �ҵ����ܹ�ƥ��Ľڵ�
			  // ���½ڵ�ʱ�䣬ʧ����ֱ�ӷ���
				if ( !update_node_timewindow(vehicle_iter->second, flight_iter->second) ) {
					return false;
				}


			  // ����vehicle_front��flight_front��node_in_queue������
				
				// cout << "find = " << flight_iter->first << endl;
				node_in_queue.erase(vehicle_iter->first);

				// TODO(Lvning): ��������������һ��һ������
				// �����µĳ���������Ϣ
				auto vehicle_new_tmp = vehicle_iter->second;
				if (vehicle_new_tmp.cur_pos < vehicle_new_tmp.max_pos - 1) {
					int vehicle_new_index = vehicles.at(vehicle_new_tmp.vehicle_index).at(vehicle_new_tmp.cur_pos + 1);
					vehicle_new_tmp.cur_pos++;
					vehicle_front.insert(make_pair(vehicle_new_index, vehicle_new_tmp));
				}
				vehicle_iter = vehicle_front.erase(vehicle_front.find(vehicle_iter->first));

				// �����µķɻ�������Ϣ
				skip_flight_node_not_in_queue(flight_iter, virtual_flight_consequence, &node_in_queue, &flight_front);
				flight_front.erase(flight_front.find(flight_iter->first));
			}
			else {
				vehicle_iter++;
			}
		}
		if (node_in_queue_size == node_in_queue.size()) { // ���ֱ�����û�ҵ����򷵻�false
			if (has_empty_vehicle()) {
				//cout << "node_in_queue = " << node_in_queue.size() << endl;
			}
			//cout << solution->to_string_without_update() << endl;
			//cout << "Do not have feasible location. empty = " << solution->has_empty_vehicle() << endl;
			return false;
		}
	}
	return true;
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
	IVehicle tmp_vehicle(depot_, all_nodes_);
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

// ��������node_in_queue�Ľڵ㣬��������Ӧ��ʱ��
void Solution::skip_flight_node_not_in_queue(const unordered_map<int, NodeLocation>::iterator& flight_iter,
											 vector<vector<int>>& virtual_flight_consequence,
											 unordered_set<int>* node_in_queue,
											 unordered_map<int, NodeLocation>* flight_front) {

	const NodeLocation& flight_old_tmp = flight_iter->second;
	vector<int>& flight_order = virtual_flight_consequence.at(flight_old_tmp.vehicle_index);
	if (flight_old_tmp.max_pos != flight_order.size() - 1) { // ���������ɻ������Ƿ���ͬ
		throw MyException("These two sequence of flight are not same.");
	}
	int flight_new_pos = flight_old_tmp.cur_pos + 1;
	while (flight_new_pos <= flight_old_tmp.max_pos &&
		node_in_queue->find(flight_order.at(flight_new_pos)) == node_in_queue->end()) { // �������ڵ�ǰ���е�����
		// ����ʱ�䴰
		int node_index = flight_order.at(flight_new_pos);
		ISolutionNode& node = *(all_nodes_->get(node_index));
		//ISolutionNode&;
		// ���ɱ�ڵ㣬�����д���
		if (node.task->type == FVTType::UNVARIABLE_FLIGHT) {
			node.service_earliest_start_time = node.task->predefined_earliest_arrival_time;
			node.arrive_earliest_time = node.service_earliest_start_time - node.task->predefined_service_max_before_;
			node.service_latest_end_time = node.task->predefined_service_latest_start_time;
		}
		if (node.task->type == FVTType::VARIABLE_FLIGHT) { // �ɱ�ڵ� ���� һ�����ǵ�һ������
			int pre_flight_node_index = flight_order.at(flight_new_pos - 1);
			ISolutionNode& pre_flight_node = *(all_nodes_->get(pre_flight_node_index));
			// ��ǰ����ڵ�ʱ�䴰 ���� ��һ�ɻ��ڵ����ʱ
			double flight_earliest_service_start_time = pre_flight_node.task->predefined_earliest_service_start_time + fts FTS_max_delta_time;
			double filght_earliest_arrive_time = pre_flight_node.task->predefined_earliest_arrival_time + fts FTS_max_delta_time;
			double flight_latest_service_start_time = pre_flight_node.task->predefined_service_latest_start_time + fts FTS_max_delta_time;

			// ��ǰ�����ڵ�ʱ�䴰 ���� ��һ�����ڵ������ʱ�� + �ϳ�ʱ�� + �³�ʱ��
			double vehicle_earliest_service_start_time = pre_flight_node.task->predefined_earliest_service_start_time + fts FTS_max_delta_time;
			double vehicle_earliest_arrive_time = pre_flight_node.task->predefined_earliest_arrival_time + fts FTS_max_delta_time;
			double vehicle_latest_service_start_time = pre_flight_node.task->predefined_service_latest_start_time + fts FTS_max_delta_time;

			// ��ǰ�����ʱ�䴰������
			double kEarliest_service_start_time = node.task->predefined_earliest_service_start_time;
			double kEarliest_arrive_time = node.task->predefined_earliest_arrival_time;
			double kLatest_service_start_time = node.task->predefined_service_latest_start_time;

			// ���µ�ǰ�ڵ�ʱ�䴰
			if ( !cal_timewindow_intersection(flight_earliest_service_start_time, flight_latest_service_start_time, vehicle_earliest_service_start_time, vehicle_latest_service_start_time, &(node.service_earliest_start_time), &(node.service_latest_end_time))) {
				cout << "[" << flight_earliest_service_start_time << "," << flight_latest_service_start_time << "]" << endl;
				cout << "[" << kEarliest_service_start_time << "," << kLatest_service_start_time << "]" << endl;
				throw MyException("The intersection of flight and task can not be less than flight.");
			}

			// �ж��Ƿ��������ʱ�䴰Ҫ��
			if (flight_earliest_service_start_time < kEarliest_service_start_time || flight_latest_service_start_time > kLatest_service_start_time) {
				cout << "[" << flight_earliest_service_start_time << "," << flight_latest_service_start_time << "]" << endl;
				cout << "[" << kEarliest_service_start_time << "," << kLatest_service_start_time << "]" << endl;
				throw MyException("The intersection of flight and task can not be less than flight.");
			}
		}	

		flight_new_pos++;
	}
	if (flight_new_pos <= flight_old_tmp.max_pos) { // �����µķɻ�������Ϣ
		int flight_new_index = flight_order.at(flight_new_pos);
		NodeLocation flight_new_tmp = { flight_old_tmp.vehicle_index, flight_new_pos, flight_old_tmp.max_pos };
		flight_front->insert(make_pair(flight_new_index, flight_new_tmp));
	}
	// flight_front->erase(flight_front->find(flight_iter->first));
}

// �÷���ֻ�����ڵ��г�vrp
bool Solution::update_node_timewindow(const NodeLocation& vehicle_location, const NodeLocation& flight_location) {
	int node_index = vehicles.at(vehicle_location.vehicle_index).at(vehicle_location.cur_pos); // ��ȡ����
	if (all_nodes_->get_node_type(node_index) != NodeType::TYPE_NODE) {
		throw MyException("This fuction can only deal with node type.");
	}
	if (node_index != inf consequence.at(flight_location.vehicle_index).at(flight_location.cur_pos)) {
		throw MyException("The information offered can not match, need to check.");
	}

	// TODO(Lvning): get_travel_time()�У����ӷ���ʱ��
	ISolutionNode& node     = all_nodes_->nodes_.at(node_index);	// ��ȡ��ǰ�ڵ�
	int pre_node_index		= vehicles.at(vehicle_location.vehicle_index).at(vehicle_location.cur_pos-1);// ��ȡǰһ�����ڵ����
	ISolutionNode& pre_node = all_nodes_->nodes_.at(node_index);    // ��ȡǰһ�����ڵ�
	// ����ǰ�ڵ�Ϊʱ�䴰���ɱ�ڵ㣨���ǰ����flight�����depot��ʱ��ֻ��Ҫ�ȼ���vehicle�����ʱ�䴰��Ȼ���flight����ʱ�䴰ȡ��������
	if (node.task->type == FVTType::UNVARIABLE_FLIGHT) {		
		// ǰһ�����ڵ�ʱ��
		double pre_earliest_service_start_time = pre_node.service_earliest_start_time;
		double pre_earliest_arrive_time		   = pre_node.arrive_earliest_time;
		double pre_latest_service_start_time   = pre_node.service_latest_end_time;

		// ����ǰһ�����ڵ�ʱ��������õĵ�ǰ�ڵ�ʱ��
		double travel_time = inf cal_travel_time(node_index, pre_node_index);
		double node_earliest_service_start_time = pre_earliest_service_start_time + travel_time;
		double node_earliest_arrive_time        = pre_node.arrive_earliest_time + travel_time;
		double node_latest_service_start_time   = pre_node.service_latest_end_time + travel_time;

		// ��ǰ����ڵ�ʱ�䴰
		double flight_earliest_service_start_time = node.task->predefined_earliest_service_start_time;
		double filght_earliest_arrive_time		  = node.task->predefined_earliest_arrival_time;
		double flight_latest_service_start_time = node.task->predefined_service_latest_start_time;

		if (!cal_timewindow_intersection(node_earliest_service_start_time, node_latest_service_start_time, flight_earliest_service_start_time, flight_latest_service_start_time, &(node.service_earliest_start_time), &(node.service_latest_end_time))) {
			return false;
		} else{
			return true;
		}

	}
	// ����ǰ�ڵ�Ϊʱ�䴰�ɱ�ڵ�ʱ��ֻ��Ҫ�ȼ���vehicle�����ʱ�䴰��Ȼ�����flight����ʱ�䴰������flight����ʱ�䴰ȡ��������
	// vehicle����϶���ǰ�ýڵ㣬flight����Ҫ�ֱ��жϣ��Լ����ݵ�ǰ�ڵ��Ƿ�ɱ�ʱ�䴰���з���
	if (node.task->type == FVTType::VARIABLE_FLIGHT) {
		int pre_flight_node_index = vehicles.at(flight_location.vehicle_index).at(flight_location.cur_pos - 1);// ��ȡǰһ�ɻ��ڵ����
		ISolutionNode& pre_flight_node = all_nodes_->nodes_.at(pre_flight_node_index);    // ��ȡǰһ�����ڵ�
		// ǰһ�����ڵ�ʱ��
		double pre_earliest_service_start_time = pre_node.service_earliest_start_time;
		double pre_earliest_arrive_time = pre_node.arrive_earliest_time;
		double pre_latest_service_start_time = pre_node.service_latest_end_time;

		// ����ǰһ�����ڵ�ʱ��������õĵ�ǰ�ڵ�ʱ��
		double travel_time = inf cal_travel_time(node_index, pre_node_index);
		double node_earliest_service_start_time = pre_earliest_service_start_time + travel_time;
		double node_earliest_arrive_time = pre_node.arrive_earliest_time + travel_time;
		double node_latest_service_start_time = pre_node.service_latest_end_time + travel_time;

		// ��ǰ����ڵ�ʱ�䴰������һ�ɻ��ڵ����ʱ
		double flight_earliest_service_start_time = pre_flight_node.task->predefined_earliest_service_start_time + fts FTS_max_delta_time;
		double filght_earliest_arrive_time = pre_flight_node.task->predefined_earliest_arrival_time + fts FTS_max_delta_time;
		double flight_latest_service_start_time = pre_flight_node.task->predefined_service_latest_start_time + fts FTS_max_delta_time;

		if (!cal_timewindow_intersection(node_earliest_service_start_time, node_latest_service_start_time, flight_earliest_service_start_time, flight_latest_service_start_time, &node.service_earliest_start_time, &node.service_latest_end_time)) {
			return false;
		}
		else {
			return true;
		}

	}
	return true;
}

bool Solution::cal_timewindow_intersection(int vehicle_start, int vehicle_end, int flight_start, int flight_end, double* inter_start, double* inter_end) {
	if (vehicle_end < flight_start) {
		return false;
	}
	if (flight_end < vehicle_start) {
		return false;
	}
	*inter_start = max(vehicle_start, flight_start);
	*inter_end = min(vehicle_end, flight_end);
	return true;
}

// ���ɿ��н��ַ�����push_solution_spaceר��
string Solution::toString_with_update() {
	// ɾ���ճ�
	for (int i = vehicles.size() - 1; i >= 0; i--) {
		if (vehicles.at(i).size() < 2) {
			throw new exception("�����ڵ�С��2");
		}
		else if (vehicles.at(i).size() == 2) {
			vehicles.erase(vehicles.begin()+i);
		}
	}
	// �Ƚ������򣨰���ÿ�����ڶ����ڵ����ţ� TODO(Lvning): �����������
	sort(vehicles.begin(), vehicles.end(), [](IVehicle& v1, IVehicle& v2)->int { return v1.at(1) - v2.at(1); });
	// ������������stringstream��Ȼ���ٵ���
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