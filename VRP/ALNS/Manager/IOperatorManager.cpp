#include "IOperatorManager.h"
IOperatorManager::IOperatorManager() 
{
	cout << "创建算子管理器" << endl;
}
void IOperatorManager::initialize()
{
	cout << "\t初始化算子管理器" << endl;
	cout << "\t    初始化所有算子权重" << endl;
	cout << "\t\t初始化所有破坏算子权重，破坏算子个数 = " << destroyOperators->size() << endl;
	for (int i = 0; i < destroyOperators->size(); i++)
		destroyOperators->at(i).setWeight(ALNS_Setting::initWeight);
	cout << "\t\t初始化所有恢复算子权重，恢复算子个数 = " << repairOperators->size() << endl;
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
	cout << "\t算子管理器注入information" << endl;
	information = _information;
	// 设置恢复算子，并赋予编号
	repairOperators = information->getRepairOperators();
	for (int i = 0; i < repairOperators->size(); i++)
		repairOperators->at(i).setIndex(i);
	// 设置破坏算子，并赋予编号
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
	cout << "\t    更新算子权重。" << endl;
	// 更新权重
	for (auto opt : *destroyOperators)
		opt.update();
	for (auto opt : *repairOperators)
		opt.update();
}