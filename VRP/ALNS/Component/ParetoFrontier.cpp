#include "ParetoFrontier.h"
int compareVector(vector<double> orginVal, vector<double> newVal);

bool ParetoFrontier::insert(ISolution& solution)
{
	bool tag = false;
	if (paretoFrontier.size() == 0)
	{
		tag = true;
		paretoFrontier.push_back(pair<vector<double>,string>(solution.getObjectives(),solution.toString()));
		cout << "\t\t压入当前解到帕累托前沿" << endl;
		return tag;
	}
	for (auto it = paretoFrontier.begin();it!= paretoFrontier.end();)
	{
		// 如果当前解更好，则删除原解
		bool isBetter = compareVector((*it).first, solution.getObjectives());
		if (isBetter = 1)
		{
			paretoFrontier.erase(it++);// 删除指针
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
	if (tag) // 如果当前解更好或者并非更坏则加入pareto解。
	{
		paretoFrontier.push_back(pair<vector<double>, string>(solution.getObjectives(), solution.toString()));
		cout << "\t\t压入当前解到帕累托前沿" << endl;
	}
	else
	{
		cout << "\t\t未压入当前解到帕累托前沿" << endl;
	}
	return tag;
}
// 计算两个结果好坏，当第一个解更好返回-1，更好返回1
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
