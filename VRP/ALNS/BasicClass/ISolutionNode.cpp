#include "ISolutionNode.h"
#include "FerryTaskSetting.h"
#include "MyException.h"

// 初始状态都不能使用
ISolutionNode::ISolutionNode(const shared_ptr <FerryVehicleTask>& _task, enum NodeType type) :
	task(_task),
	curTime(DBL_MAX),
	arrive_earliest_time(DBL_MAX),
	serive_earliest_start_time(DBL_MAX),
	servie_latest_end_time(-DBL_MAX),
	minVehicleTime(DBL_MAX),
	maxVehicleTime(-DBL_MAX),
	minFlightTime(DBL_MAX),
	maxFlightTime(-DBL_MAX),
	state(NodeState::UNKNOWN),
	type_(type)
{
	if (_task->type != TYPE_DEPOT) // 满足此条件为场站节点
	{
		// 为了方便调试，此处加入了_virtualflight不为空的判断条件，实际运行时应该跳过。
		if (vft.size() != 0)
		{
			throw MyException("The size of virtualFight should be zero.");
		}
	}

}

// 将所有节点状态重置
void ISolutionNode::reset_feasible() {
	curTime = -DBL_MAX;
	arrive_earliest_time = -DBL_MAX;
	serive_earliest_start_time = -DBL_MAX;
	servie_latest_end_time = DBL_MAX;

	minVehicleTime = -DBL_MAX;
	maxVehicleTime = DBL_MAX;

	minFlightTime = -DBL_MAX;
	maxFlightTime = DBL_MAX;

	state = NodeState::UNKNOWN;
}

void ISolutionNode::reset_infeasible() {
	curTime = DBL_MAX;
	arrive_earliest_time = DBL_MAX;
	serive_earliest_start_time = DBL_MAX;
	servie_latest_end_time = -DBL_MAX;

	minVehicleTime = DBL_MAX;
	maxVehicleTime = -DBL_MAX;

	minFlightTime = DBL_MAX;
	maxFlightTime = -DBL_MAX;

	state = NodeState::UNKNOWN;
}
