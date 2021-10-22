#include "ALNS.h"

#include <algorithm>
#include <ctime>
#include <memory>
#include <queue>
#include <unordered_set>

#include "ALNS/Util/Util.h"
#include "ALNS_Setting.h"
#include "MyException.h"

/* �˴�������¼һЩ��˼·
* 1����ǿ����ԣ��������һ���б���/�����б��еĽڵ㲻�������Ӽ���
* 2����������ԣ����������Ӻ�ִ��һ�ε����ӳ��׷��룬������ǣ����Ϊ�������������Ԥ���Ľ��ռ�
* 3��// �����ڵ���ڵ���ʱ�䲻�����ƣ�Ŀǰ����������޵ȴ� TODO(Lvning): ����ͣ��λ�ã�Ȼ���Ϊһ���ֶκ���
	vehicle_service_latest_end_time = DBL_MAX;��current��
*/

ALNS::ALNS()
{
	cout << "���� ALNS" << endl;
	// ȷ���ڶɳ����񣨳����������ޣ���������������Ϊ����������������
	total_vehicle_num = inf nodes.get_size(0b001);
	// ���ɳ�ʼ�⣬ע��information
	nodes_ = &inf nodes;
	
	// ��վ����
	if (A_S range == EAST) {
		depot_ = -2;
	}
	else {
		depot_ = -1;
	}
	// ���ɳ�ʼ��
	if (curSol == nullptr) {
		curSol.reset(new Solution(depot_, *nodes_));
	}
	else {
		throw MyException("The current solution exists.");
	}
	// ����������ѹ��RemovedList TODO(Lvning): �����Ǳ�
	for (int i = 0; i < inf nodes_indexs.at(0b001).size(); i++) {
		curSol->removedList.push_back(inf nodes_indexs.at(0b001).at(i));
	}

	// ��ʼ������
	// ��ʼ����������
	initialize_insertion();
	initialize_destroy();
	cout << "ALNS����" << endl;
	
}

