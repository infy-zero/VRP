#pragma once
#define fts FerryTaskSetting::
static class FerryTaskSetting
{
public:
	static constexpr int firstBefore = 60 * 5;// ��һ����Ӧ����ǰ����ӵ���
	static constexpr int waitingTime = 60 * 3;// ÿ�����ĵȴ�ʱ�䴰
	static constexpr int timeWindow = 60 * 3;// ÿ�����ĵȴ�ʱ�䴰
	static constexpr int boardingTime = 60 * 1;// �˿��ϳ�ʱ��
	static constexpr int alightingTime = 60 * 2;// �˿��³�ʱ��
	static constexpr int maxDeltaTime = 60 * 2;// �����ڶɳ����ʱ��
	static constexpr int relaxingTime = 60 * 10;// 10 min for vehicle to arrive the first task

	static constexpr int t1 = 60 * 0;// ���ﺽ�ࣺ��һ������ETAǰ��ʱ�䣺��׼��20��
	static constexpr int t2 = -60 * 10;// ���ﺽ�ࣺ���һ������ETAǰ��ʱ�䣺��׼��10��
	static constexpr int t3 = 60 * 0;// �������ࣺ��һ������ETAǰ��ʱ�䣺��׼��60��
	static constexpr int t4 = -60 * 20;// �������ࣺ���һ������ETAǰ��ʱ�䣺��׼��40��
	static constexpr int t50 = (firstBefore+boardingTime+alightingTime);// ���ﺽ��-��һ��������ʱ�䣺�ȴ�ʱ��+�ϳ�ʱ��+�³�ʱ��:35
	static constexpr int t51 = boardingTime + alightingTime;// ���ﺽ��-���һ��������ʱ�䣺�ȴ�ʱ��+�ϳ�ʱ��+�³�ʱ��:25
	static constexpr int t60 = (firstBefore + boardingTime + alightingTime);// ��������-��һ��������ʱ�䣺�ȴ�ʱ��+�ϳ�ʱ��+�³�ʱ��:20
	static constexpr int t61 = boardingTime + alightingTime;// ��������-���һ��������ʱ�䣺�ȴ�ʱ��+�ϳ�ʱ��+�³�ʱ��:15
	
};

