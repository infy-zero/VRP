#pragma once
#include <vector>
#include "ALNS/BasicClass/ALNSNode.h"
#include "ALNS/Component/IConstraint.h"
#include "ALNS/Component/IObjective.h"
#include "ALNS/Component/IOperator.h"
#include "ALNS/Component/ISolution.h"

using namespace std;
class IInformation
{
private:
	vector<ALNSNode> nodes;
	vector<vector<double>> matrix;
	vector<IConstraint> constraints;
	vector<IObjective> objectives;
	vector<IOperator> repairOperators;
	vector<IOperator> destroyOperators;
public:
	IInformation(); 
	vector<ALNSNode>*		getNodes();
	vector<vector<double>>* getMatrix();
	vector<IConstraint>*	getConstraints();
	vector<IObjective>*		getObjectives();
	vector<IOperator>*		getRepairOperators();
	vector<IOperator>*		getDestroyOperators();
};