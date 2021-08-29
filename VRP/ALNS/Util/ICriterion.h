#pragma once
#include <iostream>
#include "ALNS_Setting.h"
using namespace std;
class ICriterion
{
private:
	double curT			= ALNS_Setting::startT;
	double endT			= ALNS_Setting::endT;
	int curSegment		= ALNS_Setting::segment;
	int curTimes		= ALNS_Setting::decayTimes;
	const double decay	= ALNS_Setting::decay;
	int totalTimes		= 0;
public:
	ICriterion();
	bool iter();
	bool isUpdateParamter();
	double getCurT();
	int getTotalTimes();
	string toString();
};