#include "IObjectiveManager.h"
IObjectiveManager::IObjectiveManager()
{
	cout << "����Ŀ�������" << endl;
}
void IObjectiveManager::initialize()
{
	cout << "\t��ʼ��Ŀ�������" << endl;
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
	cout << "\tĿ�������ע��information" << endl;
	information = _information;
}
ISolution* IObjectiveManager::accept(double curT, ISolution* before, ISolution* after)
{
	ISolution* result;
	double delta = before->compare(after);
	// ����½����
	if (delta < 0)
	{
		cout << "\t\t���ܸ��Ž⣬delta = " << delta << endl;
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
			cout << "\t\t���ܵ�ǰ�⣬random = " << random << "��delta= " << delta << "��value = " << value << endl;
			delete before;
			result = after;
			after = nullptr;
		}
		else
		{
			cout << "\t\t�����ܵ�ǰ�⣬random = " << random << "��delta= " << delta << "��value = " << value << endl;
			delete after;
			after = nullptr;
			result = before;
		}
	}
	return result;
}