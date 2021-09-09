#include "ISolutionNode.h"
#include "FerryTaskSetting.h"
#include "MyException.h"

ISolutionNode::ISolutionNode(const shared_ptr <FerryVehicleTask>& _task) :
	task(_task),
	isUpdate(true),
	curTime(-DBL_MAX),
	minTime(-DBL_MAX),
	maxTime(DBL_MAX),
	minVehicleTime(-DBL_MAX),
	maxVehicleTime(DBL_MAX),
	minFlightTime(-DBL_MAX),
	maxFlightTime(DBL_MAX),
	state(INREMOVELIST)
{
	if (_task->type != DP) // ���������Ϊ��վ�ڵ�
	{
		// Ϊ�˷�����ԣ��˴�������_virtualflight��Ϊ�յ��ж�������ʵ������ʱӦ��������
		if (vft.size() != 0)
		{
			throw MyException("The size of virtualFight should be zero.");
		}
	}

}
