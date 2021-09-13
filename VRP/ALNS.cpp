#include "ALNS.h"

#include <algorithm>
#include <ctime>
#include <memory>
#include <unordered_set>

#include "ALNS/Util/Util.h"
#include "MyException.h"

ALNS::ALNS():virtualFlight(inf consequence)
{
	cout << "���� ALNS" << endl;
	// ���ɳ�ʼ��
	if (curSol == nullptr) {
		curSol.reset(new Solution());
	} else {
		throw MyException("The current solution exists.");
	}
	// ȷ���ڶɳ����񣨳����������ޣ���������������Ϊ����������������
	total_vehicle_num = inf nodes.nodes_.size();
	// ���ɳ�ʼ�⣬ע��information
	nodes_ = &inf nodes;
	// ����������ѹ��RemovedList
	for (int i = 0; i < nodes_->nodes_.size(); i++) {
		int id = nodes_->nodes_.at(i).task->id;// �ڶɳ�����
		curSol->removedList.push_back(id);
	}
	// ��վ����
	if (A_S range == EAST) {
		depot_ = -2;
	}
	else {
		depot_ = -1;
	}
	// ����վ�š����нڵ�ע��solution
	curSol->depot_ = depot_;
	curSol->all_nodes_ = nodes_;
	cout << "ALNS����" << endl;
}

void ALNS::start()
{
	/*����Ӧ�����������ڴ����������Ļ����������˸�����ƻ��ͻָ����ӣ�ͬʱΪ��Щ���Ӹ���Ȩ�غ�������һ����Ӧ�㣬����Ӧ������Ҫ���Ƶ��������±���r��segment number �������ڣ���ѡ����׼�����ӼƷַ���*/
	cout << "    ��ʼALNS�㷨" << std::endl;
	clock_t time = clock();
	/*���ɳ�ʼ�⡪�����ûָ�����+Լ������*/
	cout << "		���ɳ�ʼ��" << std::endl;
	greedyInsertion(curSol);
	cout << "		��ʼ���������" << std::endl;
	cal_objectives(curSol);									// ���㵱ǰĿ�꺯��ֵ
	push_solution_space(nullptr, curSol);								// �����ռ�

	/*��ʼ���������ﵽֹͣ��׼֮ǰ*/
	cout << criterion.toString() << std::endl;
	while (criterion.iter())
	{
		// �����½�
		shared_ptr<Solution> newSol(new Solution(*curSol));
		// �ƻ���ǰ��
		randomDestroy(newSol);
		// �ָ���ǰ��
		greedyInsertion(newSol);
		// ����newSol��Ϣ
		newSol->update_value();
		// �ж��Ƿ�����½�
		SolutionFrequency sf = push_solution_space(&criterion, newSol);
		string solutionString = newSol->toString_with_update();
		if (sf == BEST) // ���Ž�ֱ�Ӳ���SP�����µ�ǰ�⣬ȫ�����Ž�
		{
			solutionSpace.insert(make_pair(solutionString, newSol->curVal));
			curSol = newSol;
			bestSol = newSol;
		} else if (sf == NEVER_BUT_BETTER) { // �ȵ�ǰ����ã������SP
			solutionSpace.insert(make_pair(solutionString, newSol->curVal));
			curSol = newSol;
		} else if (sf == SA_ACCEPT) {		// û�е�ǰ��ã����Ǹ���ģ���˻�׼�����
			solutionSpace.insert(make_pair(solutionString, newSol->curVal));
			curSol = newSol;
		}
		else {	// �ܾ���ǰ��
			
		}
		// ���µ�ǰ��ķ���
		updateScores(sf);
		// ���µ�ǰ״̬
		update();
		// ���� Adaptive layer ����
		if (criterion.isUpdateParamter()) {
			updateParameters();
		}
	}
	std::cout << "    �������������Ƶ���������" << criterion.getTotalTimes() << std::endl;
	std::cout << "��ǰĿ�꺯��ֵ = " << curSol->curVal << "����ǰ������ = " << curSol->vehicles.size() << "����ǰ�� = " << curSol->toString_with_update() << endl;
	if (bestSol) {
		std::cout << "����ֵ = " << bestSol->curVal << "�����Ž� = " << bestSol->toString_with_update() << endl;
	}
	cout << "��ʱ���� = " << ((clock() - time) / CLOCKS_PER_SEC) << "��" << endl;
}
// �ָ�����
/*�������*/
// �������removedList�е����нڵ�
void ALNS::randomInsertion(shared_ptr<Solution>& solution) {
	if (solution == nullptr) {											// solutionΪ�գ�Ĭ������curSol
		throw MyException("You can check non-exist solution.");
	}
	std::cout << "�������" << std::endl;
	while (!solution->removedList.empty()) { // ֱ��removedList���
		int node_left = solution->removedList.size();
		randomInsertionOnce(solution);
		if (node_left == solution->removedList.size()) { // �ڵ�û�з����任������������
			if (solution->has_empty_vehicle()) {
				throw MyException("You can not insert in a solution with empty vehicle.");
			}
			solution->addVehicle();
		}
	}
}
// �������һ���ڵ�
void ALNS::randomInsertionOnce(shared_ptr<Solution>& solution) {
	if (solution->removedList.empty()) {								// ��removedListΪ�գ���ֹ����
		throw MyException("The removedList is empty, no need for insertion!");
	}
	shared_ptr<Solution> local_best_solution = nullptr;					// ��ǰ���Ž�
	bool tag = true;
	int sss = 0;
	// �����һ���ڵ���ǰ����������ɾ��ʱ�临�Ӷ�
	for (int i = 0; i < solution->removedList.size(); i++) {
		// ��������λ�ã�ȷ���Ƿ��ܲ���
		for (int j = 0; j < solution->vehicles.size(); j++) {				// �������г�
			for (int k = 1; k < solution->vehicles.at(j).size(); k++) {		// ������������λ��
				shared_ptr<Solution> localSol(new Solution(*solution));		// ���ɵ�ǰ��Ŀ���
				localSol->insert(j, k, solution->removedList.at(i));		// �����µĽ�
				localSol->update_value();								    // �����µĽ�Ŀ�꺯������Ϣ

				bool solution_is_feasible = check_solution_feasible(localSol);
				//if (solution->removedList.size() > -1 ) {
				//	cout << sss++ << ", " << solution_is_feasible << endl;
				//}
				if (solution_is_feasible && localSol->isBetter(local_best_solution)) {			// ��ǰ������Ҹ���������滻

					local_best_solution = localSol;
					local_best_solution->removedList.erase(local_best_solution->removedList.begin() + i);
				}
			}
		}
	}
	// ����ʧ��ʱ�������³��������һ���ڵ�����³�
	if (local_best_solution == nullptr) {
		if (solution->has_empty_vehicle()) {
			throw MyException("You can not insert in a solution with empty vehicle.");
		}
		return;
	}
	else { // ���򽫵�ǰ�������Ϊ�ֲ����Ž�
		solution = local_best_solution;
	}
}

