#include <stdlib.h>
#include <string>
#include "ISolution.h"
#include "ALNS/BasicClass/ALNSNode.h"
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
