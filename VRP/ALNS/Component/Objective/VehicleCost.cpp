#include "VehicleCost.h"
VehicleCost::VehicleCost()
{
	cout << "\tע�ᳵ���ɱ�Ŀ�꺯��" << endl;
	setLevel(1);
}
// ���ص�ǰ�⳵��ʹ�óɱ�
double VehicleCost::calObjective(ISolution& solution)
{
	return costPerVehicle * solution.getVehicles().size();
}