#include "ISolutionManager.h"
ISolutionManager::ISolutionManager()
{
	cout << "��ʼ�����н������" << endl;
}
ISolution* ISolutionManager::getInitSolution()
{
	return initSol;
}
ISolution* ISolutionManager::getBestSolution()
{
	return bestSol;
}
void ISolutionManager::setInitSolution(ISolution* _initSol)
{
	delete initSol;
	initSol = _initSol;
}
void ISolutionManager::setBestSolution(ISolution* _bestSol)
{
	delete bestSol;
	bestSol = _bestSol;
}