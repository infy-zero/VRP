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

}
IInformation::~IInformation()
{
	delete nodes;
	delete matrix;
	delete flight2FerryVehcle;
	delete index2flightName;
	delete flightName2index;
	delete flightTasks;
	delete consequence;
}
vector<FerryVehicleTask>* IInformation::getNodes()
{
	return nodes;
}
void IInformation::setNodes(vector<FerryVehicleTask>* _nodes)
{
	nodes = _nodes;
}

vector<vector<double>>* IInformation::getMatrix()
{
	return matrix;
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
