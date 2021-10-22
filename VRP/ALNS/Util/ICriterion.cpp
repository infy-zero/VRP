#include "ALNS/Util/ICriterion.h"

ICriterion::ICriterion()
{
}
bool ICriterion::iter()
{
	curSegment--;
	curTimes--;
	totalTimes++;
	if (totalTimes > A_S max_iteration) {
		return false;
	}
	if (curTimes < 0)
	{
		if (curT < endT)
			return false;
		else
		{
			curT *= decay;
			curTimes = ALNS_Setting::decayTimes;
			return true;
		}
	}

}
bool ICriterion::isUpdateParamter()
{
	if (curSegment == 0)
	{
		curSegment = ALNS_Setting::segment;
		return true;
	}
	else
		return false;
}
double ICriterion::getCurT()
{
	return curT;
}
int ICriterion::getTotalTimes()
{
	return totalTimes;
}
std::string ICriterion::toString()
{
	return "    ��ʼ�¶ȣ�" + std::to_string(curT) + "����ֹ�¶ȣ�" + std::to_string(endT) + "��ÿ�ε���������" + std::to_string(curTimes) + "���������ʣ�" + std::to_string(decay) + "����������Ƶ�ʣ�" + std::to_string(curSegment);
}
