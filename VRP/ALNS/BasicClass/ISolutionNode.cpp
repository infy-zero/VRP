#include "ISolutionNode.h"
#include "FerryTaskSetting.h"
#include "MyException.h"

// ��ʼ״̬������ʹ�á�������Ϊ��������״̬
ISolutionNode::ISolutionNode(FerryVehicleTask* _task, enum NodeType type) :
	task(_task),
	arrive_earliest_time(A_S time_max_value),
	service_earliest_start_time(A_S time_max_value),
	service_latest_start_time(-A_S time_max_value),
	state(NodeState::UNKNOWN),
	type_(type)
{

}

// �����нڵ�״̬���á�������Ϊ������״̬
void ISolutionNode::reset_feasible() {
	arrive_earliest_time = -A_S time_max_value;
	service_earliest_start_time = -A_S time_max_value;
	service_latest_start_time = A_S time_max_value;

	state = NodeState::UNKNOWN;
}

double ISolutionNode::cal_arrive_earliest_time() {
	return service_earliest_start_time - task->predefined_service_max_before_;
}
double ISolutionNode::get_arrive_earliest_time() {
	arrive_earliest_time = service_earliest_start_time - task->predefined_service_max_before_;
	return arrive_earliest_time;
}

void ISolutionNode::reset_infeasible() {
	arrive_earliest_time = DBL_MAX;
	service_earliest_start_time = DBL_MAX;
	service_latest_start_time = -DBL_MAX;

	state = NodeState::UNKNOWN;
}
