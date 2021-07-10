#include "FerryVehicleTask.h"
FerryVehicleTask::FerryVehicleTask()
{
}
FerryVehicleTask::FerryVehicleTask(
	int _id,
	Flight& _flight,
	int _serviceStartTime,
	int _serviceEndTime,
	int _serviceTime):id(_id),flight(_flight),serviceStartTime(_serviceStartTime),serviceEndTime(_serviceEndTime),boardingTime(_serviceTime)
{
}
ostream& operator<<(ostream& outs, FerryVehicleTask& ferryVehicleTask)
{
	outs <<
		ferryVehicleTask.getFlight().toString() << ", " <<
		ferryVehicleTask.getID() << ", " << 
		ferryVehicleTask.getServiceTime() << ", " <<
		ferryVehicleTask.getServiceStartTime() << ", " <<
		ferryVehicleTask.getServiceEndTime() << std::endl;
	return outs;
}
