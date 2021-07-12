#pragma once
#include <vector>
#include <unordered_set>
#include <string>
#include <iostream>
#include "ALNS/Component/ISolution.h"
#include "ALNS/setting/ALNS_Setting.h"
#include "ALNS/BasicClass/IInformation.h"
using namespace std;
// ���н�������ĸ���
class ISolutionManager
{
private:
	unordered_set<string> solutions;
	IInformation* information;
	ISolution* bestSol;
public:
	ISolutionManager();
	void initialize();
	enum SolutionFrequency push(ISolution* curSol);
	ISolution* getBestSolution();

	void setBestSolution(ISolution* _bestSol);
	void setCurSol(ISolution* _curSol);
};