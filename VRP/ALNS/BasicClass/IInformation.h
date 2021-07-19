#pragma once
#include <vector>
#include "ALNS/BasicClass/ISolutionNode.h"
#include "ALNS/Component/IConstraint.h"

#include "ALNS/Component/IObjective.h"
#include "ALNS/Component/Objective/VehicleCost.h"
#include "ALNS/Component/Objective/DistanceCost.h"

#include "ALNS/Component/IOperator.h"
#include "ALNS/Component/ISolution.h"
#include "FerryVehicleTask.h"
#include "Flight.h"
#include <unordered_map>

using namespace std;
class IInformation
{
private:
	// 任务节点
	vector<FerryVehicleTask>* nodes;
	// 距离矩阵
	vector<vector<double>>* matrix;
	// 类型-数量哈希表
	unordered_map<char, int>* flight2FerryVehcle;
	// 角标-名称数组
	vector<string>* index2flightName;
	// 名称-角标哈希表
	unordered_map<string, int>* flightName2index;
	// 飞机
	vector<Flight>* flightTasks;
	// 约束
	vector<vector<int>>* consequence;


public:
	IInformation(); 
	~IInformation();
	vector<FerryVehicleTask>*		getNodes();
	void setNodes(vector<FerryVehicleTask>* _nodes);
	vector<vector<double>>* getMatrix();

	void setDestroyOperators(vector<IOperator>* _destoryOperators);
	unordered_map<char, int>* getFlight2FerryVehcle();
	// 角标-名称数组
	vector<string>* getIndex2flightName();
	// 名称-角标哈希表
	unordered_map<string, int>* getFlightName2index();
	// 飞机
	vector<Flight>* getFlightTasks();
	// 约束
	vector<vector<int>>* getConsequence();
};