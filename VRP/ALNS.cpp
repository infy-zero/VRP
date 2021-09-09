#include "ALNS.h"

#include <algorithm>
#include <ctime>
#include <memory>

#include "ALNS/Util/Util.h"
#include "MyException.h"

ALNS::ALNS():virtualFlight(inf consequence)
{
	cout << "���� ALNS" << endl;
	// ȷ���ڶɳ����񣨳����������ޣ���������������Ϊ����������������
	total_vehicle_num = inf nodes.nodes_.size();
	// ���ɳ�ʼ�⣬ע��information
	nodes_ = &inf nodes;
	// ����������ѹ��RemovedList
	for (int i = 0; i < nodes_->nodes_.size(); i++) {
		int id = nodes_->nodes_.at(i).task->id;// �ڶɳ�����
		curSol.removedList.push_back(id);
	}
	// ��վ����
	if (A_S range == EAST) {
		depot_ = -2;
	}
	else {
		depot_ = -1;
	}
	// ����վ�š����нڵ�ע��solution
	curSol.depot_ = depot_;
	curSol.nodes_ = nodes_;
	cout << "ALNS����" << endl;
}

void ALNS::start()
{
	/*����Ӧ�����������ڴ����������Ļ����������˸�����ƻ��ͻָ����ӣ�ͬʱΪ��Щ���Ӹ���Ȩ�غ�������һ����Ӧ�㣬����Ӧ������Ҫ���Ƶ��������±���r��segment number �������ڣ���ѡ����׼�����ӼƷַ���*/
	cout << "    ��ʼALNS�㷨" << std::endl;
	clock_t time = clock();
	/*���ɳ�ʼ�⡪�����ûָ�����+Լ������*/
	cout << "		���ɳ�ʼ��" << std::endl;
	greedyInsertion();
	cout << "		��ʼ���������" << std::endl;
	cal_objectives();									// ���㵱ǰĿ�꺯��ֵ
	push_solution_space();								// �����ռ�

	/*��ʼ���������ﵽֹͣ��׼֮ǰ*/
	cout << criterion.toString() << std::endl;
	while (criterion.iter())
	{
		// �����½�
		Solution newSol = curSol;
		// �ƻ���ǰ��
		randomDestroy(&newSol);
		// �ָ���ǰ��
		greedyInsertion(&newSol);
		// ����newSol��Ϣ
		updateSolution(&newSol);
		// �ж��Ƿ�����½�
		SolutionFrequency sf = push_solution_space(&criterion, &newSol);
		string solutionString = newSol.toString();
		if (sf == BEST) // ���Ž�ֱ�Ӳ���SP�����µ�ǰ�⣬ȫ�����Ž�
		{
			solutionSpace.insert(make_pair(solutionString, newSol.curVal));
			curSol  = newSol;
			bestSol = newSol;
		} else if (sf == NEVER_BUT_BETTER) { // �ȵ�ǰ����ã������SP
			solutionSpace.insert(make_pair(solutionString, newSol.curVal));
			curSol = newSol;
		} else if (sf == SA_ACCEPT) {		// û�е�ǰ��ã����Ǹ���ģ���˻�׼�����
			solutionSpace.insert(make_pair(solutionString, newSol.curVal));
			curSol = newSol;
		}
		else {								// �ܾ���ǰ��
			
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
	std::cout << "��ǰĿ�꺯��ֵ = " << curSol.curVal << "����ǰ������ = " << curSol.vehicles.size() << "����ǰ�� = " << curSol.toString();
	std::cout << "����ֵ = " << bestSol.curVal << "�����Ž� = " << bestSol.toString();
	cout << "��ʱ���� = " << ((clock() - time) / CLOCKS_PER_SEC) << "��" << endl;
}
// �ָ�����
// ����removedList�е����нڵ�
void ALNS::greedyInsertion(Solution* solution) {
	if (solution == nullptr) {											// solutionΪ�գ�Ĭ������curSol
		solution = &curSol;
	}
	std::cout << "̰������" << std::endl;
	while (!solution->removedList.empty()) { // ֱ��removedList���
		int node_left = solution->removedList.size();
		greedyInsertionOnce(solution);
		if (node_left == solution->removedList.size()) { // �ڵ�û�з����任������������
			solution->addVehicle();
		}
	}
}
// ����һ���ڵ�
void ALNS::greedyInsertionOnce(Solution* solution) {
	if (solution->removedList.empty()) {								// ��removedListΪ�գ���ֹ����
		throw MyException("The removedList is empty, no need for insertion!");
	}
	unique_ptr<Solution> local_best_solution = nullptr;					// ��ǰ���Ž�
	bool tag = true;
	// �����һ���ڵ���ǰ����������ɾ��ʱ�临�Ӷ�
	for (int i = 0; i< solution->removedList.size(); i++) {
		// ��������λ�ã�ȷ���Ƿ��ܲ���
		for (int j = 0; j < solution->vehicles.size(); j++) {				// �������г�
			for (int k = 1; k < solution->vehicles.at(j).size(); k++) {		// ������������λ��
				Solution localSol = *solution;							// ���ɵ�ǰ��Ŀ���
				localSol.insert(j, k, solution->removedList.at(i));		// �����µĽ�
				updateSolution(&localSol);								// �����µĽ�Ŀ�꺯������Ϣ
				if (check_solution_feasible(&localSol) && localSol.isBetter(local_best_solution)) {			// ��ǰ������Ҹ���������滻
					local_best_solution.reset(new Solution(localSol));	
					local_best_solution->removedList.erase(local_best_solution->removedList.begin() + i);
				}
			}
		}
	}
	// ����ʧ��ʱ�������³��������һ���ڵ�����³�
	if (local_best_solution == nullptr || !local_best_solution) {
		return;
	} else { // ���򽫵�ǰ�������Ϊ�ֲ����Ž�
		*solution = *(local_best_solution.get());
	}
}

// �ƻ�����
void  ALNS::randomDestroy(Solution* solution) {
	if (solution == nullptr) {
		solution = &curSol;
	}
	std::cout << "���ɾ��" << std::endl;
}

// ����Ŀ�꺯��
double ALNS::cal_objectives(Solution* solution) {
	if (solution == nullptr) {
		solution = &curSol;
	}
	std::cout << "����Ŀ�꺯��" << std::endl;
	return solution->curVal;
}
// ����ǰ�������ռ�
SolutionFrequency ALNS::push_solution_space(ICriterion* criterion, Solution* solution) {
	// ����nullptrʱ�����õ�ǰ��
	if (solution == nullptr) {
		solution = &curSol;
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
	string solution_str = solution->toString();
	auto tmp = solutionSpace.find(solution_str);
	if (solution->curVal < bestSol.curVal) { // �ж��Ƿ�Ϊ���Ž⣬�����Ž�+1.5
		return BEST;
	} else if (tmp != solutionSpace.end() && solution->curVal > curSol.curVal) { // �ж��Ƿ�Ϊδ���ֹ��Ľ⣬�ȵ�ǰ�������+1.2
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
	double possibility = exp( - (curVal - bestSol.curVal) / curT);	// ϵͳ������״̬�ĸ���
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

// ���½����Ϣ
void ALNS::updateSolution(Solution* solution) {
	// TODO: ���·���
	double total_cost = 0;								// ��ʼ����Ϊ0
	for (int i = 0; i < solution->vehicles.size(); i++) { // ����Ŀ�꺯��
		total_cost += solution->cal_solution_cost();
	}
	solution->curVal = total_cost;

}

// ���µ�ǰ����Ϣ����Ҫ���º�/��ǰ���
void ALNS::update() {
	// TODO: ���µ�ǰ��״̬-�����ڲ��Խ׶ε��ԣ����ڻ��Ƴ�

}

// �жϵ�ǰ���Ƿ����
bool ALNS::check_solution_feasible(Solution* solution) {
	// TODO: �жϵ�ǰ���Ƿ����
	if (solution == nullptr) {
		solution = &curSol;
	}
	for (int i=0;i<solution->vehicles.size();i++)
		if (solution->vehicles[i].size() > 10)
			return false;
	
	return true;
}
