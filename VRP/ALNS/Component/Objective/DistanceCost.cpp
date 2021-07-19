#include "DistanceCost.h"
DistanceCost::DistanceCost()
{
	cout << "\t注册行驶成本目标函数" << endl;
	setLevel(2);
}
// 返回当前解车辆使用成本
double DistanceCost::calObjective(ISolution& solution)
{
	double totalLength = 0; 
	for (int i=0;i<solution.getVehicles().size();i++)
		totalLength += solution.getVehicles().at(i).getLength();
	return pcost * totalLength;
}