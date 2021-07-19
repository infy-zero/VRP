#pragma once

#include <vector>
#include <assert.h>
#include <iostream>

#include "ALNS/Component/IObjective.h"
#include "ALNS/BasicClass/IInformation.h"
#include "ALNS/Util/Roulette.h"
#include "ALNS/Util/ICriterion.h"
using namespace std;
// Ŀ�꺯���������ĸ���
class IObjectiveManager
{
private:
	vector<IObjective*> objectives;
	IInformation* information;
public:
	IObjectiveManager();
	vector<double> calObjectives(ISolution& solution);
	void registerObjective(IObjective* objective);
	void registerObjectives(vector<IObjective*> _objectives);
	void setInformation(IInformation* _information);
	ISolution accept(double curT, ISolution& before, ISolution& after);
};