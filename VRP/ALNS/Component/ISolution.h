#pragma once
#include <vector>
#include <iostream>
#include "ALNS/BasicClass/IVehicle.h"
#include "IRemovedList.h"
#include "FerryVehicleTask.h"
#include "ALNS/BasicClass/ISolutionNode.h"
#include "ALNS/BasicClass/ValueRange.h"
using namespace std;
// 所有解的父类
class ISolution
{
private:
	IRemovedList				removedList;
	vector<IVehicle>            vehicles;
	vector<double>				objectives;
public:
	ISolution();
	virtual string toString();
	virtual double compare(ISolution& other);
	virtual int comparePF(ISolution& other);
	vector<IVehicle> getVehicles();
	void setVehicles(vector<IVehicle> _vehicles);
	void addRemovedList(ISolutionNode node);
	void setObjectives(vector<double> _objectives);
	vector<double> getObjectives();
	vector<ValueRange> getValueRanges();
	void setValueRanges(vector<ValueRange> valueRanges);
	
};