void ALNS::start()
{
	/*����Ӧ�����������ڴ����������Ļ����������˸�����ƻ��ͻָ����ӣ�ͬʱΪ��Щ���Ӹ���Ȩ�غ�������һ����Ӧ�㣬����Ӧ������Ҫ���Ƶ��������±���r��segment number �������ڣ���ѡ����׼�����ӼƷַ���*/
	clock_t clock_time = clock();
	time_t now = time(0);	// ��ȡ��ǰʱ��
	tm now_tm;
	localtime_s(&now_tm, &now);
	string now_time = to_string(now_tm.tm_mon)+'-'+ to_string(now_tm.tm_mday) +'_'+ to_string(now_tm.tm_hour) +'-'+ to_string(now_tm.tm_min) +'-'+ to_string(now_tm.tm_sec);   // ��ǰʱ���ַ���
	string filename = A_S output_dir + "ALNS_result_" + now_time + "_MC" + to_string(nodes_->get_size(0b001)) + "_decay" + to_string(A_S decay) + "_decaytimes" + to_string(A_S decayTimes) + "_upt" + to_string(A_S segment) + ".txt";
	fopen_s(&file_, filename.c_str(), "w");
	output(file_, "    ��ʼALNS�㷨\n" );

	/*���ɳ�ʼ�⡪�����ûָ�����+Լ������*/
	output(file_, "�ڵ��� = " + to_string(curSol->removedList.size()));
	output(file_, "		���ɳ�ʼ��\n");
	select_insertion(curSol);
	output(file_, "��ʼ���������\n");
	curSol->update();													// ���㵱ǰĿ�꺯��ֵ
	push_solution_space(nullptr, curSol);								// �����ռ�
	output(file_, clock_time, BEST, curSol);

	// ����ʼ��д���ļ�
	string origin_output_string = to_string(nodes_->get_size(0b001)) + "," + now_time + "," + to_string((clock() - clock_time) / CLOCKS_PER_SEC) + "," + to_string(1) + "," + to_string(bestSol->cur_val_) + "," + to_string(bestSol->vehicles.size()) + "\n";

	/*��ʼ���������ﵽֹͣ��׼֮ǰ*/
	while (criterion.iter())
	{
		// �����½�
		shared_ptr<Solution> newSol(new Solution(*curSol));
		// �ƻ���ǰ��
		int selected_destroy_index = select_destroy(newSol);
		// �ָ���ǰ��
		int selected_repair_index = select_insertion(newSol);
		// ����newSol��Ϣ
		newSol->update();
		// �ж��Ƿ�����½�
		SolutionFrequency sf = push_solution_space(&criterion, newSol);

		// ���µ�ǰ��ķ���
		updateScores(selected_repair_index, selected_destroy_index, sf);
		// ���� Adaptive layer ����
		if (criterion.isUpdateParamter()) {
			updateParameters(criterion.max_segment);
		}
		if (criterion.curTimes % A_S PRINT_FRQ == 0) {
			output(file_, clock_time, sf, newSol);
		}
	}
	output(file_, "    �������������Ƶ���������" + to_string(criterion.getTotalTimes())+"\n");
	output(file_, clock_time, BEST, bestSol);
	fclose(file_);
	// ��������csv(����+����ʱ��+����ʱ��+��������+���ź���ֵ+������)
	FILE* file_csv;
	fopen_s(&file_csv, (A_S output_dir + "output_MC" + to_string(nodes_->get_size(0b001)) + ".csv").c_str(), "a");
	now_time = to_string(now_tm.tm_mon) + '/' + to_string(now_tm.tm_mday) + ' ' + to_string(now_tm.tm_hour) + ':' + to_string(now_tm.tm_min) + ':' + to_string(now_tm.tm_sec);   // ��ǰʱ���ַ���
	string output_string = to_string(nodes_->get_size(0b001)) + "," + now_time + "," + to_string((clock() - clock_time) / CLOCKS_PER_SEC) + "," + to_string(criterion.getTotalTimes()) + "," + to_string(bestSol->cur_val_) +"," + to_string(bestSol->vehicles.size()) + "\n";
	fprintf(file_csv, origin_output_string.c_str());
	fprintf(file_csv, output_string.c_str());
	fclose(file_csv);
}
// �ָ�����
// ��ʼ���������� TODO(Lvning):�Ż��ṹ�����ӿ�ά����
void ALNS::initialize_insertion() {
	// Ŀǰֻ���������ӡ���̰�����롢����̰�������ֵ����
	repair_grades = { A_S greedy_insertion_weight, A_S single_greedy_insertion_weight, A_S regret_insertion_weight };
	initialize_insertion_add_and_count();
}
void ALNS::initialize_insertion_add_and_count() {
	repair_add = { 0.0, 0.0, 0.0 };
	repair_count = { 0, 0, 0 };
}
// ���̶�ѡ������
int ALNS::select_insertion(shared_ptr<Solution>& solution) {
	if (solution == nullptr) {											// solutionΪ�գ�Ĭ������curSol
		throw MyException("You can check non-exist solution.");
	}
	if (solution->vehicles.size() == 0) {
		solution->add_empty_vehicle();
	}
	int select_index = Util::roulette(repair_grades);
	switch (select_index) {
	case 0:
		greedy_insertion(solution);
		break;
	case 1:
		single_greedy_insertion(solution);
		break;
	case 2:
		regret_insertion(solution); 
		break;
	default:
		throw MyException("Not a feasible index.");
	}
	return select_index;
}
// TODO(Lvning): ����is_feasible�����Ƹ�����
void ALNS::mix_insertion(shared_ptr<Solution>& solution) {

}
// ���ֵ����removedList�е����нڵ㣬��֤��ʱ�пճ�����һ�������źʹ��ſ�����һ���ڶ��⳵���У�
void ALNS::regret_insertion(shared_ptr<Solution>& solution) {
	output(file_, "���ֵ����\n");
	while (!solution->removedList.empty()) { // ֱ��removedList���
		// ȷ������ֻ��һ���ճ�
		solution->satisfy_condition();
		int node_left = solution->removedList.size();
		regret_insertion_once(solution);
		if (node_left == solution->removedList.size()) { // �ڵ�û�з����任��˵��û�ܲ���ڵ㣬����
			solution->make_single_empty_vehicle();
		}
	}
}
// �������һ���ڵ�
void ALNS::regret_insertion_once(shared_ptr<Solution>& solution) {
	if (solution->removedList.empty()) {								// ��removedListΪ�գ���ֹ����
		throw MyException("The removedList is empty, no need for insertion!");
	}
	unique_ptr<InsertDescription> global_best_operator_description;		// ȫ�����Ų���
	double global_delta = -A_S dbl_max_value;

	// �����һ���ڵ���ǰ����������ɾ��ʱ�临�Ӷ�
	for (int i = 0; i < solution->removedList.size(); i++) {
		// ��������λ�ã�ȷ���Ƿ��ܲ���
		// ÿ�α�������ֵ��delta��min��2nd��solution����������ֵ�ĺ�����ʼ��Ϊ����
		unique_ptr<InsertDescription> local_best_operator_description;    // ��ǰ�ֲ����Ų���
		priority_queue<double, vector<double>, greater<double>> best_two_values;// ��С�������Ŀ�꺯��ֵ(С����)

		for (int j = 0; j < solution->vehicles.size(); j++) {				// �������г�
			for (int k = 1; k < solution->vehicles.at(j)->get_total_size(); k++) {		// ������������λ��
				// TODO(Lvning): ����������дΪһ�����������ս�ֻ����������Ϣ������������
				unique_ptr<InsertDescription> current_operator_description(new InsertDescription(INSERT_NODE, j, k, solution->removedList.at(i), i));									  // ��ǰ����
				
				bool operator_is_feasible = solution->check_operator_feasible(current_operator_description);
				//cout << operator_is_feasible << "," << *current_operator_description;
				if (operator_is_feasible) {
					// ά��С����
					best_two_values.push(current_operator_description->save_value_);
					while (best_two_values.size() > 2) {
						best_two_values.pop();
					}
					if (current_operator_description->is_better(local_best_operator_description)) {// ��ǰ������Ҹ���������滻
						local_best_operator_description = move(current_operator_description);
					}
					
				}
			}
		}
		// ѭ���������ж��Ƿ���Ҫ����ȫ�����Ž⡣�����������ԼֵΪ����
		double local_delta;
		if (best_two_values.size() == 1) {
			local_delta = best_two_values.top()+ A_S dbl_max_value;
		} else if (best_two_values.size() == 2) {
			double second_small_val = best_two_values.top();
			best_two_values.pop();
			double first_small_val = best_two_values.top();
			local_delta = abs(second_small_val - first_small_val);
			//cout << first_small_val << "," << second_small_val << endl;
		} else {
			continue;
		}
		if (local_delta >= global_delta) {
			global_best_operator_description = move(local_best_operator_description);
			global_delta = local_delta;
		}
	}
	// �����³�
	if (global_best_operator_description != nullptr) {
		solution->excute_insert_operator(global_best_operator_description);
	}
}

