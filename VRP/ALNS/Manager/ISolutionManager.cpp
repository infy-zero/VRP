#include "ISolutionManager.h"
ISolutionManager::ISolutionManager()
{
	cout << "创建可行解管理器" << endl;
}
void ISolutionManager::initialize()
{
	cout << "\t初始化可行解管理器" << endl;
}
enum SolutionFrequency ISolutionManager::push(ISolution& curSol)
{
	// 当前解比最优解更好
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
