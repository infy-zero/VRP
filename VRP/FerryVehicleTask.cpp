#include "FerryVehicleTask.h"
#include "FerryTaskSetting.h"

FerryVehicleTask::FerryVehicleTask(
	int _id,
	Flight* _flight,
	int _serviceStartTime,
	int _serviceEndTime,
	int predefined_service_max_before,
	int boarding_time,
	int alighting_time,
	enum FVTType _type):id(_id),flight(_flight),predefined_earliest_service_start_time(_serviceStartTime),predefined_service_latest_start_time(_serviceEndTime),boarding_time_(boarding_time), predefined_service_max_before_(predefined_service_max_before),alighting_time_(alighting_time),type(_type)
{
	// 最早到达时间 = 服务开始时间 - 最大等待时间
	predefined_earliest_arrival_time = predefined_earliest_service_start_time - FerryTaskSetting::FTS_timewindow;
}
ostream& operator<<(ostream& outs, FerryVehicleTask& ferryVehicleTask)
{
	outs <<
		ferryVehicleTask.flight->toString() << ", " <<
		ferryVehicleTask.id << ", " << 
		ferryVehicleTask.predefined_earliest_arrival_time << ", " <<
		ferryVehicleTask.getServiceTime() << ", " <<
		ferryVehicleTask.getServiceStartTime() << ", " <<
		ferryVehicleTask.getServiceEndTime() << std::endl;
	return outs;
}
