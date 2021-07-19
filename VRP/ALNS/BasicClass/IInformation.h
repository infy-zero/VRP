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
	// ����ڵ�
	vector<FerryVehicleTask>* nodes;
	// �������
	vector<vector<double>>* matrix;
	// ����-������ϣ��
	unordered_map<char, int>* flight2FerryVehcle;
	// �Ǳ�-��������
	vector<string>* index2flightName;
	// ����-�Ǳ��ϣ��
	unordered_map<string, int>* flightName2index;
	// �ɻ�
	vector<Flight>* flightTasks;
	// Լ��
	vector<vector<int>>* consequence;


public:
	IInformation(); 
	~IInformation();
	vector<FerryVehicleTask>*		getNodes();
	void setNodes(vector<FerryVehicleTask>* _nodes);
	vector<vector<double>>* getMatrix();

	void setDestroyOperators(vector<IOperator>* _destoryOperators);
	unordered_map<char, int>* getFlight2FerryVehcle();
	// �Ǳ�-��������
	vector<string>* getIndex2flightName();
	// ����-�Ǳ��ϣ��
	unordered_map<string, int>* getFlightName2index();
	// �ɻ�
	vector<Flight>* getFlightTasks();
	// Լ��
	vector<vector<int>>* getConsequence();
};