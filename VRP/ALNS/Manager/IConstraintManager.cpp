#include "IConstraintManager.h"

IConstraintManager::IConstraintManager()
{
	cout << "��ʼ��Լ��������" << endl;
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
	cout << "\tԼ��������ע��information" << endl;
	information = _information;
}