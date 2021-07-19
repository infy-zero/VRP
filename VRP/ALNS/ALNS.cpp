#include "ALNS.h"
using namespace std;
ALNS::ALNS()
{
	cout << "生成 ALNS" << endl;
}
void ALNS::generateInitialSolution()
{
	cout << "    开始生成初始解。" << endl;
	cout << "\t随机选择恢复算子。" << endl;
	IOperator* selectedOperator = iopm.selectRepairOperator();
	selectedOperator->run(curSol);
	vector<double> newVal = iobm.calObjectives(curSol);
	curSol.setObjectives(newVal);
	isom.push(curSol);
	cout << "\t初始解生成结束。" << endl;
}
void ALNS::start()
{
	/*自适应大领域搜索在大领域搜索的基础上增加了更多的破坏和恢复算子，同时为这些算子赋予权重和增加了一个适应层，在适应层中需要控制迭代：更新比例r、segment number 更新周期，抽选算子准则，算子计分方法*/
	cout << "    开始ALNS算法" << endl;

	/*生成初始解——利用恢复算子+约束条件*/
	generateInitialSolution();
	/*开始迭代——达到停止标准之前*/
	cout << criterion.toString() << endl;
	enum SolutionFrequency sf = ALREADY;
	while (criterion.iter())
	{ 
		/*两个轮盘赌选择破坏、恢复算子*/
		IOperator* destroy = iopm.selectDestroyOperator();
		IOperator* repair = iopm.selectRepairOperator();
		/*破坏当前解*/
		ISolution destroyed = destroy->run(curSol);
		/*恢复当前解*/
		ISolution repaired  = repair->run(destroyed);
		/*利用模拟退火准则判断是否接受当前解*/
		vector<double> newVal = iobm.calObjectives(repaired);
		repaired.setObjectives(newVal);
		curSol = iobm.accept(criterion.getCurT(), curSol, repaired);
		newVal.clear();
		newVal = iobm.calObjectives(curSol);
		curSol.setObjectives(newVal);
		// 将当前解放入解空间
		sf = isom.push(curSol);
		if (sf == NEVER || sf == BEST)
		{
			isom.pushPF(curSol);
		}
		destroy->addScores(sf);
		repair->addScores(sf);
		/*更新 Adaptive layer 参数*/
		if (criterion.isUpdateParamter())
			iopm.updateParameter();
	}
	cout << "    迭代结束，共计迭代次数：" << criterion.getTotalTimes() << endl;
}
void ALNS::setInformation(IInformation* _information)
{
	cout << "    ALNS注入静态信息" << endl;
	information = _information;
	icom.setInformation(information);
	iobm.setInformation(information);
	iopm.setInformation(information);
}
// 每当改变当前解时，向下进行传播
void ALNS::setSolution(ISolution _curSol)
{
	cout << "    ALNS注入初始解" << endl;
	curSol = _curSol;
}


IConstraintManager* ALNS::getICOM()
{
	return &icom;
}
IObjectiveManager* ALNS::getIOBM()
{
	return &iobm;
}
IOperatorManager* ALNS::getIOPM()
{
	return &iopm;
}
ISolutionManager* ALNS::getISOM()
{
	return &isom;
}