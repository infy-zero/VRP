#include "IOperatorManager.h"
IOperatorManager::IOperatorManager() 
{
	cout << "初始化算子管理器" << endl;
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
void IOperatorManager::registerInformation(IInformation* _information)
{
	cout << "\t算子管理器注入information" << endl;
	information = _information;
}

IOperator* IOperatorManager::selectDestroyOperator()
{
	return Roulette::getRouletteIOperator(destroyOperators);

}
IOperator* IOperatorManager::selectRepairOperator()
{
	return Roulette::getRouletteIOperator(repairOperators);

}