// ̰������removedList�е����нڵ�
void ALNS::greedy_insertion(shared_ptr<Solution>& solution) {
	output(file_, "̰������\n");
	while (!solution->removedList.empty()) { // ֱ��removedList���
		// cout << "��ǰʣ��ڵ��� = " << solution->removedList.size() << endl;
		int node_left = solution->removedList.size();
		solution->satisfy_condition();
		//cout << solution->to_string_without_update() << endl;
		greedy_insertion_once(solution);
		if (node_left == solution->removedList.size()) { // �ڵ�û�з����任������������
			solution->add_empty_vehicle();
		}
	}
}
// ����һ���ڵ�
void ALNS::greedy_insertion_once(shared_ptr<Solution>& solution) {
	if (solution->removedList.empty()) {								// ��removedListΪ�գ���ֹ����
		throw MyException("The removedList is empty, no need for insertion!");
	}
	unique_ptr<InsertDescription> global_best_operator_description;		// ȫ�����Ų���
	bool tag = true;
	//cout << "insert once" << endl;
	// �����һ���ڵ���ǰ����������ɾ��ʱ�临�Ӷ�
	for (int i = solution->removedList.size()-1; i>=0; i--) {
		// ��������λ�ã�ȷ���Ƿ��ܲ���
		for (int j = 0; j < solution->vehicles.size(); j++) {				// �������г�
			for (int k = 1; k < solution->vehicles.at(j)->get_total_size(); k++) {		// ������������λ��
				unique_ptr<InsertDescription> current_operator_description(new InsertDescription(INSERT_NODE, j, k, solution->removedList.at(i), i));			// ��ǰ����
				
				bool operator_is_feasible = solution->check_operator_feasible(current_operator_description);
				
				if (operator_is_feasible && current_operator_description->is_better(global_best_operator_description)) {			// ��ǰ������Ҹ���������滻
					global_best_operator_description = move(current_operator_description);
				} else {
					//cout << "isfeasible="<< is_feasible << ", local_exitsts=" << (localSol!=nullptr) <<endl;
				}
	
			}
		}
	}
	// ����ʧ��ʱ�������³��������һ���ڵ�����³�
	if (global_best_operator_description != nullptr) {// ���򽫵�ǰ�������Ϊ�ֲ����Ž�
		solution->excute_insert_operator(global_best_operator_description);
	}
}
// ����̰�����루ÿ��ֻ����һ���ڵ��̰�����룬������ڵ����ѡ��
void ALNS::single_greedy_insertion(shared_ptr<Solution>& solution) {
	output(file_, "����̰������\n");
	while (!solution->removedList.empty()) { // ֱ��removedList���
		solution->satisfy_condition();
		int node_left = solution->removedList.size();
		single_greedy_insertion_once(solution);
		if (node_left == solution->removedList.size()) { // ���нڵ㲻�ܲ���ʱ�������³�
			solution->make_single_empty_vehicle();
		}
	}
	solution->update();
}
void ALNS::single_greedy_insertion_once(shared_ptr<Solution>& solution) {
	if (solution->removedList.empty()) {								// ��removedListΪ�գ���ֹ����
		throw MyException("The removedList is empty, no need for insertion!");
	}
	unique_ptr<InsertDescription> global_best_operator_description;		// ȫ�����Ų���
	int left_node_size = solution->removedList.size();
	
	// �����һ���ڵ���ǰ����������ɾ��ʱ�临�Ӷ�
	int selected_node_pos = left_node_size * Util::getRandom();
	// ��������λ�ã�ȷ���Ƿ��ܲ���
	for (int j = 0; j < solution->vehicles.size(); j++) {				// �������г�
		for (int k = 1; k < solution->vehicles.at(j)->get_total_size(); k++) {		// ������������λ��
			unique_ptr<InsertDescription> current_operator_description(new InsertDescription(INSERT_NODE, j, k, solution->removedList.at(selected_node_pos), selected_node_pos));			// ���ɵ�ǰ��Ŀ���
			
			bool operator_is_feasible = solution->check_operator_feasible(current_operator_description);
			//cout << is_feasible << endl;
			if (operator_is_feasible && current_operator_description->is_better(global_best_operator_description)) {			// ��ǰ������Ҹ���������滻
				global_best_operator_description = move(current_operator_description);
			}
			else {
			}
		}
	}
	if (global_best_operator_description != nullptr) {
		solution->excute_insert_operator(global_best_operator_description);
	}
}