// ̰������removedList�е����нڵ�
void ALNS::greedyInsertion(shared_ptr<Solution>& solution) {
	if (solution == nullptr) {											// solutionΪ�գ�Ĭ������curSol
		throw MyException("You can check non-exist solution.");
	}
	std::cout << "̰������" << std::endl;
	while (!solution->removedList.empty()) { // ֱ��removedList���
		// cout << "��ǰʣ��ڵ��� = " << solution->removedList.size() << endl;
		int node_left = solution->removedList.size();
		greedyInsertionOnce(solution);
		if (node_left == solution->removedList.size()) { // �ڵ�û�з����任������������
			if (solution->has_empty_vehicle()) { // �������ճ��󻹲��ܽ��е�����֤�����������⣬��Ҫɾ���ڵ���л���
				throw MyException("You can not insert in a solution with empty vehicle.");
			}
			solution->addVehicle();
		}
	}
}
// ����һ���ڵ�
void ALNS::greedyInsertionOnce(shared_ptr<Solution>& solution) {
	if (solution->removedList.empty()) {								// ��removedListΪ�գ���ֹ����
		throw MyException("The removedList is empty, no need for insertion!");
	}
	shared_ptr<Solution> local_best_solution = nullptr;					// ��ǰ���Ž�
	bool tag = true;
	//cout << "insert once" << endl;
	// �����һ���ڵ���ǰ����������ɾ��ʱ�临�Ӷ�
	for (int i = 0; i< solution->removedList.size(); i++) {
		// ��������λ�ã�ȷ���Ƿ��ܲ���
		for (int j = 0; j < solution->vehicles.size(); j++) {				// �������г�
			for (int k = 1; k < solution->vehicles.at(j).size(); k++) {		// ������������λ��
				shared_ptr<Solution> localSol(new Solution(*solution));		// ���ɵ�ǰ��Ŀ���
				localSol->insert(j, k, solution->removedList.at(i));		// �����µĽ�
				localSol->update_value();								    // �����µĽ�Ŀ�꺯������Ϣ
				
				bool is_feasible = check_solution_feasible(localSol);
				
				//cout << "removedList" << solution->removedList.size() << ", feasible" << is_feasible << ",empty" << solution->has_empty_vehicle() << endl;

				if (is_feasible && localSol->isBetter(local_best_solution)) {			// ��ǰ������Ҹ���������滻

					local_best_solution = localSol;	
					local_best_solution->removedList.erase(local_best_solution->removedList.begin() + i);
				}
			}
		}
	}
	// ����ʧ��ʱ�������³��������һ���ڵ�����³�
	if (local_best_solution == nullptr) {
		if (solution->has_empty_vehicle()) {
			throw MyException("You can not insert into a solution with empty vehicle.");
		}
		return;
	} else { // ���򽫵�ǰ�������Ϊ�ֲ����Ž�
		solution = local_best_solution;
	}
}

