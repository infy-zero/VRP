#include "ISolutionManager.h"
ISolutionManager::ISolutionManager()
{
	cout << "�������н������" << endl;
}
void ISolutionManager::initialize()
{
	cout << "\t��ʼ�����н������" << endl;
}
enum SolutionFrequency ISolutionManager::push(ISolution& curSol)
{
	// ��ǰ������Ž����
	if (curSol.compare(bestSol))
	{
		solutions.insert(curSol.toString());
		bestSol = curSol;
		return BEST;
	}
	else if (solutions.count(curSol.toString()))
		return ALREADY;
	else
		return NEVER;

}
bool ISolutionManager::pushPF(ISolution curSol)
{
	return pf.insert(curSol);
}
ISolution ISolutionManager::getBestSolution()
{
	return bestSol;
}

void ISolutionManager::setBestSolution(ISolution _bestSol)
{
	bestSol = _bestSol;
}
