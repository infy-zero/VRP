#include "IConstraintManager.h"

IConstraintManager::IConstraintManager()
{
	cout << "初始化约束管理器" << endl;
}
void IConstraintManager::registerConstraint(IConstraint* constraint)
{
	constrants->push_back(*constraint);
}
void IConstraintManager::registerConstraints(vector<IConstraint>* constraints)
{
	constraints = constraints;
}
bool IConstraintManager::getFeasible()
{
	bool feasible = true;
	for (int i = 0; i < constrants->size(); i++)
		feasible &= constrants->at(i).getFeasible();
	return feasible;
}
void IConstraintManager::registerInformation(IInformation* _information)
{
	cout << "\t约束管理器注入information" << endl;
	information = _information;
}