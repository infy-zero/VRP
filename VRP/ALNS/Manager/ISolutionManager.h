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
	ISolution* initSol;
public:
	ISolutionManager();
	enum SolutionFrequency push()
	{
		// ��ǰ������Ž����
		if (bestSol == nullptr || (bestSol != nullptr && initSol->compare(bestSol)))
		{
			solutions.insert(initSol->toString());
			delete bestSol;
			bestSol = new ISolution(*initSol);
			return BEST;
		}
		else if (solutions.count(initSol->toString()))
			return ALREADY;
		else
			return NEVER;
		
	}
	ISolution* getInitSolution();
	ISolution* getBestSolution();
	void setInitSolution(ISolution* _initSol);
	void setBestSolution(ISolution* _bestSol);
};