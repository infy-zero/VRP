#include "FerryVehicleTask.h"
#include "FerryTaskSetting.h"
FerryVehicleTask::FerryVehicleTask()
{
}
FerryVehicleTask::FerryVehicleTask(
	int _id,
	Flight* _flight,
	int _serviceStartTime,
	int _serviceEndTime,
	int _serviceTime,
	enum FVTType _type):id(_id),flight(_flight),predefined_earliest_service_start_time(_serviceStartTime),predefined_service_latest_start_time(_serviceEndTime),serviceTime(_serviceTime),type(_type)
{
	predefined_earliest_arrival_time = predefined_earliest_service_start_time - FerryTaskSetting::FTS_TIMEWINDOW_BEFORE_START;
}
ostream& operator<<(ostream& outs, FerryVehicleTask& ferryVehicleTask)
{
	outs <<
		ferryVehicleTask.getFlight()->toString() << ", " <<
		ferryVehicleTask.getID() << ", " << 
		ferryVehicleTask.predefined_earliest_arrival_time << ", " <<
		ferryVehicleTask.getServiceTime() << ", " <<
		ferryVehicleTask.getServiceStartTime() << ", " <<
		ferryVehicleTask.getServiceEndTime() << std::endl;
	return outs;
}
