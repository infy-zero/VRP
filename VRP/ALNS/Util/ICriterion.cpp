#include "ALNS/Util/ICriterion.h"
ICriterion::ICriterion()
{
}
bool ICriterion::iter()
{
	curSegment--;
	curTimes--;
	totalTimes++;
	if (curTimes < 0)
	{
		if (curT < endT)
			return false;
		else
		{
			curT	*= decay;
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
string ICriterion::toString()
{
	return "    ��ʼ�¶ȣ�"+to_string(curT)+"����ֹ�¶ȣ�"+to_string(endT)+"��ÿ�ε���������"+to_string(curTimes) +"���������ʣ�" + to_string(decay) +"����������Ƶ�ʣ�" +to_string(curSegment);
}
