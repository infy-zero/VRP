#pragma once
#include <vector>
#include "ALNS/Component/IOperator.h"
#include "ALNS/BasicClass/IInformation.h"
#include "ALNS/Util/Roulette.h"
#include "ALNS/setting/ALNS_Setting.h"
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
	void initialize();
	void registerOperator(IOperator* ioperator);
	void registerOperators(vector<IOperator>* _ioperators);
	void setInformation(IInformation* _information);
	IOperator* selectDestroyOperator();
	IOperator* selectRepairOperator();
	IOperator* getRouletteIOperator(vector<IOperator>* operators);
	vector<IOperator>* getDestroyOperators();
	vector<IOperator>* getRepairOperators();
	void updateParameter();
};