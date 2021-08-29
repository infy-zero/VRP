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
	enum FVTType _type):id(_id),flight(_flight),serviceStartTime(_serviceStartTime),serviceEndTime(_serviceEndTime),serviceTime(_serviceTime),type(_type)
{
	earliestArrivalTime = serviceStartTime - FerryTaskSetting::timeWindow;
}
ostream& operator<<(ostream& outs, FerryVehicleTask& ferryVehicleTask)
{
	outs <<
		ferryVehicleTask.getFlight()->toString() << ", " <<
		ferryVehicleTask.getID() << ", " << 
		ferryVehicleTask.earliestArrivalTime << ", " <<
		ferryVehicleTask.getServiceTime() << ", " <<
		ferryVehicleTask.getServiceStartTime() << ", " <<
		ferryVehicleTask.getServiceEndTime() << std::endl;
	return outs;
}
