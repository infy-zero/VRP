#include "ISolutionNode.h"
#include "FerryTaskSetting.h"
#include "MyException.h"

// ��ʼ״̬������ʹ��
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
	if (_task->type != TYPE_DEPOT) // ���������Ϊ��վ�ڵ�
	{
		// Ϊ�˷�����ԣ��˴�������_virtualflight��Ϊ�յ��ж�������ʵ������ʱӦ��������
		if (vft.size() != 0)
		{
			throw MyException("The size of virtualFight should be zero.");
		}
	}

}

// �����нڵ�״̬����
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
