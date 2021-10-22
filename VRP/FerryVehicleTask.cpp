#include "FerryVehicleTask.h"
#include "FerryTaskSetting.h"

FerryVehicleTask::FerryVehicleTask(
	int id,
	Flight* flight,
	const double& serviceStartTime,
	const double& serviceEndTime,
	const double& predefined_service_max_before,
	const double& boarding_time,
	const double& alighting_time,
	const double& extra_service_time,
	int from_index,
	int end_index,
	enum FVTType _type):id_(id),flight_(flight),predefined_earliest_service_start_time_(serviceStartTime),predefined_service_latest_start_time_(serviceEndTime),boarding_time_(boarding_time), predefined_service_max_before_(predefined_service_max_before), extra_service_time_(extra_service_time), from_index_(from_index),
	end_index_(end_index), alighting_time_(alighting_time),type_(_type)
{
	// 最早到达时间 = 服务开始时间 - 最大等待时间
	predefined_earliest_arrival_time_ = predefined_earliest_service_start_time_ - predefined_service_max_before;

	predefined_latest_arrival_time_ = predefined_service_latest_start_time_;

}
ostream& operator<<(ostream& outs, FerryVehicleTask& ferryVehicleTask)
{
	outs <<
		ferryVehicleTask.id_ << ", " <<
		ferryVehicleTask.flight_->toString() << ", " <<
		ferryVehicleTask.flight_->direction << ", " <<
		ferryVehicleTask.predefined_earliest_arrival_time_ << ", " <<
		ferryVehicleTask.get_predefined_earliest_service_start_time() << ", " <<
		ferryVehicleTask.get_predefined_service_latest_start_time() << ", " <<
		ferryVehicleTask.total_service_time_ << ", " <<
		ferryVehicleTask.service_distance_ << ", " <<
		ferryVehicleTask.get_boarding_time() << ", "<< 
		ferryVehicleTask.alighting_time_  << std::endl;
	return outs;
}
const int FerryVehicleTask::getID()
{
    return id_;
}
Flight* FerryVehicleTask::getFlight()
{
    return flight_;
}
const double FerryVehicleTask::get_predefined_earliest_service_start_time()
{
    return predefined_earliest_service_start_time_;
}
const double FerryVehicleTask::get_predefined_service_latest_start_time()
{
    return predefined_service_latest_start_time_;
}
const double  FerryVehicleTask::get_predefined_earliest_arrival_time() {
	return predefined_earliest_arrival_time_;
}
const double FerryVehicleTask::get_boarding_time()
{
    return boarding_time_;
}
void FerryVehicleTask::set_boarding_time(double boarding_time)
{
    boarding_time_ = boarding_time;
}
double  FerryVehicleTask::get_total_service_time() {
	return total_service_time_;
}
