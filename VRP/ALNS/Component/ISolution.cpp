#include <stdlib.h>
#include <string>
#include "ISolution.h"
#include "ALNS/BasicClass/ISolutionNode.h"
ISolution::ISolution()
{
	cout << "\t    构建空解" << endl;

}

// 生成解的字符串表示
string ISolution::toString()
{
	return to_string(rand()%128);
}
// 比较两个解的优劣
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


