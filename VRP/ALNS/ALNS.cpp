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
	/*����Ӧ�����������ڴ����������Ļ����������˸�����ƻ��ͻָ����ӣ�ͬʱΪ��Щ���Ӹ���Ȩ�غ�������һ����Ӧ�㣬����Ӧ������Ҫ���Ƶ��������±���r��segment number �������ڣ���ѡ����׼�����ӼƷַ���*/
	cout << "    ��ʼALNS�㷨" << endl;
	/*���ɳ�ʼ�⡪�����ûָ�����+Լ������*/
	generateInitialSolution();
	/*��ʼ���������ӵ�Ȩ��*/

	/*��ʼ���������ﵽֹͣ��׼֮ǰ*/
		/*�������̶�ѡ���ƻ����ָ�����*/

		/*�ƻ���ǰ��*/

		/*�ָ���ǰ��*/

		/*����ģ���˻�׼���ж��Ƿ���ܵ�ǰ��*/

		/*����Adaptive layer ����*/

}
void ALNS::registerInformation(IInformation* _information)
{
	cout << "    ALNSע�뾲̬��Ϣ" << endl;
	information = _information;
	icom.registerInformation(information);
	iobm.registerInformation(information);
	iopm.registerInformation(information);
}