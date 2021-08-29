#include "ParetoFrontier.h"
int compareVector(vector<double> orginVal, vector<double> newVal);

bool ParetoFrontier::insert(ISolution& solution)
{
	bool tag = false;
	if (paretoFrontier.size() == 0)
	{
		tag = true;
		paretoFrontier.push_back(pair<double,string>(solution.getObjectives(),solution.toString()));
		cout << "\t\tѹ�뵱ǰ�⵽������ǰ��" << endl;
		return tag;
	}
	for (auto it = paretoFrontier.begin();it!= paretoFrontier.end();)
	{
		// �����ǰ����ã���ɾ��ԭ��
		int isBetter = compareVector((*it).first, solution.getObjectives());
		if (isBetter = 1)
		{
			paretoFrontier.erase(it++);// ɾ��ָ��
			tag = true;
		}
		else if (isBetter == 0)
		{
			tag = true;
			it++;
		}
		else
			it++;
	}
	if (tag) // �����ǰ����û��߲��Ǹ��������pareto�⡣
	{
		paretoFrontier.push_back(pair<double, string>(solution.getObjectives(), solution.toString()));
		cout << "\t\tѹ�뵱ǰ�⵽������ǰ��" << endl;
	}
	else
	{
		cout << "\t\tδѹ�뵱ǰ�⵽������ǰ��" << endl;
	}
	return tag;
}
// ������������û�������һ������÷���-1�����÷���1
int compareVector(double orginVal, double newVal)
{
	if (orginVal < newVal)
		return -1;
	if (orginVal == newVal)
		return 1;
	return 0;


}
