#pragma once
#include <vector>
#include "ALNS/Component/IOperator.h"
#include "ALNS/BasicClass/IInformation.h"
#include "ALNS/Util/Roulette.h"
using namespace std;
// 算子管理器的父类
class IOperatorManager
{
private:
	vector<IOperator>* destroyOperators;
	vector<IOperator>* repairOperators;
	IInformation* information;
public:
	IOperatorManager(); 
	void registerOperator(IOperator* ioperator);
	void registerOperators(vector<IOperator>* _ioperators);
	void registerInformation(IInformation* _information);
	IOperator* selectDestroyOperator();
	IOperator* selectRepairOperator();
};