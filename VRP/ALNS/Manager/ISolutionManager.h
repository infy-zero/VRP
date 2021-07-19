#pragma once
#include <vector>
#include <unordered_set>
#include <string>
#include <iostream>
#include "ALNS/Component/ISolution.h"
#include "ALNS/setting/ALNS_Setting.h"
#include "ALNS/BasicClass/IInformation.h"
#include "ALNS/Component/ParetoFrontier.h"
using namespace std;
// 可行解管理器的父类
class ISolutionManager
{
private:
	unordered_set<string> solutions;
	IInformation* information;
	ParetoFrontier pf;
	ISolution bestSol;
public:
	ISolutionManager();
	void initialize();
	enum SolutionFrequency push(ISolution& curSol);
	bool pushPF(ISolution curSol);
	ISolution getBestSolution();

	void setBestSolution(ISolution _bestSol);
	void setCurSol(ISolution _curSol);
};