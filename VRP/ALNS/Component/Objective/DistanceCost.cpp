#include "DistanceCost.h"
DistanceCost::DistanceCost()
{
	cout << "\tע����ʻ�ɱ�Ŀ�꺯��" << endl;
	setLevel(2);
}
// ���ص�ǰ�⳵��ʹ�óɱ�
double DistanceCost::calObjective(ISolution& solution)
{
	double totalLength = 0; 
	for (int i=0;i<solution.getVehicles().size();i++)
		totalLength += solution.getVehicles().at(i).getLength();
	return pcost * totalLength;
}