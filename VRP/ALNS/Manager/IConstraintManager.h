#pragma once
#include <iostream>
#include <vector>
#include "ALNS/Component/IConstraint.h"
#include "ALNS/BasicClass/ALNSNode.h"
#include "ALNS/BasicClass/IInformation.h"
//#include "IConstraint.h"

using namespace std;
// 约束管理器的父类
class IConstraintManager
{
private:
	// double violation;
	vector<IConstraint>* constrants;
	IInformation* information;
public:
	IConstraintManager();
	bool getFeasible();
	void registerConstraint (IConstraint* constraint);
	void registerConstraints(vector<IConstraint>* constraints);
	void registerInformation(IInformation* _information);

};