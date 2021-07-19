#include "VehicleCost.h"
VehicleCost::VehicleCost()
{
	cout << "\t注册车辆成本目标函数" << endl;
	setLevel(1);
}
// 返回当前解车辆使用成本
double VehicleCost::calObjective(ISolution& solution)
{
	return costPerVehicle * solution.getVehicles().size();
}