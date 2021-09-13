#pragma once
#define fts FerryTaskSetting::
static class FerryTaskSetting
{
public:
	static constexpr int FTS_firstBefore = 60 * 5;// ��һ����Ӧ����ǰ����ӵ���
	static constexpr int FTS_waitingTime = 60 * 3;// ÿ�����ĵȴ�ʱ�䴰
	static constexpr int FTS_TIMEWINDOW_BEFORE_START = 60 * 3;// ÿ�����ĵȴ�ʱ�䴰
	static constexpr int FTS_boardingTime = 60 * 1;// �˿��ϳ�ʱ��
	static constexpr int FTS_alightingTime = 60 * 2;// �˿��³�ʱ��
	static constexpr int FTS_maxDeltaTime = 60 * 2;// �����ڶɳ����ʱ��
	static constexpr int FTS_relaxingTime = 60 * 10;// 10 min for vehicle to arrive the first task

	static constexpr int FTS_t1 = 60 * 0;// ���ﺽ�ࣺ��һ������ETAǰ��ʱ�䣺��׼��20��
	static constexpr int FTS_t2 = -60 * 10;// ���ﺽ�ࣺ���һ������ETAǰ��ʱ�䣺��׼��10��
	static constexpr int FTS_t3 = 60 * 0;// �������ࣺ��һ������ETAǰ��ʱ�䣺��׼��60��
	static constexpr int FTS_t4 = -60 * 20;// �������ࣺ���һ������ETAǰ��ʱ�䣺��׼��40��
	static constexpr int FTS_t50 = (FTS_firstBefore+FTS_boardingTime+FTS_alightingTime);// ���ﺽ��-��һ��������ʱ�䣺�ȴ�ʱ��+�ϳ�ʱ��+�³�ʱ��:35
	static constexpr int FTS_t51 = FTS_boardingTime + FTS_alightingTime;// ���ﺽ��-���һ��������ʱ�䣺�ȴ�ʱ��+�ϳ�ʱ��+�³�ʱ��:25
	static constexpr int FTS_t60 = (FTS_firstBefore + FTS_boardingTime + FTS_alightingTime);// ��������-��һ��������ʱ�䣺�ȴ�ʱ��+�ϳ�ʱ��+�³�ʱ��:20
	static constexpr int FTS_t61 = FTS_boardingTime + FTS_alightingTime;// ��������-���һ��������ʱ�䣺�ȴ�ʱ��+�ϳ�ʱ��+�³�ʱ��:15
	
};

