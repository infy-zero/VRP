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
	if (_task->type != DP) // 满足此条件为场站节点
	{
		// 为了方便调试，此处加入了_virtualflight不为空的判断条件，实际运行时应该跳过。
		if (vft.size() != 0)
		{
			throw MyException("The size of virtualFight should be zero.");
		}
	}

}
