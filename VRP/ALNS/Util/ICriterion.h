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

	double curT = ALNS_Setting::startT;			// ��ʼ�¶�
	double endT = ALNS_Setting::endT;			// �����¶�
	int curSegment = ALNS_Setting::segment;		// ÿ���׶εĵ��������������²����Ĵ���
	const int max_segment = ALNS_Setting::segment;
	int curTimes = ALNS_Setting::decayTimes;	// ÿ���׶εĵ�������
	const double decay = ALNS_Setting::decay;	// ˥������
	int totalTimes = 0;

};
