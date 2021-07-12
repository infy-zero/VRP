#include <stdlib.h>
#include <string>
#include "ISolution.h"
#include "ALNS/BasicClass/ALNSNode.h"
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
double ISolution::compare(ISolution* other)
{
	return rand() % 5 - 2;
}
void ISolution::addRemovedList(ALNSNode* node)
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