/*�ƻ�����*/ 
// ���ɾ�� 4-60% ���ڵ�
void  ALNS::randomDestroy(shared_ptr<Solution>& solution) {
	if (solution == nullptr) {
		throw MyException("You can check non-exist solution.");
	}
	std::cout << "���ɾ��" << std::endl;
	if (solution->all_nodes_->nodes_.size() == solution->removedList.size()) { // û�нڵ����ɾ��
		throw MyException("You can not delete from an empty solution.");
	}
	int delete_node_num = static_cast<int> (Util::getRandom() * 0.6 * solution->all_nodes_->nodes_.size());
	while (delete_node_num--) {
		solution->random_delete_without_update();
	}
	solution->clear_empty_vehicle();
	solution->update_value();

}

// ����Ŀ�꺯��
double ALNS::cal_objectives(shared_ptr<Solution>& solution) {
	if (solution == nullptr) {
		throw MyException("You can check non-exist solution.");
	}
	std::cout << "����Ŀ�꺯��" << std::endl;
	return solution->curVal;
}
// ����ǰ�������ռ�
SolutionFrequency ALNS::push_solution_space(ICriterion* criterion, shared_ptr<Solution>& solution) {
	// ����nullptrʱ�����õ�ǰ��
	if (solution == nullptr) {
		throw MyException("You can check non-exist solution.");
	}
	// ��û���жϱ�׼ʱ����Ϊ��һ�Σ�ֱ�ӽ���
	double curT;
	if (criterion == nullptr) { // curT = inf -> p = 1
		curT = DBL_MAX;
	} else {
		curT = criterion->getCurT();
	}
	// ����Ŀ�꺯��ֵ
	cal_objectives(solution);
	// �����ַ���
	string solution_str = solution->toString_with_update();
	auto tmp = solutionSpace.find(solution_str);
	if (solution->isBetter(bestSol)) { // �ж��Ƿ�Ϊ���Ž⣬�����Ž�+1.5
		return BEST;
	} else if (tmp != solutionSpace.end() && curSol->isBetter(solution)) { // �ж��Ƿ�Ϊδ���ֹ��Ľ⣬�ȵ�ǰ�������+1.2
		return NEVER_BUT_BETTER;
	} else { // ģ���˻�
		if (simulated_annealing(curT, solution->curVal)) { // ���ܵ�ǰ��
			return SA_ACCEPT;
		} else { // �ܾ���ǰ��
			return SA_REJECT;
		}
	}
	throw exception("You can not reach here!");
	return BEST;
}

// ģ���˻�׼��
bool ALNS::simulated_annealing(double curT, double curVal) {
	double possibility = exp( - (curVal - bestSol->curVal) / curT);	// ϵͳ������״̬�ĸ���
	double random = Util::getRandom();							// �������0��1��
	if (random < possibility) {
		return true;
	} else {
		return false;
	}
}

// ���²�����Ϣ
void ALNS::updateParameters() {

}

// ���µ÷����
void ALNS::updateScores(SolutionFrequency sf) {
	// TODO: ���µ�ǰ׷�ӵ÷�
}

// ���µ�ǰ����Ϣ����Ҫ���º�/��ǰ���
void ALNS::update() {
	// TODO: ���µ�ǰ��״̬-�����ڲ��Խ׶ε��ԣ����ڻ��Ƴ�

}

// �жϵ�ǰ���Ƿ���С�����updateSolution���ף��߼�һ�£������漰������ѧ���㣬ĳ�̶ֳ���˵����������ҪΪ�˼�������
bool ALNS::check_solution_feasible(shared_ptr<Solution>& solution) {
	// TODO: �жϵ�ǰ���Ƿ����
	if (solution == nullptr) {
		throw MyException("You can check non-exist solution.");
	}

	/*��ʼ��*/
	// �����нڵ���Ϊδ֪�ڵ�
	solution->all_nodes_->set_state_all_unknown();
	solution->all_nodes_->set_nodes_unchecked();
	solution->all_nodes_->set_depot_unchecked();

	/*��������*/
	// �����ʻ��������
	//cout << "check max length" << endl;
	if (!solution->judge_not_exceed_max_length()) {
		return false;
	}

	//cout << "check reachable" << endl;
	// Ҫ�������������пɴ���
	if (!solution->judge_sequence_feasible()) {
		return false;
	}

	return true;
}

