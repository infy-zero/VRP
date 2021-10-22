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
	return "    起始温度：" + std::to_string(curT) + "，终止温度：" + std::to_string(endT) + "，每次迭代次数：" + std::to_string(curTimes) + "，降温速率：" + std::to_string(decay) + "，参数更新频率：" + std::to_string(curSegment);
}
