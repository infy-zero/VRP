#include <stdlib.h>
#include <string>
#include "ISolution.h"
#include "ALNS/BasicClass/ALNSNode.h"
ISolution::ISolution()
{
	cout << "�����ս�" << endl;
}
// ���ɽ���ַ�����ʾ
string ISolution::toString()
{
	return to_string(rand()%128);
}
// �Ƚ������������
bool ISolution::compare(ISolution* other)
{
	return rand() % 2;
}
void ISolution::addRemovedList(ALNSNode* node)
{
	removedList.add(node);
}
