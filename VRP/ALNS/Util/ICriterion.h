#pragma once
#include <iostream>
#include "ALNS_Setting.h"

class ICriterion
{
public:
	ICriterion();
	bool iter();
	bool isUpdateParamter();
	double getCurT();
	int getTotalTimes();
	std::string toString();

	double curT = ALNS_Setting::startT;			// 起始温度
	double endT = ALNS_Setting::endT;			// 结束温度
	int curSegment = ALNS_Setting::segment;		// 每个阶段的迭代次数——更新参数的次数
	const int max_segment = ALNS_Setting::segment;
	int curTimes = ALNS_Setting::decayTimes;	// 每个阶段的迭代次数
	const double decay = ALNS_Setting::decay;	// 衰减速率
	int totalTimes = 0;

};