/*�ƻ�����*/ 
void ALNS::initialize_destroy() {
	// Ŀǰֻ���������ӡ������ɾ����̰��ɾ��
	destroy_grades = { A_S random_removal_weight, A_S greedy_removal_weight };
	initialize_destroy_add_and_count();
}
void ALNS::initialize_destroy_add_and_count() {
	destroy_add = { 0.0, 0.0 };
	destroy_count = { 0, 0 };
}
// ���̶�ѡ������
int ALNS::select_destroy(shared_ptr<Solution>& solution) {
	if (solution == nullptr) {
		throw MyException("You can check non-exist solution.");
	}
	int select_index = Util::roulette(destroy_grades);
	switch (select_index) {
	case 0:
		random_destroy(solution);
		break;
	case 1:
		greedy_destroy(solution);
		break;
	default:
		throw MyException("Not a feasible index.");
	}
	return select_index;
}
// ���ɾ�����ԣ����������ӻ��ʹ�ã���������Ӧ��  TODO(Lvning): ����is_feasible�����Ƹ�����
void ALNS::mix_destory(shared_ptr<Solution>& solution) {

}

// ���ɾ�� 4-40% ���ڵ㣬���ڽڵ�֮����������������ԣ� 
void ALNS::random_destroy(shared_ptr<Solution>& solution) {
	output(file_, "���ɾ��\n");
	if (solution->all_nodes_.get_size(0b001) == solution->removedList.size()) { // û�нڵ����ɾ��
		throw MyException("You can not delete from an empty solution.");
	}
	int rest_node_num = solution->all_nodes_.get_size(0b001) - solution->removedList.size();
	int delete_node_num;
	if (rest_node_num > 10) {
		delete_node_num = static_cast<int> (Util::getRandom() * 0.4 * rest_node_num);
	} else {
		delete_node_num = static_cast<int> (Util::getRandom() * rest_node_num);
	}
	delete_node_num = max(delete_node_num, 1);	// ����ɾ��һ��
	while (delete_node_num--) {
		random_destroy_once(solution);
	}
	// TODO(Lvning): ά�����б�����ͳһ�ĵط�
	output(file_, "��ǰ������ʣ��ڵ��� = " + to_string(solution->all_nodes_.get_size(0b001) - solution->removedList.size()) + "���ڵ����� = " + to_string(solution->all_nodes_.get_size(0b001)) + "\n");
	solution->update_after_delete();
}

