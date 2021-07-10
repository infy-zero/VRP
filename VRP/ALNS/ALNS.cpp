#include "ALNS.h"
using namespace std;
ALNS::ALNS()
{
	curSol = isom.getInitSolution();
}
void ALNS::generateInitialSolution()
{
	cout << "    Starting to generate initial solution." << endl;
	
}
void ALNS::start()
{
	/*自适应大领域搜索在大领域搜索的基础上增加了更多的破坏和恢复算子，同时为这些算子赋予权重和增加了一个适应层，在适应层中需要控制迭代：更新比例r、segment number 更新周期，抽选算子准则，算子计分方法*/
	cout << "    开始ALNS算法" << endl;
	/*生成初始解——利用恢复算子+约束条件*/
	generateInitialSolution();
	/*初始化各个算子的权重*/

	/*开始迭代——达到停止标准之前*/
		/*两个轮盘赌选择破坏、恢复算子*/

		/*破坏当前解*/

		/*恢复当前解*/

		/*利用模拟退火准则判断是否接受当前解*/

		/*更新Adaptive layer 参数*/

}
void ALNS::registerInformation(IInformation* _information)
{
	cout << "    ALNS注入静态信息" << endl;
	information = _information;
	icom.registerInformation(information);
	iobm.registerInformation(information);
	iopm.registerInformation(information);
}