#include "ALNS.h"
using namespace std;

ALNS::ALNS()
{
	cout << "启动 ALNS" << endl;
	// 生成初始解，注入information
	if (ALNS_Setting::range==WEST)
		curSol = new ISolution(inf depots.at(0));
	else if (ALNS_Setting::range == EAST)
		curSol = new ISolution(inf depots.at(1));
	// 将所有任务压入RemovedList
	for (int i=0;i<inf nodes.size();i++)
	{
		FerryVehicleTask* fvt = inf nodes.at(i); // 摆渡车任务
		pair<int, int>& position = inf positions[i];   
		vector<int>& virtualFlightList = inf consequence.at(position.first); // 该节点的虚拟航班列表
		int pos = position.second; // 实际的位置
		spn sn(new ISolutionNode(fvt, shared_ptr<vector<int>>(&virtualFlightList), pos));// 扔到堆里，和解共生。
		curSol->addNode(sn); // 所有节点信息存储在ISolution内
		curSol->getRemovedList()->push_back(sn);
	}
	cout << "ALNS就绪" << endl;
}

bool accept(double curT, ISolution* curSol, ISolution* newSol);
void ALNS::start()
{
	/*自适应大领域搜索在大领域搜索的基础上增加了更多的破坏和恢复算子，同时为这些算子赋予权重和增加了一个适应层，在适应层中需要控制迭代：更新比例r、segment number 更新周期，抽选算子准则，算子计分方法*/
	cout << "    开始ALNS算法" << endl;
	/*生成初始解——利用恢复算子+约束条件*/
	curSol->greedyInsertion();
	bestVal = curSol->getObjectives();
	solutionSpace.insert(curSol->toString());

	/*开始迭代——达到停止标准之前*/
	cout << criterion.toString() << endl;
	enum SolutionFrequency sf = ALREADY;
	while (criterion.iter())
	{ 
		/*构建新解*/
		ISolution* newSol = new ISolution(*curSol);
		/*破坏当前解*/
		pair<int, enum DestroyOperatorNum> ond = newSol->randomDestroy();
		/*恢复当前解*/
		pair<int, enum RepairOperatorNum> onr = newSol->greedyInsertion();
		/*利用模拟退火准则判断是否接受当前解*/
		bool isAccept = accept(criterion.getCurT(), curSol, newSol);
		if (isAccept)
		{
			string solutionString = newSol->toString();
			if (newSol->getObjectives() < bestVal)
			{
				solutionSpace.insert(solutionString);
				sf = BEST;
			}
			else if (solutionSpace.count(solutionString))// 没找到则插入
			{
				solutionSpace.insert(solutionString);
				sf = NEVER;
			}
			else
				sf = ALREADY;
			delete curSol;
			curSol = newSol;
		}
		else
			delete newSol;
		// 将当前解放入解空间
		curSol->updateScores(sf, ond, onr);
		/*更新 Adaptive layer 参数*/
		if (criterion.isUpdateParamter())
			curSol->updateParameter(ALNS_Setting::segment);
	}
	cout << "    迭代结束，共计迭代次数：" << criterion.getTotalTimes() << endl;
}
bool accept(double curT, ISolution* curSol, ISolution* newSol)
{
	return true;
}