// ���ɾ��1���ڵ�
void ALNS::random_destroy_once(shared_ptr<Solution>& solution) {
	if (solution->removedList.size() == solution->all_nodes_.get_size(0b001)) {
		throw MyException("Can't delete from an empty solution.");
	}
	// ���ɾ��һ���ڵ� TODO(Lvning): д��һ������
	int total_node_num = solution->get_total_node_num();
	int selected_node_num = total_node_num * Util::getRandom()+1;
	int selected_num = selected_node_num;
	int vehicle_index = -1;
	int selected_node_pos = -1;
	for (vehicle_index = 0; vehicle_index < solution->vehicles.size(); vehicle_index++) {
		if (selected_node_num > solution->vehicles.at(vehicle_index)->get_node_size()) {
			selected_node_num -= solution->vehicles.at(vehicle_index)->get_node_size();
			continue;
		}
		for (int node_pos = 0; node_pos < solution->vehicles.at(vehicle_index)->get_total_size(); node_pos++) {
			int node_index = solution->vehicles.at(vehicle_index)->get_node_index(node_pos);
			if (solution->all_nodes_.get_node_type(node_index) == TYPE_NODE) {
				selected_node_num--;
			}
			if (selected_node_num == 0) {
				selected_node_pos = node_pos;
				break;
			}
		}
		if (selected_node_num == 0) {
			break;
		}
	}
	int node_index = solution->vehicles.at(vehicle_index)->get_node_index(selected_node_pos);
	if (solution->all_nodes_.get_node_type(node_index) != TYPE_NODE) {
		throw MyException("You can't delete node which type is not node.");
	}
	solution->vehicles.at(vehicle_index)->delete_node(selected_node_pos, solution->removedList);
}
// ̰���ƽ⻵����
void ALNS::greedy_destroy(shared_ptr<Solution>& solution) {
	output(file_, "̰��ɾ��\n");
	if (solution->all_nodes_.get_size(0b001) == solution->removedList.size()) { // û�нڵ����ɾ��
		throw MyException("You can not delete from an empty solution.");
	}
	int rest_node_num = solution->all_nodes_.get_size(0b001) - solution->removedList.size();
	int delete_node_num;
	if (rest_node_num > 10) {
		delete_node_num = static_cast<int> (Util::getRandom() * 0.4 * rest_node_num);
	}
	else {
		delete_node_num = static_cast<int> (Util::getRandom() * rest_node_num);
	}
	delete_node_num = max(delete_node_num, 1);	// ����ɾ��һ��
	while (delete_node_num--) {
		greedy_destroy_once(solution);
	}
	// TODO(Lvning): ά�����б�����ͳһ�ĵط�
	output(file_, "��ǰ������ʣ��ڵ��� = " + to_string(solution->all_nodes_.get_size(0b001) -solution->removedList.size()) + "���ڵ����� = " + to_string(solution->all_nodes_.get_size(0b001)) + "\n");
	solution->update_after_delete();
}
// һ��̰���ƽ⻵���ӣ�ɾ��һ���ڵ㣬��ȡĿ�꺯��ֵ��͵��Ǹ�������������feasible����
void ALNS::greedy_destroy_once(shared_ptr<Solution>& solution) {
	if (solution->removedList.size() == solution->all_nodes_.get_size(0b001)) {
		throw MyException("Can't delete from an empty solution.");
	}
	// �ҵ�ɾ��֮��Ľ����Ľڵ�
	unique_ptr<DeleteDescription> global_best_operator_description = nullptr;
	for (int i = 0; i < solution->vehicles.size(); i++) {	// �������г�
		for (int j = 0; j < solution->vehicles.at(i)->get_total_size(); j++) { // �������нڵ�
			int node_index = solution->vehicles.at(i)->get_node_index(j);
			if (solution->all_nodes_.get_node(node_index)->type_ == TYPE_NODE) {
				unique_ptr<DeleteDescription> current_operator_description(new DeleteDescription(DELETE_NODE, i, j, node_index));// ��ǰ����
				// �����Լֵ

				solution->cal_delete_save_value(current_operator_description);

				if (current_operator_description->is_better(global_best_operator_description)) {
					global_best_operator_description = move(current_operator_description);
				}
			} else {
				continue;
			}
		}
	}
	if (global_best_operator_description != nullptr) {
		solution->excute_insert_operator(global_best_operator_description);
	} else {
		throw MyException("Should delete a node.");
	}
}

