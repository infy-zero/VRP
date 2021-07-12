#include "IInformation.h"
#include "FerryVehicleTask.h"
using namespace std;
IInformation::IInformation()
{
	cout << "初始化静态数据" << endl;
	nodes				= new vector<FerryVehicleTask>;
	matrix				= new vector<vector<double>>;
	flight2FerryVehcle = new unordered_map<char, int>;
	index2flightName = new vector<string>;
	flightName2index = new unordered_map<string, int>;
	flightTasks = new vector<Flight>;
	consequence = new vector<vector<int>>;

	constraints			= new vector<IConstraint>;
	objectives			= new vector<IObjective>;
	IObjective* objective = new IObjective;
	objectives->push_back(*objective);
	// 算子
	repairOperators		= new vector<IOperator>;
	IOperator* repair   = new IOperator;
	repair->setType(REPAIR);
	repairOperators->push_back(*repair);
	IOperator* repair1 = new IOperator;
	repair1->setType(REPAIR);
	repairOperators->push_back(*repair1);
	destroyOperators	= new vector<IOperator>;
	IOperator* destroy  = new IOperator;
	destroy->setType(DESTROY);
	destroyOperators->push_back(*destroy);
	IOperator* destroy1 = new IOperator;
	destroy1->setType(DESTROY);
	destroyOperators->push_back(*destroy1);
}
vector<FerryVehicleTask>* IInformation::getNodes()
{
	return nodes;
}
void IInformation::setNodes(vector<FerryVehicleTask>* _nodes)
{
	nodes = _nodes;
}
void IInformation::setMatrix(vector<vector<double>>*_matrix)
{
	matrix = _matrix;
}
void IInformation::setConstraints(vector<IConstraint>* _constraints)
{
	constraints = _constraints;
}

void IInformation::setObjectives(vector<IObjective>* _objectives)
{
	objectives = _objectives;
}
void IInformation::setRepairOperators(vector<IOperator>* _repairOperators)
{
	repairOperators = _repairOperators;
}
void IInformation::setDestroyOperators(vector<IOperator>* _destoryOperators)
{
	destroyOperators = _destoryOperators;
}
vector<vector<double>>* IInformation::getMatrix()
{
	return matrix;
}
vector<IConstraint>* IInformation::getConstraints()
{
	return constraints;
}
vector<IObjective>* IInformation::getObjectives()
{
	return objectives;
}
vector<IOperator>* IInformation::getRepairOperators()
{
	return repairOperators;
}
vector<IOperator>* IInformation::getDestroyOperators()
{
	return destroyOperators;
}

unordered_map<char, int>* IInformation::getFlight2FerryVehcle()
{
	return flight2FerryVehcle;
}
// 角标-名称数组
vector<string>* IInformation::getIndex2flightName()
{
	return index2flightName;
}
// 名称-角标哈希表
unordered_map<string, int>* IInformation::getFlightName2index()
{
	return flightName2index;
}
// 飞机
vector<Flight>* IInformation::getFlightTasks()
{
	return flightTasks;
}
// 约束
vector<vector<int>>* IInformation::getConsequence()
{
	return consequence;
}
