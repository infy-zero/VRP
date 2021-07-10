#include "IInformation.h"
IInformation::IInformation()
{
	cout << "初始化静态数据" << endl;
}
vector<ALNSNode>* IInformation::getNodes()
{
	return &nodes;
}
vector<vector<double>>* IInformation::getMatrix()
{
	return &matrix;
}
vector<IConstraint>* IInformation::getConstraints()
{
	return &constraints;
}
vector<IObjective>* IInformation::getObjectives()
{
	return &objectives;
}
vector<IOperator>* IInformation::getRepairOperators()
{
	return &repairOperators;
}
vector<IOperator>* IInformation::getDestroyOperators()
{
	return &destroyOperators;
}
