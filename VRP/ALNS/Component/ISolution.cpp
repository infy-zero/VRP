#include <stdlib.h>
#include <string>
#include "ISolution.h"
#include "ALNS/BasicClass/ISolutionNode.h"
ISolution::ISolution()
{
	cout << "\t    �����ս�" << endl;

}

// ���ɽ���ַ�����ʾ
string ISolution::toString()
{
	return to_string(rand()%128);
}
// �Ƚ������������
double ISolution::compare(ISolution& other)
{
	return rand() % 5 - 2;
}
int ISolution::comparePF(ISolution& other)
{
	return rand() % 3 - 1;
}
vector<IVehicle> ISolution::getVehicles()
{
	return vehicles;
}
void ISolution::setVehicles(vector<IVehicle> _vehicles)
{
	vehicles = _vehicles;
}
void ISolution::addRemovedList(ISolutionNode node)
{
	removedList.add(node);
}
void ISolution::setObjectives(vector<double> _objectives)
{
	objectives = _objectives;
}
vector<double> ISolution::getObjectives()
{
	return objectives;
}


