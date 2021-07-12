#include "ALNS.h"
using namespace std;
ALNS::ALNS()
{
	cout << "���� ALNS" << endl;
}
void ALNS::generateInitialSolution()
{
	cout << "    ��ʼ���ɳ�ʼ�⡣" << endl;
	cout << "\t���ѡ��ָ����ӡ�" << endl;
	IOperator* selectedOperator = iopm.selectRepairOperator();
	curSol = selectedOperator->run(curSol);
	isom.push(curSol);
	cout << "\t��ʼ�����ɽ�����" << endl;
}
void ALNS::start()
{
	/*����Ӧ�����������ڴ����������Ļ����������˸�����ƻ��ͻָ����ӣ�ͬʱΪ��Щ���Ӹ���Ȩ�غ�������һ����Ӧ�㣬����Ӧ������Ҫ���Ƶ��������±���r��segment number �������ڣ���ѡ����׼�����ӼƷַ���*/
	cout << "    ��ʼALNS�㷨" << endl;
	/*��ʼ��*/
	initialize();
	/*���ɳ�ʼ�⡪�����ûָ�����+Լ������*/
	generateInitialSolution();
	/*��ʼ���������ﵽֹͣ��׼֮ǰ*/
	cout << criterion.toString() << endl;
	enum SolutionFrequency sf = ALREADY;
	while (criterion.iter())
	{ 
		/*�������̶�ѡ���ƻ����ָ�����*/
		IOperator* destroy = iopm.selectDestroyOperator();
		IOperator* repair = iopm.selectRepairOperator();
		/*�ƻ���ǰ��*/
		ISolution* destroyed = destroy->run(curSol);
		/*�ָ���ǰ��*/
		ISolution* repaired  = repair->run(destroyed);
		/*����ģ���˻�׼���ж��Ƿ���ܵ�ǰ��*/
		vector<double> newVal = iobm.calObjectives(repaired);
		repaired->setObjectives(newVal);
		curSol = iobm.accept(criterion.getCurT(), curSol, repaired);
		sf = isom.push(curSol);
		destroy->addScores(sf);
		repair->addScores(sf);
		/*���� Adaptive layer ����*/
		if (criterion.isUpdateParamter())
			iopm.updateParameter();
	}
	cout << "    �������������Ƶ���������" << criterion.getTotalTimes() << endl;
}
void ALNS::setInformation(IInformation* _information)
{
	cout << "    ALNSע�뾲̬��Ϣ" << endl;
	information = _information;
	icom.setInformation(information);
	iobm.setInformation(information);
	iopm.setInformation(information);
}
// ÿ���ı䵱ǰ��ʱ�����½��д���
void ALNS::setSolution(ISolution* _curSol)
{
	cout << "    ALNSע���ʼ��" << endl;
	delete curSol;
	curSol = _curSol;
}
void ALNS::initialize()
{
	cout << "    ��ʼ��ʼ��" << endl;
	icom.initialize();
	iobm.initialize();
	iopm.initialize();
	isom.initialize();
}

IConstraintManager* ALNS::getICOM()
{
	return &icom;
}
IObjectiveManager* ALNS::getIOBM()
{
	return &iobm;
}
IOperatorManager* ALNS::getIOPM()
{
	return &iopm;
}
ISolutionManager* ALNS::getISOM()
{
	return &isom;
}