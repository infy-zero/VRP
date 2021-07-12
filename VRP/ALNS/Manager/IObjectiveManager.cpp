#include "IObjectiveManager.h"
IObjectiveManager::IObjectiveManager()
{
	cout << "创建目标管理器" << endl;
}
void IObjectiveManager::initialize()
{
	cout << "\t初始化目标管理器" << endl;
	objectives = information->getObjectives();
}
vector<double> IObjectiveManager::calObjectives(ISolution* solution)
{
	vector<double> objectiveValues;
	for (int i = 0; i < objectives->size(); i++)
	{
		while (objectiveValues.size() < i + 1)
			objectiveValues.push_back(0);
		objectiveValues.at(i) = objectives->at(i).calObjective(solution);
	}
	return objectiveValues;
}
void IObjectiveManager::registerObjective(IObjective* objective)
{
	objectives->push_back(*objective);
}
void IObjectiveManager::registerObjectives(vector<IObjective>* _objectives)
{
	objectives = _objectives;
}
void IObjectiveManager::setInformation(IInformation* _information)
{
	cout << "\t目标管理器注入information" << endl;
	information = _information;
}
ISolution* IObjectiveManager::accept(double curT, ISolution* before, ISolution* after)
{
	ISolution* result;
	double delta = before->compare(after);
	// 如果新解更好
	if (delta < 0)
	{
		cout << "\t\t接受更优解，delta = " << delta << endl;
		delete before;
		result = after;
		after = nullptr;
	}
	else
	{
		double random = Roulette::getRandom();
		double value = exp(-delta / curT);
		if (random > value)
		{
			cout << "\t\t接受当前解，random = " << random << "，delta= " << delta << "，value = " << value << endl;
			delete before;
			result = after;
			after = nullptr;
		}
		else
		{
			cout << "\t\t不接受当前解，random = " << random << "，delta= " << delta << "，value = " << value << endl;
			delete after;
			after = nullptr;
			result = before;
		}
	}
	return result;
}