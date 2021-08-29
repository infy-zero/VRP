#include "ALNS.h"
#include <algorithm>

namespace alns {

ALNS::ALNS():virtualFlight(inf consequence)
{
	cout << "���� ALNS" << endl;
	curSol.curVal = DBL_MAX;
	// ���ɳ�ʼ�⣬ע��information
	if (ALNS_Setting::range == WEST)
		curSol.depot = inf depots.at(1)->getID();
	else if (ALNS_Setting::range == EAST)
		curSol.depot = inf depots.at(0)->getID();
	// ����������ѹ��RemovedList
	for (int i = 0; i < inf nodes.size(); i++) {
		FerryVehicleTask* fvt = inf nodes.at(i); // �ڶɳ�����
		ISolutionNode sn(fvt);// �ӵ�����ͽ⹲����
		curSol.nodes.push_back(std::move(sn));
		curSol.removedList.push_back(i);
	}
	// ����վ�ڵ�ѹ��nodesĩβ
	for (int i = 0; i < inf depots.size(); i++) {
		FerryVehicleTask* fvt = inf depots.at(i); // �ڶɳ�����
		ISolutionNode sn(fvt);// �ӵ�����ͽ⹲����
		curSol.nodes.push_back(std::move(sn));
	}
	cout << "ALNS����" << endl;
}

void ALNS::start()
{
	/*����Ӧ�����������ڴ����������Ļ����������˸�����ƻ��ͻָ����ӣ�ͬʱΪ��Щ���Ӹ���Ȩ�غ�������һ����Ӧ�㣬����Ӧ������Ҫ���Ƶ��������±���r��segment number �������ڣ���ѡ����׼�����ӼƷַ���*/
	std::cout << "    ��ʼALNS�㷨" << std::endl;
	/*���ɳ�ʼ�⡪�����ûָ�����+Լ������*/
	std::cout << "		���ɳ�ʼ��" << std::endl;
	greedyInsertion();
	std::cout << "		��ʼ���������" << std::endl;
	cal_objectives();									// ���㵱ǰĿ�꺯��ֵ
	push_solution_space();								// �����ռ�

	/*��ʼ���������ﵽֹͣ��׼֮ǰ*/
	std::cout << criterion.toString() << std::endl;
	enum SolutionFrequency sf = ALREADY;
	while (criterion.iter())
	{
		/*�����½�*/
		Solution newSol = curSol;
		/*�ƻ���ǰ��*/
		randomDestroy(&newSol);
		/*�ָ���ǰ��*/
		greedyInsertion(&newSol);
		// �����½�
		double grade = push_solution_space(&newSol);
		/*����ģ���˻�׼���ж��Ƿ���ܵ�ǰ��*/
		bool isAccept = accept(criterion.getCurT(), sol, newSol);
		if (isAccept)
		{
			string solutionString = newSol->toString();
			if (newSol->cal_objectives() < bestVal)
			{
				solutionSpace.insert(solutionString);
				sf = BEST;
			}
			else if (solutionSpace.count(solutionString))// û�ҵ������
			{
				solutionSpace.insert(solutionString);
				sf = NEVER;
			}
			else
				sf = ALREADY;
			delete sol;
			sol = newSol;
		}
		else
			delete newSol;
		// ����ǰ������ռ�
		sol->updateScores(sf, ond, onr);
		/*���� Adaptive layer ����*/
		if (criterion.isUpdateParamter())
			sol->updateParameter(ALNS_Setting::segment);
	}
	std::cout << "    �������������Ƶ���������" << criterion.getTotalTimes() << std::endl;
}
// �ָ�����
void ALNS::greedyInsertion(Solution* solution) {
	if (solution == nullptr) {
		solution = &curSol;
	}
	std::cout << "̰������" << std::endl;
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
double ALNS::push_solution_space(Solution* solution) {
	// ����nullptrʱ�����õ�ǰ��
	if (solution == nullptr) {
		solution = &curSol;
	}
	// ����Ŀ�꺯��ֵ
	cal_objectives(solution);
	// �����ַ���
	string solution_str = toString(solution);
	
	if (solution->curVal < bestSol.curVal) { // �ж��Ƿ�Ϊ���Ž�
		solutionSpace.insert(make_pair(solution_str, solution->curVal));
		return 1.2;
	} else if ()
	auto tmp = solutionSpace.find(solution_str);
	if (tmp == solutionSpace.end()) {

	}
	return 0;
}

// �����ǰ��
std::string ALNS::toString(Solution* solution) {
	if (solution == nullptr) {
		solution = &curSol;
	}
	// ɾ���ճ�
	for (auto iter = solution->sol.end(); iter != solution->sol.begin(); iter--) {
		if (iter->size() < 2) {
			throw new exception("�����ڵ�С��2");
		} else if (iter->size() == 2) {
			iter = solution->sol.erase(iter);
		}
	}
	// �Ƚ�������
	sort(solution->sol.begin(), solution->sol.end());
	std::string result;
	for (int i = 0; i < solution->sol.size(); i++) {
		for (int j = 0; j < solution->sol[i].size(); j++) {
			result += std::to_string(solution->sol[i][j]) + ',';
		}
		result += '\n';
	}
	return result;
}
} // alns
