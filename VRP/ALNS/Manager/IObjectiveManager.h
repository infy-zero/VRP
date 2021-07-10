#pragma once

#include <vector>
#include <assert.h>
#include <iostream>

#include "ALNS/Component/IObjective.h"
#include "ALNS/BasicClass/IInformation.h"
using namespace std;
// Ŀ�꺯���������ĸ���
class IObjectiveManager
{
private:
	vector<IObjective>* objectives;
	IInformation* information;
public:
	IObjectiveManager();
	vector<double> getObjectives();
	void registerObjective(IObjective* objective);
	void registerObjectives(vector<IObjective>* _objectives);
	void registerInformation(IInformation* _information);
};