// ����ǰ�������ռ�
SolutionFrequency ALNS::push_solution_space(ICriterion* criterion, shared_ptr<Solution>& solution) {
	// ����Ŀ�꺯��ֵ
	solution->update();
	// �����ַ���
	solution->solution_string_ = solution->toString_with_update();
	// ����nullptrʱ�����õ�ǰ��
	if (solution == nullptr) {
		throw MyException("You can check non-exist solution.");
	}
	// ��û���жϱ�׼ʱ����Ϊ��һ�Σ�ֱ�ӽ���
	double curT;
	if (criterion == nullptr) { // curT = inf -> p = 1
		solutionSpace.insert(make_pair(solution->solution_string_, solution->cur_val_));
		curSol = solution;
		bestSol = shared_ptr<Solution>(new Solution(*curSol));
		return SolutionFrequency::BEST;
	} else {
		curT = criterion->getCurT();
	}
	
	auto tmp = solutionSpace.find(solution->solution_string_);
	if (solution->isBetter(bestSol)) { // �ж��Ƿ�Ϊ���Ž⣬�����Ž�+1.5
		solutionSpace.insert(make_pair(solution->solution_string_, solution->cur_val_));
		curSol = solution;
		bestSol = shared_ptr<Solution>(new Solution(*curSol));
		return BEST;
	} else if (tmp == solutionSpace.end() && solution->isBetter(curSol)) { // �ж��Ƿ�Ϊδ���ֹ��Ľ⣬�ȵ�ǰ�������+1.2
		solutionSpace.insert(make_pair(solution->solution_string_, solution->cur_val_));
		curSol = solution;
		return NEVER_BUT_BETTER;
	} else { // ģ���˻�
		if (simulated_annealing(curT, solution->cur_val_)) { // ���ܵ�ǰ��
			solutionSpace.insert(make_pair(solution->solution_string_, solution->cur_val_));
			curSol = solution;
			return SA_ACCEPT;
		} else { // �ܾ���ǰ��
			curSol = shared_ptr<Solution>(new Solution(*bestSol));
			return SA_REJECT;
		}
	}
	throw exception("You can not reach here!");
	return BEST;
}

