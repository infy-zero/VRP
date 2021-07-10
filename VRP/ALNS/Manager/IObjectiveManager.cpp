#include "IObjectiveManager.h"
IObjectiveManager::IObjectiveManager()
{
	cout << "��ʼ��Ŀ�������" << endl;
}
vector<double> IObjectiveManager::getObjectives()
{
	vector<double> objectiveValues;
	for (int i = 0; i < objectives->size(); i++)
		objectiveValues.at(i) = objectives->at(i).getObjective();
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
void IObjectiveManager::registerInformation(IInformation* _information)
{
	cout << "\tĿ�������ע��information" << endl;
	information = _information;
}