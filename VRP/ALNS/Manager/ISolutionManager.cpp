#include "ISolutionManager.h"
ISolutionManager::ISolutionManager()
{
	cout << "创建可行解管理器" << endl;
}
void ISolutionManager::initialize()
{
	cout << "\t初始化可行解管理器" << endl;
}
enum SolutionFrequency ISolutionManager::push(ISolution* curSol)
{
	// 当前解比最优解更好
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