// ģ���˻�׼��
bool ALNS::simulated_annealing(double curT, double curVal) {
	double possibility = exp( - (curVal - bestSol->cur_val_) / curT );	// ϵͳ������״̬�ĸ���
	double random = Util::getRandom();							// �������0��1��
	if (random < possibility) {
		return true;
	} else {
		return false;
	}
}

// ���²�����Ϣ
void ALNS::updateParameters(int times) {
	if (repair_grades.size() != repair_add.size()) {
		throw MyException("The size of grades and add are not equal.");
	}
	int total_repair_times = 0;
	for (int i = 0; i < repair_grades.size(); i++) {
		if (repair_count.at(i) == 0) {
			continue;
		}
		repair_grades.at(i) = repair_grades.at(i) * (1.0 - A_S update_ratio) + A_S update_ratio * repair_add.at(i) / static_cast<double>(repair_count.at(i));
		total_repair_times += repair_count.at(i);
	}
	int total_destroy_times = 0;
	for (int i = 0; i < destroy_grades.size(); i++) {
		if (destroy_count.at(i) == 0) {
			continue;
		}
		destroy_grades.at(i) = destroy_grades.at(i) * (1.0 - A_S update_ratio) + A_S update_ratio * destroy_add.at(i) / static_cast<double>(destroy_count.at(i));
		total_destroy_times += destroy_count.at(i);
	}
	if (total_repair_times != total_destroy_times || total_destroy_times != times) {			// ��������Ƿ����
		throw MyException("Total times are not equal.");
	}
	initialize_insertion_add_and_count();
	initialize_destroy_add_and_count();
}

// ���µ÷����
void ALNS::updateScores(int selected_repair_index, int selected_destroy_index, SolutionFrequency sf) {
	// TODO: ���µ�ǰ׷�ӵ÷�
	repair_add.at(selected_repair_index) += A_S get_grade_addition(sf);
	repair_count.at(selected_repair_index)++;
	destroy_add.at(selected_destroy_index) += A_S get_grade_addition(sf);
	destroy_count.at(selected_destroy_index)++;
}

void ALNS::output(FILE* file, time_t time, SolutionFrequency sf, shared_ptr<Solution>& solution, int type) {
	cout << "��ǰ��������" << criterion.getTotalTimes() << endl;
	fprintf(file, "��ǰ�������� = %d\n", criterion.getTotalTimes());
	output(file, "��ǰ��״̬ = " + A_S SolutionFrequency_to_string(sf) + "\n");

	output(file, "��ǰ����Ŀ�꺯��ֵ = " + to_string(solution->cur_val_) + "����ǰ������ = " + to_string(solution->vehicles.size()) + "����ǰ�� = " + solution->solution_string_ + "\n");

	output(file, "��ǰȫ��Ŀ�꺯��ֵ = " + to_string(curSol->cur_val_) + "����ǰ������ = " + to_string(curSol->vehicles.size()) + "����ǰ�� = " + curSol->solution_string_ + "\n");
	
	if (bestSol!=nullptr) {
		output(file, "��ǰȫ������ֵ = " + to_string(bestSol->cur_val_) + "����ǰ������ = " + to_string(bestSol->vehicles.size()) + "�����Ž� = " + bestSol->solution_string_ + "\n");
	}
	output(file, "��Ӧ��������£�\n");
	output(file, "�ָ����ӣ�̰�����롢����̰�������ֵ����\n");
	for (double repair_grade : repair_grades) {
		output(file, to_string(repair_grade) + ",");
	}
	output(file, "\n");
	
	output(file, "�ƻ����ӣ�����ƻ���̰���ƻ�\n");
	for (double destroy_grade : destroy_grades) {
		output(file, to_string(destroy_grade) + ",");
	}
	output(file, "\n");

	cout << "��ʱ���� = " << ((clock() - time) / CLOCKS_PER_SEC) << "��" << endl;
	fprintf(file, "��ʱ���� =  %d��, ƽ���ٶ� = %d\n", ((clock() - time) / CLOCKS_PER_SEC));
}
void ALNS::output(FILE* file, string content, int type) {
	cout << content;
	fprintf(file, content.c_str());
}

