#include "IOperatorManager.h"
IOperatorManager::IOperatorManager() 
{
	cout << "�������ӹ�����" << endl;
}
void IOperatorManager::initialize()
{
	cout << "\t��ʼ�����ӹ�����" << endl;
	cout << "\t    ��ʼ����������Ȩ��" << endl;
	cout << "\t\t��ʼ�������ƻ�����Ȩ�أ��ƻ����Ӹ��� = " << destroyOperators->size() << endl;
	for (int i = 0; i < destroyOperators->size(); i++)
		destroyOperators->at(i).setWeight(ALNS_Setting::initWeight);
	cout << "\t\t��ʼ�����лָ�����Ȩ�أ��ָ����Ӹ��� = " << repairOperators->size() << endl;
	for (int i = 0; i < repairOperators->size(); i++)
		repairOperators->at(i).setWeight(ALNS_Setting::initWeight);
	
}
void IOperatorManager::registerOperator(IOperator* ioperator)
{
	switch (ioperator->getType())
	{
	case DESTROY:
		destroyOperators->push_back(*ioperator);
		ioperator->setIndex(destroyOperators->size());
		break;
	case REPAIR:
		repairOperators->push_back(*ioperator);
		ioperator->setIndex(repairOperators->size());
		break;
	default:
		throw "Wrong operator type can't be recognized.";
	}
}
void IOperatorManager::registerOperators(vector<IOperator>* _ioperators)
{
	for (auto _operator : *_ioperators)
		registerOperator(&_operator);
}
void IOperatorManager::setInformation(IInformation* _information)
{
	cout << "\t���ӹ�����ע��information" << endl;
	information = _information;
	// ���ûָ����ӣ���������
	repairOperators = information->getRepairOperators();
	for (int i = 0; i < repairOperators->size(); i++)
		repairOperators->at(i).setIndex(i);
	// �����ƻ����ӣ���������
	destroyOperators = information->getDestroyOperators();
	for (int i = 0; i < destroyOperators->size(); i++)
		destroyOperators->at(i).setIndex(i);
}

IOperator* IOperatorManager::selectDestroyOperator()
{
	return getRouletteIOperator(destroyOperators);
}
IOperator* IOperatorManager::selectRepairOperator()
{
	IOperator* result = getRouletteIOperator(repairOperators);
	return result;
}

IOperator* IOperatorManager::getRouletteIOperator(vector<IOperator>* operators)
{
	double totalWeight = 0;
	for (int i = 0; i < operators->size(); i++)
		totalWeight += operators->at(i).getWeight();
	double selectedWeight = Roulette::getRandom() * totalWeight;
	for (int i = 0; i < operators->size(); i++)
	{
		selectedWeight -= operators->at(i).getWeight();
		if (selectedWeight <= 0)
		{
			return &(operators->at(i));
		}
	}
	return &(operators->at(operators->size() - 1));
}
vector<IOperator>* IOperatorManager::getDestroyOperators()
{
	return destroyOperators;
}
vector<IOperator>* IOperatorManager::getRepairOperators()
{
	return repairOperators;
}
void IOperatorManager::updateParameter()
{
	cout << "\t    ��������Ȩ�ء�" << endl;
	// ����Ȩ��
	for (auto opt : *destroyOperators)
		opt.update();
	for (auto opt : *repairOperators)
		opt.update();
}