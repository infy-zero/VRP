#include "ALNS.h"
using namespace std;

ALNS::ALNS()
{
	cout << "���� ALNS" << endl;
	// ���ɳ�ʼ�⣬ע��information
	if (ALNS_Setting::range==WEST)
		curSol = new ISolution(inf depots.at(0));
	else if (ALNS_Setting::range == EAST)
		curSol = new ISolution(inf depots.at(1));
	// ����������ѹ��RemovedList
	for (int i=0;i<inf nodes.size();i++)
	{
		FerryVehicleTask* fvt = inf nodes.at(i); // �ڶɳ�����
		pair<int, int>& position = inf positions[i];   
		vector<int>& virtualFlightList = inf consequence.at(position.first); // �ýڵ�����⺽���б�
		int pos = position.second; // ʵ�ʵ�λ��
		spn sn(new ISolutionNode(fvt, shared_ptr<vector<int>>(&virtualFlightList), pos));// �ӵ�����ͽ⹲����
		curSol->addNode(sn); // ���нڵ���Ϣ�洢��ISolution��
		curSol->getRemovedList()->push_back(sn);
	}
	cout << "ALNS����" << endl;
}

bool accept(double curT, ISolution* curSol, ISolution* newSol);
void ALNS::start()
{
	/*����Ӧ�����������ڴ����������Ļ����������˸�����ƻ��ͻָ����ӣ�ͬʱΪ��Щ���Ӹ���Ȩ�غ�������һ����Ӧ�㣬����Ӧ������Ҫ���Ƶ��������±���r��segment number �������ڣ���ѡ����׼�����ӼƷַ���*/
	cout << "    ��ʼALNS�㷨" << endl;
	/*���ɳ�ʼ�⡪�����ûָ�����+Լ������*/
	curSol->greedyInsertion();
	bestVal = curSol->getObjectives();
	solutionSpace.insert(curSol->toString());

	/*��ʼ���������ﵽֹͣ��׼֮ǰ*/
	cout << criterion.toString() << endl;
	enum SolutionFrequency sf = ALREADY;
	while (criterion.iter())
	{ 
		/*�����½�*/
		ISolution* newSol = new ISolution(*curSol);
		/*�ƻ���ǰ��*/
		pair<int, enum DestroyOperatorNum> ond = newSol->randomDestroy();
		/*�ָ���ǰ��*/
		pair<int, enum RepairOperatorNum> onr = newSol->greedyInsertion();
		/*����ģ���˻�׼���ж��Ƿ���ܵ�ǰ��*/
		bool isAccept = accept(criterion.getCurT(), curSol, newSol);
		if (isAccept)
		{
			string solutionString = newSol->toString();
			if (newSol->getObjectives() < bestVal)
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
			delete curSol;
			curSol = newSol;
		}
		else
			delete newSol;
		// ����ǰ������ռ�
		curSol->updateScores(sf, ond, onr);
		/*���� Adaptive layer ����*/
		if (criterion.isUpdateParamter())
			curSol->updateParameter(ALNS_Setting::segment);
	}
	cout << "    �������������Ƶ���������" << criterion.getTotalTimes() << endl;
}
bool accept(double curT, ISolution* curSol, ISolution* newSol)
{
	return true;
}