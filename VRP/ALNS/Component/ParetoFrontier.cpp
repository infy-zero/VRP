#include "ParetoFrontier.h"
int compareVector(vector<double> orginVal, vector<double> newVal);

bool ParetoFrontier::insert(ISolution& solution)
{
	bool tag = false;
	if (paretoFrontier.size() == 0)
	{
		tag = true;
		paretoFrontier.push_back(pair<vector<double>,string>(solution.getObjectives(),solution.toString()));
		cout << "\t\tѹ�뵱ǰ�⵽������ǰ��" << endl;
		return tag;
	}
	for (auto it = paretoFrontier.begin();it!= paretoFrontier.end();)
	{
		// �����ǰ����ã���ɾ��ԭ��
		bool isBetter = compareVector((*it).first, solution.getObjectives());
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
		paretoFrontier.push_back(pair<vector<double>, string>(solution.getObjectives(), solution.toString()));
		cout << "\t\tѹ�뵱ǰ�⵽������ǰ��" << endl;
	}
	else
	{
		cout << "\t\tδѹ�뵱ǰ�⵽������ǰ��" << endl;
	}
	return tag;
}
// ������������û�������һ������÷���-1�����÷���1
int compareVector(vector<double> orginVal, vector<double> newVal)
{
	assert(orginVal.size() == newVal.size());
	int better = 0;
	int equals = 0;
	int worse = 0;
	for (int i = 0; i < orginVal.size(); i++)
	{
		if (orginVal[i] < newVal[i])
			better++;
		else if (orginVal[i] == newVal[i])
			equals++;
		else
			worse++;
	}
	if (equals + worse == orginVal.size())
		return -1;
	if (equals + better == orginVal.size())
		return 1;
	return 0;


}
