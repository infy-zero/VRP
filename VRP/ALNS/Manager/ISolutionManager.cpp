#include "ISolutionManager.h"
ISolutionManager::ISolutionManager()
{
	cout << "�������н������" << endl;
}
void ISolutionManager::initialize()
{
	cout << "\t��ʼ�����н������" << endl;
}
enum SolutionFrequency ISolutionManager::push(ISolution* curSol)
{
	// ��ǰ������Ž����
	if (bestSol == nullptr || (bestSol != nullptr && curSol->compare(bestSol)))
	{
		solutions.insert(curSol->toString());
		delete bestSol;
		bestSol = new ISolution(*curSol);
		return BEST;
	}
	else if (solutions.count(curSol->toString()))
		return ALREADY;
	else
		return NEVER;

}
ISolution* ISolutionManager::getBestSolution()
{
	return bestSol;
}

void ISolutionManager::setBestSolution(ISolution* _bestSol)
{
	delete bestSol;
	bestSol = _bestSol;
}
